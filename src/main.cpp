#include "SDL_main.h"

#include "core/MilkState.h"

int main(int argc, char* argv[]) {
	milk::MilkState state;
	return state.run();
}