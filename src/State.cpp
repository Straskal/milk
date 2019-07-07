#include "State.h"

bool milk::State::initialized = false;
milk::Time* milk::State::time = nullptr;
milk::Window* milk::State::window = nullptr;
milk::Renderer* milk::State::renderer = nullptr;
milk::Mouse* milk::State::mouse = nullptr;
milk::Keyboard* milk::State::keyboard = nullptr;
milk::ImageCache* milk::State::images = nullptr;
milk::SoundCache* milk::State::sounds = nullptr;
milk::AudioPlayer* milk::State::audioPlayer = nullptr;
milk::MusicCache* milk::State::music = nullptr;