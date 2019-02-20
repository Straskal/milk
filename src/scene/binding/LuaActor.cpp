#include "LuaActor.h"

#include "scripting/sol/sol.hpp"

#include "graphics/Animator.h"
#include "graphics/Sprite.h"
#include "physics/Velocity.h"
#include "scene/Actor.h"

namespace milk
{
	namespace lua
	{
		void LuaActor::bind(sol::state& luaState)
		{
			luaState.new_usertype<Actor>("Actor",
				"name", sol::readonly_property(&LuaActor::name),
				"position", sol::readonly_property(&LuaActor::position),
				"move", &LuaActor::move,
				"set_animation", &LuaActor::setAnimation,
				"flip_x", &LuaActor::flipX,
				"flip_y", &LuaActor::flipY);
		}

		std::string LuaActor::name(Actor& actor)
		{
			return actor.name();
		}

		Vector2 LuaActor::position(Actor& actor)
		{
			return actor.position();
		}

		void LuaActor::move(Actor& actor, float x, float y)
		{
			auto velocity = actor.getComponent<Velocity>();

			if (velocity == nullptr)
				return;

			velocity->value(x, y);
		}

		void LuaActor::setAnimation(Actor& actor, const std::string& name)
		{
			auto animator = actor.getComponent<Animator>();

			if (animator == nullptr)
				return;

			animator->setAnimation(name);
		}

		void LuaActor::flipX(Actor& actor)
		{
			auto sprite = actor.getComponent<Sprite>();

			if (sprite == nullptr)
				return;

			sprite->flipX();
		}

		void LuaActor::flipY(Actor& actor)
		{
			auto sprite = actor.getComponent<Sprite>();

			if (sprite == nullptr)
				return;

			sprite->flipY();
		}
	}
}