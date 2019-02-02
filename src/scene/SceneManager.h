#ifndef MILK_SCENE_MANAGER_H
#define MILK_SCENE_MANAGER_H

#define MILK_NULL_SCENE "ABSOLUTELY_NO_GD_SCENE"

#include <memory>
#include <string>

namespace milk
{
    class EventQueue;
    class GameEvent;
    class Scene;
    class SceneLoader;

    class SceneManager
    {
    public:
        /// The SceneManager manages Scene state.
        /// \param eventQueue: The SceneManager emits GameEvents such as SCENE_LOADED, SCENE_CHANGED, etc...
        /// \param sceneLoader: The SceneManager uses the SceneLoader to load Scenes.
        explicit SceneManager(EventQueue& eventQueue, SceneLoader& sceneLoader);

        ~SceneManager();

        /// Updates the state of the current Scene
        void update();

        /// Loads a new Scene
        /// \param scene: The name of the Scene to load
        void loadScene(const std::string& scene);

        /// \returns the current Scene
        Scene* currentScene() const;

    private:
        EventQueue& eventQueue_;
        SceneLoader& sceneLoader_;

        std::unique_ptr<Scene> scene_;
        std::string sceneToLoad_;
    };
}

#endif
