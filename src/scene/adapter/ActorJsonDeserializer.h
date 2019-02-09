#ifndef MILK_ACTORJSONPARSER_H
#define MILK_ACTORJSONPARSER_H

#include "json/json.hpp"

namespace milk
{
    class Actor;

    namespace adapter
    {
        class ActorJsonDeserializer
        {
        public:
            virtual void deserialize(Actor& actor, nlohmann::json& actorJson) const = 0;
        };
    }
}

#endif
