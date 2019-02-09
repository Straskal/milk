#ifndef MILK_ACTOR_COMPONENT_H
#define MILK_ACTOR_COMPONENT_H

namespace milk
{
    class Actor;

    /// Different types of ActorComponents
    enum ComponentType
    {
        SCRIPT = 1 << 0,
        SPRITE = 1 << 1,
        ANIMATOR = 1 << 2,
        BOX_COLLIDER = 1 << 3,
        VELOCITY = 1 << 4,
        MAX_COMPONENTS = 1 << 31
    };

    /// An ActorComponent is the building block of an Actor's behavior.
    class ActorComponent
    {
    public:
        /// \param actor: The Actor that owns this ActorComponent
        explicit ActorComponent(Actor& actor);

        virtual ~ActorComponent() = default;

        // \returns the ActorComponent's Actor
        Actor& actor() const;

    protected:
        Actor& actor_;
    };
}

#endif
