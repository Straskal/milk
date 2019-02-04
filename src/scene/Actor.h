#ifndef MILK_ACTOR_H
#define MILK_ACTOR_H

#include <memory>
#include <string>
#include <unordered_map>

#include "math/Vector2d.h"

namespace milk
{
    class ActorComponent;
    class Scene;

    class Actor
    {
    public:
        /// An actor is an object that exists within a scene.
        /// An actor can range from a rock on the ground to a dragon boss monster.
        /// Attach components to an actor in order to define it and give it behavior.
        /// \param id: The Actor's unique identifier
        /// \param name: The Actor's name
        /// \param position: The Actor's position
        Actor(Scene& scene, int id, const std::string& name, const Vector2d& position);

        ~Actor();

        /// \returns the Actor's Scene.
        Scene& scene() const;

        /// \returns the actor's unique id.
        int id() const;

        // \returns the actor's name.
        std::string name() const;

        // \returns the actor's position.
        Vector2d position() const;

        /// Sets the actor's position.
        /// \param x: Actor's x coordinate
        /// \param y: Actor's x coordinate
        void position(float x, float y);

        // Returns the added component and nullptr if addition fails.

        /// Attemps to add an ActorComponent of the given type to the Actor.
        /// \tparam TComponent: The type of ActorComponent
        /// \param args The ActorComponent's constructor parameters
        /// \returns newly added ActorComponent if successfull, nullptr if not
        template<class TComponent, typename... Args>
        TComponent* addComponent(Args&& ... args)
        {
            auto type = TComponent::type;

            if ((componentBitmask_ & type) == type)
                return nullptr;

            componentBitmask_ |= type;

            componentsByType_.insert(std::make_pair(type, std::make_unique<TComponent>(*this, std::forward<Args>(args)...)));

            return getComponent<TComponent>();
        }
        /// \tparam TComponent: The type of ActorComponent
        /// \returns ActorComponent of given type if found, nullptr if not
        template<class TComponent>
        TComponent* getComponent() const
        {
            auto type = TComponent::type;

            if ((componentBitmask_ & type) != type)
                return nullptr;

            auto& component = componentsByType_.at(type);

            return dynamic_cast<TComponent*>(component.get());
        }

    private:
        Scene& scene_;

        int id_;
        std::string name_;
        Vector2d position_;
        uint32_t componentBitmask_;

        std::unordered_map<int, std::unique_ptr<ActorComponent>> componentsByType_;
    };
}

#endif

