#include "Game.h"

#include <iostream>

#include "SDL.h"

#include "assetcache/adapter/TextureCacheAdapter.h"

#ifdef _DEBUG
#include "debugtools/DebugTools.h"
#endif

#include "filesystem/Filesystem.h"
#include "filesystem/adapter/FilesystemAdapter.h"

#include "input/Keyboard.h"

#include "scripting/api/LuaApi.h"

#include "states/GameState.h"
#include "states/GameState_SceneTransition.h"

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

void milk::Game::changeState(std::unique_ptr<milk::GameState> state)
{
    while (!stateStack_.empty())
        popState();

    state->begin();

    stateStack_.push(std::move(state));
}

void milk::Game::pushState(std::unique_ptr<milk::GameState> state)
{
    state->begin();

    stateStack_.push(std::move(state));
}

void milk::Game::popState()
{
    auto& state = stateStack_.top();

    state->end();

    stateStack_.pop();
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
    auto newState = stateStack_.top()->checkState();

    if (newState != nullptr)
        changeState(std::move(newState));

    stateStack_.top()->update();
}

void milk::Game::render()
{
    window_->renderer().clear();

    stateStack_.top()->render();

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

sol::state& milk::Game::luaState()
{
    return luaState_;
}

milk::DebugTools& milk::Game::debugTools() const
{
    return *debugTools_;
}

void milk::Game::loadScene(const std::string& name)
{
    stateStack_.top()->loadScene(name);
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

    Keyboard::initialize();

#ifdef _DEBUG
    debugTools_ = std::make_unique<DebugTools>(window_->renderer());
#endif

    LuaApi::init(luaState_);

    luaState_["Game"] = this;
    luaState_["Window"] = dynamic_cast<Window*>(window_.get());

    changeState(std::make_unique<GameState_SceneTransition>(*this, entryScene));

    return true;
}

void milk::Game::shutDown()
{
    while (!stateStack_.empty())
        popState();

    textureCache_->free();

    window_->free();
}

void milk::Game::quit()
{
    isRunning_ = false;
}
