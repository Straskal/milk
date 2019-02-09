#ifndef MILK_SCENE_H
#define MILK_SCENE_H

#include <memory>
#include <string>
#include <unordered_map>

#include "Camera.h"

#include "math/Rectangle.h"

#include "utilities/IdGenerator.h"

namespace milk
{
    class Actor;
    class ActorLoader;
    class Tilemap;

    class Scene
    {
    public:
        /// A Scene represents the current state that the game is in.
        /// Some examples: Main menu, dungeon level, cinematic, turn based combat sequence, etc...
        /// \param id: The Scene's unique indentifier
        /// \param name: The Scene's name
        /// \param actorLoader: The scene uses the ActorLoader when spawning Actors from templates
        Scene(int id, const std::string& name, std::unique_ptr<ActorLoader> actorLoader);

        ~Scene();

        /// Spawns an Actor with the given name and position.
        /// \param actorName: The Actor's name
        /// \param position: The Actor's position
        /// \returns newly spawned Actor
        Actor* spawnActor(const std::string& actorName, Vector2d position);

        /// Spawns an Actor with the given name, position, and template.
        /// \param actorName: The Actor's name
        /// \param position: The Actor's position
        /// \param templateName: The Actor's component template
        /// \returns newly spawned Actor
        Actor* spawnActor(const std::string& actorName, Vector2d position, const std::string& templateName);

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

        /// \returns the Scene's Tilemap, or nullptr if the Scene doesn't have a tilemap.
        Tilemap* tilemap();

        /// Sets the Scene's Tilemap.
        /// \param tilemap
        void tilemap(std::unique_ptr<Tilemap> tilemap);

        /// \returns the next spawned Actor in the "to spawn" queue.
        Actor* pollSpawned();

        /// \returns the next spawned Actor in the "to destroy" queue.
        Actor* pollDestroyed();

        /// \returns The Scene's boundaries
        Rectangle bounds() const;

        /// Mark the scene as ended. After a Scene has ended, no new spawns are processed.
        void end();

    private:
        std::unique_ptr<ActorLoader> actorLoader_;

        int id_;
        std::string name_;

        IdGenerator idGenerator_;
        Camera camera_;

        std::unique_ptr<Tilemap> tilemap_;

        bool ended_;

        std::unordered_map<int, std::unique_ptr<Actor>> actorsById_;
        std::vector<std::unique_ptr<Actor>> actorsToSpawn_;
        std::vector<int> actorsToDestroy_;
    };
}

#endif
