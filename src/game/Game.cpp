#include "Game.h"

#include <iostream>

#include "SDL.h"

#include "assetcache/adapter/ActorTemplateCacheAdapter.h"
#include "assetcache/adapter/TextureCacheAdapter.h"

#ifdef _DEBUG
#include "debugtools/DebugTools.h"
#endif

#include "filesystem/Filesystem.h"
#include "filesystem/adapter/FilesystemAdapter.h"

#include "graphics/Color.h"
#include "graphics/Graphics.h"

#include "input/Keyboard.h"

#include "physics/Physics.h"

#include "scripting/Logic.h"
#include "scripting/api/LuaApi.h"

#include "scene/Scene.h"
#include "scene/adapter/SceneLoaderAdapter.h"

#include "utilities/Timer.h"

#include "window/adapter/WindowAdapter.h"
#include "window/adapter/RendererAdapter.h"

milk::Game::Game()
{
    initialized_ = false;
    isRunning_ = false;
}

milk::Game::~Game() = default;

void milk::Game::init(std::string configFilepath)
{
    if (initialized_)
        return;

    if (configFilepath.empty())
    {
        std::cout << "Cannot find config file" << std::endl;
        return;
    }

    luaState_.open_libraries(sol::lib::base, sol::lib::math, sol::lib::package);

    sol::load_result loadResult = luaState_.load_file(configFilepath);

    if (!loadResult.valid())
    {
        std::cout << "could not load config file!" << std::endl;
        return;
    }

    sol::table config = loadResult();

    std::string title = config["title"];
    unsigned int width = config["width"];
    unsigned int height = config["height"];
    unsigned int vwidth = config["vwidth"];
    unsigned int vheight = config["vheight"];
    bool fullscreen = config["fullscreen"];
    std::string assetRootDir = config["resourceRootDir"];
    std::string entryScene = config["entryScene"];

    window_ = std::make_unique<adapter::WindowAdapter>();

    if (!window_->init(title, width, height, vwidth, vheight, fullscreen))
        return;

    textureCache_ = std::make_unique<adapter::TextureCacheAdapter>(*window_->rendererAdapter().sdlRenderer(), assetRootDir);

    if (!textureCache_->init())
    {
        window_->free();
        return;
    }

    fileSystem_ = std::make_unique<adapter::FilesystemAdapter>(assetRootDir);

#ifdef _DEBUG
    debugTools_ = std::make_unique<DebugTools>(window_->renderer());
#endif

    sceneLoader_ = std::make_unique<adapter::SceneLoaderAdapter>(*this);

    actorTemplateCache_ = std::make_unique<adapter::ActorTemplateCacheAdapter>(assetRootDir, *fileSystem_);

    Keyboard::initialize();

    LuaApi::init(luaState_);

    logic_ = std::make_unique<Logic>(luaState());
    physics_ = std::make_unique<Physics>();
    graphics_ = std::make_unique<Graphics>(window().renderer(), textureCache());

    luaState_["Game"] = this;
    luaState_["Window"] = dynamic_cast<Window*>(window_.get());

    loadScene(entryScene);

    initialized_ = true;
}

int milk::Game::run()
{
    if (!initialized_)
        return MILK_FAIL;

    isRunning_ = true;

    const int MILLISECONDS_PER_FRAME = 1000 / 60; // = 16

    Timer frameCapTimer;

    try
    {
        while (isRunning_)
        {
            frameCapTimer.start();

            handleEvents();
            update();
            render();

            int frameTicks = frameCapTimer.milliseconds();
            if (frameTicks < MILLISECONDS_PER_FRAME)
                SDL_Delay((Uint32)(MILLISECONDS_PER_FRAME - frameTicks));
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "Fatal error occurred: " << e.what() << std::endl;

        shutDown();

        return MILK_FAIL;
    }

    shutDown();

    return MILK_SUCCESS;
}

void milk::Game::handleEvents()
{
    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent))
    {
        switch (sdlEvent.type)
        {
            case SDL_QUIT:
                quit();
                break;
            case SDL_KEYUP:
                if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
                    quit();
#ifdef _DEBUG
                if (sdlEvent.key.keysym.sym == SDLK_BACKQUOTE)
                    debugTools_->show = !debugTools_->show;
                break;
#endif
            default:
                break;
        }
    }

    // It is important that this is called AFTER polling all SDL events.
    // SDL_PollEvent internally updates SDL key/mouse/gamepad states..
    Keyboard::updateKeyboardState();
}

void milk::Game::update()
{
    if (!sceneToLoad_.empty())
    {
        if (scene_ != nullptr)
        {
            scene_->end();

            logic_->flush();
            physics_->flush();
            graphics_->flush();

#ifdef _DEBUG
            debugTools_->flush();
#endif
        }

        scene_ = sceneLoader_->load(sceneToLoad_);

        sceneToLoad_.erase();

        textureCache_->freeUnreferencedAssets();
        actorTemplateCache_->freeUnreferencedAssets();
    }

    if (scene_ == nullptr)
        return;

    // Lets handle all of the actors that were spawned last frame!
    while (auto spawned = scene_->pollSpawned())
    {
        physics_->onActorSpawned(*spawned);
        graphics_->onActorSpawned(*spawned);

#ifdef _DEBUG
        debugTools_->onActorSpawned(*spawned);
#endif

        logic_->onActorSpawned(*spawned);
    }

    // Now lets all of the actors that were destroyed last frame!
    while (auto destroyed = scene_->pollDestroyed())
    {
        physics_->onActorDestroyed(*destroyed);
        graphics_->onActorDestroyed(*destroyed);

#ifdef _DEBUG
        debugTools_->onActorDestroyed(*destroyed);
#endif

        logic_->onActorDestroyed(*destroyed);
    }

    // NOW lets handle all of the collisions last frame!
    while (auto collisionEvent = physics_->pollCollisions())
    {
        logic_->onActorCollision(*collisionEvent);
    }

    logic_->update();
    physics_->update();
    logic_->lateUpdate();
}

void milk::Game::render()
{
    window_->renderer().clear(Color::black());

    if (scene_ != nullptr)
    {
        graphics_->render(*scene_);

#ifdef _DEBUG
        debugTools_->render(*scene_);
#endif
    }

    window_->renderer().present();
}

milk::Window& milk::Game::window() const
{
    return *window_;
}

milk::Filesystem& milk::Game::filesystem() const
{
    return *fileSystem_;
}

milk::AssetCache<milk::Texture>& milk::Game::textureCache() const
{
    return *textureCache_;
}

milk::AssetCache<nlohmann::json>& milk::Game::actorTemplateCache() const
{
    return *actorTemplateCache_;
}

sol::state& milk::Game::luaState()
{
    return luaState_;
}

#ifdef _DEBUG
milk::DebugTools& milk::Game::debugTools() const
{
    return *debugTools_;
}
#endif

void milk::Game::loadScene(const std::string& name)
{
    sceneToLoad_ = name;
}

void milk::Game::shutDown()
{
    if (scene_ != nullptr)
        scene_->end();

    logic_->flush();
    physics_->flush();
    graphics_->flush();

#ifdef _DEBUG
    debugTools_->flush();
#endif

    scene_.reset();

    textureCache_->free();
    actorTemplateCache_->free();

    window_->free();
}

void milk::Game::quit()
{
    isRunning_ = false;
}
