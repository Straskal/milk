#include "ActorJsonDeserializerV1.h"

#include "game/Game.h"

#include "graphics/Animator.h"
#include "graphics/Sprite.h"

#include "physics/BoxCollider.h"
#include "physics/Velocity.h"

#include "scene/Actor.h"

#include "scripting/Script.h"

milk::adapter::ActorJsonDeserializerV1::ActorJsonDeserializerV1(Game& game)
    : game_(game)
{
}

void milk::adapter::ActorJsonDeserializerV1::deserialize(Actor& actor, nlohmann::json& actorJson) const
{
    using json = nlohmann::json;

    json componentsJson = actorJson["components"];

    for (const auto& componentJson : componentsJson)
    {
        auto componentType = componentJson["type"].get<std::string>();

        /// Sprite
        //////////////////////////////////////////////////////////
        if (componentType == "sprite")
        {
            auto textureName = componentJson["textureName"].get<std::string>();

            int srcRectX = componentJson["sourceRect"]["x"].get<int>();
            int srcRectY = componentJson["sourceRect"]["y"].get<int>();
            int srcRectW = componentJson["sourceRect"]["width"].get<int>();
            int srcRectH = componentJson["sourceRect"]["height"].get<int>();

            auto sprite = actor.addComponent<Sprite>(textureName);

            if (sprite != nullptr)
            {
                sprite->sourceRect(srcRectX, srcRectY, srcRectW, srcRectH);
                sprite->center();
                sprite->load(game_.textureCache());
            }
        }

            /// Animator
            //////////////////////////////////////////////////////////
        else if (componentType == "animator")
        {
            int spritesheetRows = componentJson["spriteSheetRows"].get<int>();
            int spritesheetColumns = componentJson["spriteSheetColumns"].get<int>();

            auto animator = actor.addComponent<Animator>(spritesheetRows, spritesheetColumns);

            auto animationsJson = componentJson["animations"];

            for (const auto& animationJson : animationsJson)
            {
                auto animationName = animationJson["name"].get<std::string>();

                Animation animation{animationName};

                auto framesJson = animationJson["frames"];

                for (const auto& frameJson : framesJson)
                {
                    int frame = frameJson.get<int>();
                    animation.frames.emplace_back(frame);
                }

                animator->addAnimation(animation);
            }
        }

            /// Velocity
            //////////////////////////////////////////////////////////
        else if (componentType == "velocity")
        {
            int xVelocity = componentJson["value"]["x"].get<int>();
            int yVelocity = componentJson["value"]["y"].get<int>();

            auto velocity = actor.addComponent<Velocity>();

            velocity->value(xVelocity, yVelocity);
        }

            /// Script
            //////////////////////////////////////////////////////////
        else if (componentType == "script")
        {
            auto scriptName = componentJson["name"].get<std::string>();
            auto script = actor.addComponent<Script>(scriptName);

            script->load(game_.luaState());
        }

            /// collider
            //////////////////////////////////////////////////////////
        else if (componentType == "collider")
        {
            int width = componentJson["size"]["width"].get<int>();
            int height = componentJson["size"]["height"].get<int>();
            int xOffset = componentJson["offset"]["x"].get<int>();
            int yOffset = componentJson["offset"]["y"].get<int>();

            auto collider = actor.addComponent<BoxCollider>(width, height);
            collider->offset(xOffset, yOffset);
            collider->center();
        }
    }
}
