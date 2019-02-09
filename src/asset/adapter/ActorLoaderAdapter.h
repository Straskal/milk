#ifndef MILK_ACTORLOADERADAPTER_H
#define MILK_ACTORLOADERADAPTER_H

#include "asset/ActorLoader.h"

namespace milk
{
    class Game;

    namespace adapter
    {
        class ActorLoaderAdapter : public ActorLoader
        {
        public:
            explicit ActorLoaderAdapter(Game& game);

            ~ActorLoaderAdapter();

            void load(Actor& actor, const std::string& templateName) const override;

        private:
            Game& game_;
        };
    }
}

#endif
