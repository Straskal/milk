#include "SDL_main.h"
#include "MilkState.h"

int main(int argc, char* argv[]) {
	milk::MilkState state;
	return state.run("res/config.lua");;
}