#ifndef MILK_PHYSICS_H
#define MILK_PHYSICS_H

#include <memory>
#include <queue>
#include <unordered_map>

namespace milk
{
    class Actor;
    class BoxCollider;
    class Collision;
    class SpatialPartitionGrid;
    class Velocity;

    class Physics
    {
    public:
        Physics();

        ~Physics();

        void update();

        void onActorSpawned(Actor& actor);

        void onActorDestroyed(Actor& actor);

        Collision* pollCollisions();

        void flush();

    private:
        std::unordered_map<int, Velocity*> velocityByActorId_;
        std::unique_ptr<SpatialPartitionGrid> partitionGrid_;
        std::queue<std::unique_ptr<Collision>> collisionEvents_;
    };
}

#endif
