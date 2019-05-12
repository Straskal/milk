#include "Actors.h"

#include <assert.h>s

namespace milk
{
	Actor Actors::create(const String& name)
	{
		U32 id = ids_.create();
		names_.push(id, name);
		positions_.insert(std::make_pair(id, Vector2::zero()));
		return Actor{ id };
	}

	void Actors::destroy(Actor actor)
	{
		ids_.remove(actor.id);
		names_.remove(actor.id);
		positions_.erase(actor.id);
	}

	bool Actors::alive(Actor actor) const
	{
		return ids_.valid(actor.id);
	}

	bool Actors::isTagged(Actor actor, U32 tag)
	{
		Array<U32>& taggedActors = taggedGroups_.lookup(tag);
		for (int i = 0; i < taggedActors.size(); ++i) 
		{
			if (taggedActors[i] == actor.id)
				return true;
		}
		return false;
	}

	void Actors::tag(Actor actor, U32 tag)
	{
		taggedGroups_.lookup(tag).push_back(actor.id);
	}

	void Actors::untag(Actor actor, U32 tag)
	{
		Array<U32>& tagged = taggedGroups_.lookup(tag);
		tagged.erase(std::find(tagged.begin(), tagged.end(), actor.id));
	}

	void Actors::getByTag(Array<Actor>& tagged, U32 tag) const
	{
	}

	String Actors::getName(Actor actor)
	{
		return names_.lookup(actor.id);
	}

	void Actors::setName(Actor actor, const String& name)
	{
	}

	void Actors::getByName(const String& name) const
	{
	}

	Vector2 Actors::getPosition(Actor actor)
	{
		return positions_.at(actor.id);
	}

	void Actors::setPosition(Actor actor, const Vector2& position)
	{
		positions_.at(actor.id) = position;
	}
}
