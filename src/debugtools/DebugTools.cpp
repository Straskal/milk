#include "DebugTools.h"

#include "graphics/Color.h"

#include "scene/Actor.h"
#include "scene/Scene.h"

#include "physics/BoxCollider.h"
#include "physics/SpatialPartitionGrid.h"

#include "window/Renderer.h"

milk::DebugTools::DebugTools(Renderer& renderer)
        : renderer_(renderer)
{
}

void milk::DebugTools::onActorSpawned(Actor& actor)
{
    actorsById_.insert(std::make_pair(actor.id(), &actor));
}

void milk::DebugTools::onActorDestroyed(Actor& actor)
{
    actorsById_.erase(actor.id());
}

void milk::DebugTools::onSceneChanged()
{
    actorsById_.clear();
}

void milk::DebugTools::render(Scene& scene)
{
    if (!show)
        return;

    auto& camera = scene.camera();

    auto screenSize = renderer_.resolution();
    // camera has to handle all of this stuff.
    // if we want the camera to clamp from a script, this already has to be calculated, or the clamp will be overridden
    Vector2d camOffset = {camera.position().x - screenSize.width * 0.5f, camera.position().y - screenSize.height * 0.5f};

    int cells = SpatialPartitionGrid::NUM_CELLS;
    int size = SpatialPartitionGrid::CELL_SIZE;

    for (int i = 0; i < cells; i++)
    {
        for (int j = 0; j < cells; j++)
        {
            Rectangle dest = {(int)(j * size - camOffset.x),
                              (int)(i * size - camOffset.y),
                              size,
                              size};

            renderer_.drawRectangleOutline(dest, Color{0x00, 0xFF, 0x00, 0x5A});
        }
    }

    for (auto it : actorsById_)
    {
        auto coll = it.second->getComponent<BoxCollider>();

        if (coll != nullptr)
        {
            auto collRect = coll->rect();
            Rectangle dest = {(int)(collRect.x - camOffset.x), (int)(collRect.y - camOffset.y), collRect.width, collRect.height};
            renderer_.drawRectangle(dest, Color{0xFF, 0x00, 0x00, 0x46});
        }
    }
}

void milk::DebugTools::flush()
{
    onSceneChanged();
}
