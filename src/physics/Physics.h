#ifndef MILK_PHYSICS_H
#define MILK_PHYSICS_H

#include <memory>
#include <unordered_map>

namespace milk
{
    class Actor;
    class BoxCollider;
    class EventQueue;
    class GameEvent;
    class SpatialPartitionGrid;
    class Velocity;

    class Physics
    {
    public:
        explicit Physics(EventQueue& eventQueue);

        ~Physics();

        void handleEvent(GameEvent& gameEvent);

        void update();

        void onActorSpawned(Actor& actor);

        void onActorDestroyed(Actor& actor);

        void flush();

    private:
        EventQueue& eventQueue_;

        std::unordered_map<int, Velocity*> velocityByActorId_;
        std::unique_ptr<SpatialPartitionGrid> partitionGrid_;
    };
}

#endif
