#ifndef MILK_ANIMATOR_H
#define MILK_ANIMATOR_H

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "scene/ActorComponent.h"
#include "utilities/Timer.h"

namespace milk
{
    class Sprite;

    /// An animation, duh
    struct Animation
    {
        explicit Animation(std::string nm)
                : name(std::move(nm))
        {
        }

        /// Name of animation
        const std::string name;

        /// Animation frames
        std::vector<int> frames;
    };

    /// All Actor's that contain animated Sprites do so via the Animator component!
    class Animator : public ActorComponent
    {
    public:
        static const ComponentType type;

        /// \param rows: The number of rows in the spritesheet
        /// \param columns: The number of columns in the spritesheet
        /// \param timeBetweenFrames: The time between frames in seconds
        Animator(Actor& actor,
                 int rows,
                 int columns,
                 float timeBetweenFrames = 0.1f);

        ~Animator() override = default;

        /// Initialize the animator.
        void init();

        /// Toggle animation paused.
        void togglePaused();

        /// Add an animation.
        /// \param animation: The animation passed does not have to worry about going out of scope. The Animator takes ownership of it.
        void addAnimation(const Animation& animation);

        /// Sets the current animation.
        /// \param name: The name of the Animation to be played.
        void setAnimation(const std::string& name);

        /// Updates the Animator's state.
        void update();

    private:
        Sprite* sprite_;

        Timer timer_;
        int currentFrame_;
        float timeBetweenFrames_;

        int rows_;
        int columns_;
        int frameWidth_;
        int frameHeight_;

        std::unordered_map<std::string, std::unique_ptr<Animation>> animations_;
        Animation* currentAnimation_;

        void updateSourceRect();
    };
}

#endif
