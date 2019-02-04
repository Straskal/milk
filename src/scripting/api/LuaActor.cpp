#include "LuaActor.h"

#include "graphics/Animator.h"
#include "graphics/Sprite.h"
#include "physics/Velocity.h"
#include "scene/Actor.h"

namespace milk
{
    namespace lua
    {
        namespace actor
        {
            std::string name(Actor& actor)
            {
                return actor.name();
            }

            Vector2d position(Actor& actor)
            {
                return actor.position();
            }

            void move(Actor& actor, float x, float y)
            {
                auto velocity = actor.getComponent<Velocity>();

                if (velocity == nullptr)
                    return;

                velocity->value(x, y);
            }

            void setAnimation(Actor& actor, const std::string& name)
            {
                auto animator = actor.getComponent<Animator>();

                if (animator == nullptr)
                    return;

                animator->setAnimation(name);
            }

            void flipX(Actor& actor)
            {
                auto sprite = actor.getComponent<Sprite>();

                if (sprite == nullptr)
                    return;

                sprite->flipX();
            }

            void flipY(Actor& actor)
            {
                auto sprite = actor.getComponent<Sprite>();

                if (sprite == nullptr)
                    return;

                sprite->flipY();
            }
        }
    }
}