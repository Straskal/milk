#include "game/Game.h"

int main(int argc, char* argv[])
{
	auto& game = milk::Game::instance();

	game.init("res/config.lua");

	return game.run();
}