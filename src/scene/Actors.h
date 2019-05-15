#ifndef MILK_ACTORS_H
#define MILK_ACTORS_H

#include "common/Ids.h"
#include "common/int.h"
#include "common/HashMap.h"
#include "common/FastMap.h"
#include "common/String.h"
#include "math/Vector2.h"

namespace milk
{
	struct Actor 
	{
		U32 id = Ids<>::INVALID;
	};

	class Actors
	{
	public:
		static const U16 MAX = 20000;

		Actor create(const String& name);
		void destroy(Actor actor);
		bool alive(Actor actor) const;

		bool isTagged(Actor actor, U32 tag);
		void tag(Actor actor, U32 tag);
		void untag(Actor actor, U32 tag);
		void getByTag(Array<Actor>& tagged, U32 tag);

		String getName(Actor actor);
		void setName(Actor actor, const String& name);
		Actor getByName(const String& name);

		Vector2 getPosition(Actor actor);
		void setPosition(Actor actor, const Vector2& position);

	private:
		struct Tag
		{
			U32 actorId = 0;
			U32 mask = 0;
		};
		struct Name
		{
			U32 actorId = 0;
			String name = "";
		};

		Ids<> ids_{};
		FastMap<Name> names_{};
		FastMap<Tag> tagMasks_{};
		HashMap<U32, Vector2> positions_{};
	};
}

#endif