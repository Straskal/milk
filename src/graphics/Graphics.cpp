#include "Graphics.h"

#include <cmath>

#include "Animator.h"
#include "Sprite.h"
#include "Texture.h"
#include "Tilemap.h"

#include "scene/Actor.h"
#include "scene/Scene.h"

#include "window/Renderer.h"

milk::Graphics::Graphics(Renderer& renderer, AssetCache<Texture>& textureCache)
        : renderer_(renderer),
          textureCache_(textureCache)
{
}

void milk::Graphics::render(milk::Scene& scene)
{
    auto& camera = scene.camera();

    auto screenSize = renderer_.resolution();

    Matrix3 camTransform = camera.transformation();

    auto tilemap = scene.tilemap();

    if (tilemap != nullptr)
    {
        for (auto& layer : tilemap->layers)
        {
            for (auto& tile : layer->tiles)
            {
                Rectangle destinationRect;

                Vector2 destVec = Vector2{(float)tile->x, (float)tile->y};
                destVec = Vector2::transform(destVec, camTransform);

                destinationRect.x = (int)destVec.x;
                destinationRect.y = (int)destVec.y;
                destinationRect.width = tile->type.sourceRect.width;
                destinationRect.height = tile->type.sourceRect.height;

                renderer_.draw(*(tilemap->texture), tile->type.sourceRect, destinationRect, 0);
            }
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

        Vector2 destVec = Vector2{(float)destinationRect.x, (float)destinationRect.y};
        destVec = Vector2::transform(destVec, camTransform);

        destinationRect.x = (int)destVec.x;
        destinationRect.y = (int)destVec.y;

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
