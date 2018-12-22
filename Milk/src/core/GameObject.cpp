#include "GameObject.h"

#include "../utilities/ResourceManager.h"

GameObject::GameObject(const std::string& textureName)
	: _textureName(textureName)
{
	_texture = nullptr;
}

GameObject::~GameObject()
{
}

void GameObject::load(ResourceManager& resourceManager)
{
	_texture = resourceManager.loadTexture(_textureName);
}

void GameObject::begin()
{
}

void GameObject::handleEvent(SDL_Event& e)
{
}

void GameObject::update()
{
}

void GameObject::end()
{
}

Texture* GameObject::getTexture() const
{
	return _texture;
}

int GameObject::getId() const
{
	return _id;
}