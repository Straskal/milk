#include "Graphics.h"

#include <cmath>

#include "Animator.h"
#include "Sprite.h"
#include "Texture.h"

#include "events/GameEvents.h"
#include "scene/Actor.h"
#include "scene/Scene.h"
#include "window/Renderer.h"

milk::Graphics::Graphics(Renderer& renderer, AssetCache<Texture>& textureCache)
        : renderer_(renderer),
          textureCache_(textureCache)
{
}

void milk::Graphics::handleEvent(milk::GameEvent& gameEvent)
{
    switch (gameEvent.type())
    {
        case GameEventType::ACTOR_SPAWNED:
        {
            auto& spawnedEvent = dynamic_cast<ActorSpawnedEvent&>(gameEvent);
            onActorSpawned(spawnedEvent.actor());
        }
            break;
        case GameEventType::ACTOR_DETROYED:
        {
            auto& destroyedEvent = dynamic_cast<ActorDestroyedEvent&>(gameEvent);
            onActorDestroyed(destroyedEvent.actor());
        }
            break;
        case GameEventType::SCENE_CHANGED:
        {
            spritesByActorId_.clear();
        }
            break;
        default:
            break;
    }
}

void milk::Graphics::render(milk::Scene& scene)
{
    auto& tilemap = scene.tilemap();
    auto& camera = scene.camera();

    auto screenSize = renderer_.resolution();
    // camera has to handle all of this stuff.
    // if we want the camera to clamp from a script, this already has to be calculated, or the clamp will be overridden
    Vector2d camOffset = {camera.position().x - screenSize.width * 0.5f, camera.position().y - screenSize.height * 0.5f};

    for (auto& layer : tilemap.layers)
    {
        for (auto& tile : layer->tiles)
        {
            Rectangle destinationRect;
            destinationRect.x = tile->x - (int)camOffset.x;
            destinationRect.y = tile->y - (int)camOffset.y;
            destinationRect.width = tile->type.sourceRect.width;
            destinationRect.height = tile->type.sourceRect.height;

            renderer_.draw(*tilemap.texture, tile->type.sourceRect, destinationRect, 0);
        }
    }

    for (auto it : spritesByActorId_)
    {
        auto animator = it.second->actor().getComponent<Animator>();

        if (animator != nullptr)
            animator->update();

        auto texture = it.second->texture();
        auto sourceRect = it.second->sourceRect();
        auto destinationRect = it.second->destinationRect();
        destinationRect.x -= (int)camOffset.x;
        destinationRect.y -= (int)camOffset.y;

        renderer_.draw(*texture, sourceRect, destinationRect, it.second->rendererFlip());
    }
}

void milk::Graphics::onActorSpawned(milk::Actor& actor)
{
    auto sprite = actor.getComponent<Sprite>();

    if (sprite == nullptr)
        return;

    sprite->load(textureCache_);

    spritesByActorId_.insert(std::make_pair(actor.id(), sprite));

    auto anim = actor.getComponent<Animator>();

    if (anim != nullptr)
        anim->init();
}

void milk::Graphics::onActorDestroyed(Actor& actor)
{
    if (spritesByActorId_.find(actor.id()) != spritesByActorId_.end())
        spritesByActorId_.erase(actor.id());
}

void milk::Graphics::flush()
{
    spritesByActorId_.clear();
}
