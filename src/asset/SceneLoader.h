#ifndef MILK_SCENELOADER_H
#define MILK_SCENELOADER_H

namespace milk
{
    class Scene;

    class SceneLoader
    {
    public:
        virtual std::unique_ptr<Scene> load(const std::string& file) const = 0;
    };
}

#endif
