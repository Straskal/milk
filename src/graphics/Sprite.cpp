#include "Sprite.h"

#include "graphics/Texture.h"

#include "math/Vector2.h"

#include "scene/Actor.h"

const milk::ComponentType milk::Sprite::type = SPRITE;

milk::Sprite::Sprite(milk::Actor& actor, const std::string& textureName)
        : ActorComponent::ActorComponent(actor),
          textureName_(textureName),
          alignment_(Alignment::TOP_LEFT),
          flip_(SDL_FLIP_NONE)
{

    sourceRect_ = {0, 0, 0, 0};
}

void milk::Sprite::load(AssetCache<Texture>& textureLoader)
{
    texture_ = textureLoader.load(textureName_);

    // If the source rect was never explictly set, lets default it to texture rect.
    if (sourceRect_.empty())
    {
        sourceRect_.x = 0;
        sourceRect_.y = 0;
        sourceRect_.width = texture_->width();
        sourceRect_.height = texture_->height();
    }
}

std::shared_ptr<milk::Texture> milk::Sprite::texture() const
{
    return texture_;
}

void milk::Sprite::center()
{
    alignment_ = Alignment::CENTER;
}

void milk::Sprite::sourceRect(int x, int y, int width, int height)
{
    sourceRect_.x = x;
    sourceRect_.y = y;
    sourceRect_.width = width;
    sourceRect_.height = height;
}

milk::Rectangle milk::Sprite::sourceRect() const
{
    return sourceRect_;
}

milk::Rectangle milk::Sprite::destinationRect() const
{
    Vector2 actorPosition = actor_.position();

    Rectangle destinationRect;
    destinationRect.width = sourceRect_.width;
    destinationRect.height = sourceRect_.height;

    switch (alignment_)
    {
        case Alignment::TOP_LEFT:
            destinationRect.x = (int)std::floor(actorPosition.x);
            destinationRect.y = (int)std::floor(actorPosition.y);
            break;
        case Alignment::CENTER:
            destinationRect.x = (int)std::floor(actorPosition.x) - (sourceRect_.width / 2);
            destinationRect.y = (int)std::floor(actorPosition.y) - (sourceRect_.height / 2);
            break;
    }

    return destinationRect;
}

void milk::Sprite::flipX()
{
    if (flippedX())
        flip_ &= ~SDL_FLIP_HORIZONTAL;
    else
        flip_ |= SDL_FLIP_HORIZONTAL;
}

bool milk::Sprite::flippedX()
{
    return (flip_ & SDL_FLIP_HORIZONTAL) == SDL_FLIP_HORIZONTAL;
}

void milk::Sprite::flipY()
{
    if (flippedY())
        flip_ &= ~SDL_FLIP_VERTICAL;
    else
        flip_ |= SDL_FLIP_VERTICAL;
}

bool milk::Sprite::flippedY()
{
    return (flip_ & SDL_FLIP_VERTICAL) == SDL_FLIP_VERTICAL;
}

int milk::Sprite::rendererFlip() const
{
    return flip_;
}
