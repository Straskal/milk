#ifndef MILK_GAME_H
#define MILK_GAME_H

// Exit codes
#define MILK_SUCCESS 0
#define MILK_FAIL 1

#include <memory>
#include <stack>
#include <string>
#include <unordered_map>

#include "scripting/sol/sol.hpp"

namespace milk
{
	template<class T>
	class AssetCache;

	class Filesystem;
	class Renderer;
	class Texture;
	class Window;

	namespace adapter
	{
		class FilesystemAdapter;
		class RendererAdapter;
		class TextureCacheAdapter;
		class WindowAdapter;
	}

	class Game
	{
	public:
		static Game& instance()
		{
			static Game instance;
			return instance;
		}

		~Game();

		void init(std::string configFilepath);

		int run();

		Window& window() const;

		Filesystem& filesystem() const;

		AssetCache<Texture>& textureCache() const;
		
		sol::state& luaState();

		void loadScene(const std::string& name);

		void quit();

	private:
		Game();

		std::string sceneToLoad_;

		adapter::WindowAdapter* window_;
		adapter::RendererAdapter* renderer_;
		adapter::FilesystemAdapter* fileSystem_;
		adapter::TextureCacheAdapter* textureCache_;

		sol::state luaState_;

		bool initialized_;
		bool running_;

		void handleEvents();
		void update();
		void render();
		void shutDown();
	};
}

#endif
