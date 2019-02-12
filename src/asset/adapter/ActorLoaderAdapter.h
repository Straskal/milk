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
            static ActorLoaderAdapter& instance()
            {
                static ActorLoaderAdapter instance;
                return instance;
            }

            ~ActorLoaderAdapter();

            void load(Actor& actor, const std::string& templateName) const override;

        private:
            ActorLoaderAdapter() = default;
        };
    }
}

#endif
