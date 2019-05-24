#ifndef MILK_ACTORS_H
#define MILK_ACTORS_H

#include <string>
#include <unordered_map>
#include <vector>

#include "data/Ids.h"
#include "data/int.h"

namespace milk {
	struct NameData {
		U32 actorid;
		std::string name;
	};

	struct ActorData {
		IdData ids;
		std::vector<U32> destroyed;
		std::vector<NameData> names;
		std::unordered_map<U32, U32> namemap;
	};

	namespace actor {
		U32 create(ActorData& actorData, const std::string& name);
		void destroy(ActorData& actorData, U32 actor);
		std::string getName(ActorData& actorData, U32 actor);
		void setName(ActorData& actorData, U32 actor, const std::string& name);
		U32 getByName(ActorData& actorData, const std::string& name);
	}
}

#endif