#include "Game.h"

#include <iostream>

#include "SDL.h"
#include "SDL_image.h"

#include "assetcache/adapter/TextureCacheAdapter.h"

#ifdef _DEBUG
#include "debugtools/DebugTools.h"
#endif

#include "events/EventQueue.h"
#include "events/GameEvents.h"

#include "filesystem/Filesystem.h"
#include "filesystem/adapter/FilesystemAdapter.h"

#include "graphics/Graphics.h"

#include "input/Keyboard.h"

#include "physics/Physics.h"

#include "scene/Scene.h"
#include "scene/SceneLoader.h"
#include "scene/SceneManager.h"

#include "scripting/api/LuaApi.h"
#include "scripting/Logic.h"

#include "utilities/Timer.h"

#include "window/adapter/WindowAdapter.h"
#include "window/adapter/RendererAdapter.h"

milk::Game::Game() = default;

milk::Game::Game(const std::string& configFile)
        : configFile_(configFile)
{
}

milk::Game::~Game() = default;

int milk::Game::run()
{
    if (!initFromConfig())
        return MILK_FAIL;

    isRunning_ = true;

    const int MILLISECONDS_PER_FRAME = 1000 / 60; // = 16

    Timer fpsTimer;
    Timer frameCapTimer;

    int countedFrames = 0;

    fpsTimer.start();

    try
    {
        while (isRunning_)
        {
            frameCapTimer.start();

            float averageFps = countedFrames / fpsTimer.seconds();
            if (averageFps > 2000000)
                averageFps = 0;

            std::cout << averageFps << std::endl;

            handleEvents();
            update();
            render();

            countedFrames++;

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
                isRunning_ = false;
                break;
            case SDL_KEYUP:
                if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
                    isRunning_ = false;
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

    // Let systems handle game events enqueued last frame.
    while (auto gameEvent = events_->poll())
    {
        physics_->handleEvent(*gameEvent);
        graphics_->handleEvent(*gameEvent);

#ifdef _DEBUG
        debugTools_->handleEvent(*gameEvent);
#endif

        // It is important to let logic handle it's event last.
        // This is because the previous systems may load resources or init components that a script depends on.
        logic_->handleEvent(*gameEvent);

        // If scene loaded, then lets free any unreferenced assets left over from the last scene.
        if (gameEvent->type() == GameEventType::SCENE_LOADED)
        {
            textureCache_->freeUnreferencedAssets();
        }
    }
}

void milk::Game::update()
{
    sceneManager_->update();
    logic_->update();
    physics_->update();

    logic_->lateUpdate();
}

void milk::Game::render()
{
    window_->renderer().clear();

    if (auto scene = sceneManager_->currentScene())
    {
        graphics_->render(*scene);

#ifdef _DEBUG
        debugTools_->render(*scene);
#endif
    }

    // TODO: This is being called BEFORE any systems start processing their newly aquired Actors
    // TODO: This is causing artifacts because the scene's tilemap is rendered BEFORE a script can center or CLAMP the camera.
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

milk::EventQueue& milk::Game::events() const
{
    return *events_;
}

milk::SceneManager& milk::Game::sceneManager() const
{
    return *sceneManager_;
}

bool milk::Game::initFromConfig()
{
    if (configFile_.empty())
    {
        std::cout << "Cannot find config file" << std::endl;
        return MILK_FAIL;
    }

    luaState_.open_libraries(sol::lib::base, sol::lib::math, sol::lib::package);

    sol::load_result loadResult = luaState_.load_file(configFile_);

    if (!loadResult.valid())
    {
        std::cout << "could not load config file!" << std::endl;
        return false;
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
        return false;

    textureCache_ = std::make_unique<adapter::TextureCacheAdapter>(*window_->rendererAdapter().sdlRenderer(), assetRootDir);

    if (!textureCache_->init())
    {
        window_->free();
        return false;
    }

    fileSystem_ = std::make_unique<adapter::FilesystemAdapter>(assetRootDir);

    events_ = std::make_unique<EventQueue>();

    sceneLoader_ = std::make_unique<SceneLoader>(*this);
    sceneManager_ = std::make_unique<SceneManager>(*events_, *sceneLoader_);


    Keyboard::initialize();

#ifdef _DEBUG
    debugTools_ = std::make_unique<DebugTools>(window_->renderer());
#endif

    logic_ = std::make_unique<Logic>(luaState_);
    physics_ = std::make_unique<Physics>(*events_);
    graphics_ = std::make_unique<Graphics>(window_->renderer(), *textureCache_);

    LuaApi::init(luaState_);

    luaState_["Window"] = dynamic_cast<Window*>(window_.get());
    luaState_["SceneManager"] = sceneManager_.get();

    sceneManager_->loadScene(entryScene);

    return true;
}

void milk::Game::shutDown()
{
    sceneManager_->loadScene(MILK_NULL_SCENE);
    // Let systems handle the newly enqueued actor destroyed events.
    handleEvents();
    // Lets load NULL_SCENE and free the previous one.
    sceneManager_->update();

    textureCache_->free();

    window_->free();
}
