#include "Actors.h"

#include <algorithm>

milk::U32 milk::actor::create(ActorData& actorData, const std::string& name) {
	U32 id = id::create(actorData.ids);
	NameData nameData;
	nameData.actorid = id;
	nameData.name = name;
	actorData.names.push_back(nameData);
	actorData.namemap.insert(std::make_pair(id, actorData.names.size() - 1));
	return id;
}

void milk::actor::destroy(ActorData& actorData, U32 actor) {
	if (!id::valid(actorData.ids, actor)) {
		return;
	}
	id::remove(actorData.ids, actor);
	U32 nameidx = actorData.namemap.at(actor);
	U32 lastidx = actorData.names.size() - 1;
	if (actorData.names.size() > 1) {
		actorData.names[nameidx] = actorData.names[lastidx];
		actorData.namemap.at(lastidx) = nameidx;
	}
	actorData.names.pop_back();
	actorData.namemap.erase(actor);
	actorData.destroyed.push_back(actor);
}

std::string milk::actor::getName(ActorData& actorData, U32 actor) {
	if (!id::valid(actorData.ids, actor)) {
		return "";
	}
	U32 nameidx = actorData.namemap.at(actor);
	return actorData.names[nameidx].name;
}

void milk::actor::setName(ActorData& actorData, U32 actor, const std::string& name) {
	if (!id::valid(actorData.ids, actor)) {
		return;
	}
	U32 nameidx = actorData.namemap.at(actor);
	actorData.names[nameidx].name = name;
}

milk::U32 milk::actor::getByName(ActorData& actorData, const std::string& name) {
	for (int i = 0; i < actorData.names.size(); ++i) {
		if (actorData.names[i].name == name) {
			return actorData.names[i].actorid;
		}
	}
	return id::INVALID;
}
