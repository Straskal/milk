#include "game/Game.h"

int main(int argc, char* argv[])
{
	auto& game = milk::Game::getInstance();

	game.init("res/config.lua");

	return game.run();
}