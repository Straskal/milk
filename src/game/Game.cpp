#include "Game.h"

#include <iostream>

#include "SDL.h"

#include "asset/adapter/TextureCacheAdapter.h"
#include "filesystem/Filesystem.h"
#include "filesystem/adapter/FilesystemAdapter.h"
#include "graphics/Color.h"
#include "input/Keyboard.h"
#include "scripting/LuaApi.h"
#include "timer/Timer.h"
#include "window/adapter/WindowAdapter.h"
#include "window/adapter/RendererAdapter.h"
#include "scene/Actors.h"
#include "scene/Transforms.h"

milk::Game::Game()
{
    initialized_ = false;
    running_ = false;
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

    luaState_.open_libraries(sol::lib::base, sol::lib::math);

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
    std::string entryScene = config["entryScene"];

    window_ = &adapter::WindowAdapter::instance();

    if (!window_->init(title, width, height, fullscreen))
        return;

    renderer_ = &adapter::RendererAdapter::instance();

    if (!renderer_->init(window_->sdlWindow(), vwidth, vheight))
    {
        window_->free();
        return;
    }

    textureCache_ = &adapter::TextureCacheAdapter::instance();

    if (!textureCache_->init())
    {
        renderer_->free();
        window_->free();
        return;
    }

    fileSystem_ = &adapter::FilesystemAdapter::instance();

    Keyboard::initialize();

    LuaApi::init(luaState_);

    loadScene(entryScene);

    initialized_ = true;
}

int milk::Game::run()
{
    if (!initialized_)
        return MILK_FAIL;

    running_ = true;

    const int MILLISECONDS_PER_FRAME = 1000 / 60; // = 16

    Timer frameCapTimer;

    try
    {
        while (running_)
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
    
}

void milk::Game::render()
{
    renderer_->clear(Color::black());
    renderer_->present();
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

void milk::Game::loadScene(const std::string& name)
{
    sceneToLoad_ = name;
}

void milk::Game::shutDown()
{
    textureCache_->free();
    renderer_->free();
    window_->free();
}

void milk::Game::quit()
{
    running_ = false;
}
