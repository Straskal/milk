#ifndef MILK_GRAPHICS_H
#define MILK_GRAPHICS_H

#include <unordered_map>

struct SDL_Renderer;

namespace milk
{
    class Actor;
    class Camera;
    class GameEvent;
    class Renderer;
    class Scene;
    class Sprite;
    class Texture;

    template<class T>
    class AssetCache;

    struct Tilemap;

    class Graphics
    {
    public:
        /// Graphics handles drawing sprites, updating animators. etc...
        /// \param renderer: A reference to the Game's Renderer.
        /// \param textureCache: A reference to the Game's TextureCache.
        Graphics(Renderer& renderer, AssetCache<Texture>& textureCache);

        /// Handles a GameEvent.
        /// \param gameEvent: The GameEvent to handle.
        void handleEvent(GameEvent& gameEvent);

        /// Renders the current Scene.
        /// \param scene: The Game's current Scene.
        void render(Scene& scene);

        void onActorSpawned(Actor& actor);

        void onActorDestroyed(Actor& actor);

        void flush();

    private:
        Renderer& renderer_;
        AssetCache<Texture>& textureCache_;

        // We only store references to sprites in the graphics system.
        // Every frame, if the sprite also has an animator, we update the animator.
        std::unordered_map<int, Sprite*> spritesByActorId_;

    };
}

#endif
