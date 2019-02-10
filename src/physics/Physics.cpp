#include "Physics.h"

#include <vector>

#include "BoxCollider.h"
#include "SpatialPartitionGrid.h"
#include "Velocity.h"

#include "scene/Actor.h"

milk::Physics::Physics()
        : partitionGrid_(new SpatialPartitionGrid())
{
}

milk::Physics::~Physics() = default;

void milk::Physics::update()
{
    for (auto& velItr : velocityByActorId_)
    {
        Vector2 velocity = velItr.second->value();

        if (velocity == Vector2(0, 0))
            continue;

        Actor& actor = velItr.second->actor();
        Vector2 oldActorPosition = actor.position();

        auto newPos = actor.position() + velocity;
        actor.position(newPos.x, newPos.y);

        auto collider = velItr.second->actor().getComponent<BoxCollider>();

        if (collider == nullptr)
            continue;

        collider->updateBBox();

        // Get collision events from collision world.
        auto collisions = partitionGrid_->getCollisions(collider);

        // Resolve the collisions:
        // Collision events contain a collision side and intersection depth rectangle.
        // In the future, these two values can be used to create a more accurate collision.
        // For now, simply reverting back to the actors previous axis position is fine.
        for (auto collItr : collisions)
        {
            collisionEvents_.push(std::make_unique<Collision>(collItr));

            auto pos = actor.position();

            actor.position(actor.position().x, oldActorPosition.y);
            collider->updateBBox();

            if (collider->overlaps(collItr.other.rect()))
            {
                actor.position(oldActorPosition.x, pos.y);
                collider->updateBBox();
            }
        }
    }
}

void milk::Physics::onActorSpawned(Actor& actor)
{
    auto velocity = actor.getComponent<Velocity>();

    if (velocity != nullptr)
        velocityByActorId_.insert(std::make_pair(actor.id(), velocity));

    auto collider = actor.getComponent<BoxCollider>();

    if (collider != nullptr)
        collider->init(partitionGrid_.get());
}

void milk::Physics::onActorDestroyed(Actor& actor)
{
    if (velocityByActorId_.find(actor.id()) != velocityByActorId_.end())
        velocityByActorId_.erase(actor.id());

    auto collider = actor.getComponent<BoxCollider>();

    if (collider != nullptr)
        partitionGrid_->remove(collider);
}

void milk::Physics::flush()
{
    partitionGrid_->clear();
    velocityByActorId_.clear();

    std::queue<std::unique_ptr<Collision>> empty;
    std::swap(collisionEvents_, empty);
}

milk::Collision* milk::Physics::pollCollisions()
{
    static std::unique_ptr<Collision> lastPolled = nullptr;

    if (lastPolled != nullptr)
    {
        lastPolled.reset();
        lastPolled = nullptr;
    }

    if (collisionEvents_.empty())
        return nullptr;

    auto& collisionEvent = collisionEvents_.front();
    lastPolled = std::move(collisionEvent);
    collisionEvents_.pop();

    return lastPolled.get();
}
