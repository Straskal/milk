#include "Actors.h"

#include <assert.h>s

namespace milk
{
	Actor Actors::create(const String& name)
	{
		U32 id = ids_.create();
		names_.push(id, Name{ id, name });
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
		return tagMasks_.contains(actor.id) && (tagMasks_.lookup(actor.id).mask & tag) == tag;
	}

	void Actors::tag(Actor actor, U32 tag)
	{
		if (tagMasks_.contains(actor.id))
		{
			tagMasks_.lookup(actor.id).mask |= tag;
			return;
		}
		Tag newTag;
		newTag.actorId = actor.id;
		newTag.mask = tag;
		tagMasks_.push(actor.id, newTag);
	}

	void Actors::untag(Actor actor, U32 tag)
	{
		if (tagMasks_.contains(actor.id))
		{
			tagMasks_.lookup(actor.id).mask &= ~tag;
		}
	}

	void Actors::getByTag(Array<Actor>& tagged, U32 tag)
	{
		for (int i = 0; i < tagMasks_.size(); ++i)
		{
			if ((tagMasks_[i].mask & tag) == tag)
			{
				tagged.push_back(Actor{ tagMasks_[i].actorId });
			}
		}
	}

	String Actors::getName(Actor actor)
	{
		return names_.lookup(actor.id).name;
	}

	void Actors::setName(Actor actor, const String & name)
	{
		names_.lookup(actor.id).name = name;
	}

	Actor Actors::getByName(const String & name)
	{
		for (int i = 0; i < names_.size(); ++i)
		{
			if (names_[i].name == name)
			{
				return Actor{ names_[i].actorId };
			}
		}
		return Actor{ Ids<>::INVALID };
	}

	Vector2 Actors::getPosition(Actor actor)
	{
		return positions_.at(actor.id);
	}

	void Actors::setPosition(Actor actor, const Vector2 & position)
	{
		positions_.at(actor.id) = position;
	}
}
