#ifndef MILK_ACTORLOADER_H
#define MILK_ACTORLOADER_H

#include <memory>
#include <string>

namespace milk
{
    class Actor;

    class ActorLoader
    {
    public:
        virtual void load(Actor& actor, const std::string& templateName) const = 0;
    };
}

#endif
