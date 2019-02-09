#ifndef MILK_ACTORJSONPARSERV1_H
#define MILK_ACTORJSONPARSERV1_H

#include "asset/adapter/ActorJsonDeserializer.h"

namespace milk
{
    class Game;

    namespace adapter
    {
        class ActorJsonDeserializerV1 : public ActorJsonDeserializer
        {
        public:
            explicit ActorJsonDeserializerV1(Game& game);

            void deserialize(Actor& actor, nlohmann::json& actorJson) const override;
        private:
            Game& game_;
        };
    }
}

#endif
