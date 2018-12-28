#ifndef _SPRITE_
#define _SPRITE_

#include <string>

#include "SDL.h"

#include "../core/ActorComponent.h"

class Texture;

// All objects that are drawn to the screen do so via the Sprite component.
class Sprite : public ActorComponent
{
public:
	static const ComponentType type;

	Sprite(Actor& actor);
	~Sprite();

	// Load the sprites texture.
	void load(ResourceManager& resourceManager);

	// Set the sprites texture asset name.
	void textureName(const std::string& name);

	// Get the sprites texture.
	Texture* texture() const;

	// Set the sprites source rectangle.
	void sourceRect(int x, int y, int width, int height);

	// Set the sprites source rectangle.
	void sourceRect(SDL_Rect rect);

	// Get the sprites source rectangle.
	SDL_Rect getSourceRect() const;

private:
	std::string textureName_;
	Texture* texture_;
	SDL_Rect sourceRect_;
};

#endif
