#ifndef _ISCENE_
#define _ISCENE_

#include <string>

#include "SDL.h"

class Actor;

class Camera;

class Tilemap;

class IScene {
public:
    virtual Actor* spawnActor(const std::string& name) = 0;

    virtual bool destroyActor(int id) = 0;

    virtual Actor* findActor(const std::string& name) const = 0;

    virtual Camera& camera() = 0;

    virtual Tilemap& tilemap() = 0;

    virtual void syncActorLists() = 0;

    virtual SDL_Rect bounds() const = 0;
};

#endif