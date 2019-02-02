#ifndef MILK_SCENE_H
#define MILK_SCENE_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Camera.h"

#include "graphics/Tilemap.h"
#include "math/Rectangle.h"
#include "utilities/IdGenerator.h"

namespace milk
{
    class Actor;
    class EventQueue;

    class Scene
    {
    public:
        /// A Scene represents the current state that the game is in.
        /// Some examples: Main menu, dungeon level, cinematic, turn based combat sequence, etc...
        /// \param eventQueue: Scenes emit GameEvents such as ACTOR_SPAWNED and ACTOR_DESTROYED
        explicit Scene(EventQueue& eventQueue);

        ~Scene();

        /// Spawns an Actor into the current Scene and returns it.
        /// \param name: The Actor's name
        /// \returns newly spawned Actor
        Actor* spawnActor(const std::string& name);

        /// Attempts to destroy an Actor with the given id.
        /// \param id: The id of the Actor to destroy
        /// \returns true if the Actor is successfully deleted
        bool destroyActor(int id);

        /// Attempts to find an actor with the given name.
        /// \param name: The name of the Actor to find
        /// \returns Actor if found and nullptr if not found
        Actor* findActor(const std::string& name) const;

        /// \returns The Scene's Camera
        Camera& camera();

        /// \returns the Scene's Tilemap
        Tilemap& tilemap();

        /// Synchronizes the Scene's internal Actor data after the last frame's actor spawnes or destroys.
        void syncActorLists();

        /// \returns The Scene's boundaries
        Rectangle bounds() const;

        /// Mark the scene as ended. After a Scene has been added, all newly spawned Actors will not emit an ACTOR_SPAWNED event.
        void end();

    private:
        EventQueue& eventQueue_;

        IdGenerator idGenerator_;
        Camera camera_;

        // TODO: break this out into an actor component.
        // This implies that every single scene needs a tilemap. What about the title screen, or a cinematic, etc...
        Tilemap tilemap_;

        bool ended_;

        std::unordered_map<int, std::unique_ptr<Actor>> actorsById_;
        std::vector<std::unique_ptr<Actor>> actorsToSpawn_;
        std::vector<int> actorsToDestroy_;
    };
}

#endif
