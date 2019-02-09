#include "BoxCollider.h"

#include "SDL.h"

#include "SpatialPartitionGrid.h"

#include "scene/Actor.h"

const milk::ComponentType milk::BoxCollider::type = BOX_COLLIDER;

milk::BoxCollider::BoxCollider(Actor& actor, int width, int height)
        : ActorComponent::ActorComponent(actor),
          origin_(Alignment::TOP_LEFT),
          rect_(0, 0, width, height),
          offset_(0, 0),
          grid_(nullptr),
          next_(nullptr),
          prev_(nullptr)
{
    updateBBox();
}

void milk::BoxCollider::init(SpatialPartitionGrid* grid)
{
    grid_ = grid;

    grid->add(this);
}

void milk::BoxCollider::center()
{
    origin_ = Alignment::CENTER;

    updateBBox();
}

void milk::BoxCollider::updateBBox()
{
    Vector2d actorPosition = actor_.position();

    // Update the old bounding box before invalidating the current one.
    oldRect_ = rect_;

    switch (origin_)
    {
        case Alignment::TOP_LEFT:
            rect_.x = (int)(actorPosition.x + offset_.x);
            rect_.y = (int)(actorPosition.y + offset_.y);
            break;
        case Alignment::CENTER:
            rect_.x = (int)(actorPosition.x - (rect_.width / 2) + offset_.x);
            rect_.y = (int)(actorPosition.y - (rect_.height / 2) + offset_.y);
            break;
    }

    if (grid_ != nullptr)
        grid_->move(this);
}

milk::Rectangle milk::BoxCollider::rect() const
{
    return rect_;
}

void milk::BoxCollider::offset(int xOffset, int yOffset)
{
    offset_.x = xOffset;
    offset_.y = yOffset;
}

bool milk::BoxCollider::overlaps(Rectangle otherRect)
{
    return rect_.overlaps(otherRect);
}

// TODO: Fix this. Get rid of SDL. For now it is gross because i am refactoring the way we poll collisions from physics.
bool milk::BoxCollider::overlaps(Rectangle otherRect, Rectangle* result)
{
    SDL_Rect rect = {rect_.x, rect_.y, rect_.width, rect_.height};
    SDL_Rect other = {otherRect.x, otherRect.y, otherRect.width, otherRect.height};
    SDL_Rect res;

    bool overlaps = SDL_IntersectRect(&rect, &other, &res);

    result->x = res.x;
    result->y = res.y;
    result->width = res.w;
    result->height = res.h;

    return overlaps;
}

int milk::BoxCollider::top()
{
    return rect_.y;
}

int milk::BoxCollider::bottom()
{
    return rect_.y + rect_.height;
}

int milk::BoxCollider::left()
{
    return rect_.x;
}

int milk::BoxCollider::right()
{
    return rect_.x + rect_.width;
}
