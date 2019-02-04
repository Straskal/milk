#include "ActorTemplateCacheAdapter.h"

#include "filesystem/Filesystem.h"

milk::adapter::ActorTemplateCacheAdapter::ActorTemplateCacheAdapter(const std::string& rootDir, Filesystem& filesystem)
        : AssetCache(rootDir),
          filesystem_(filesystem)
{
}

std::shared_ptr<json> milk::adapter::ActorTemplateCacheAdapter::load(const std::string& actorTemplateName)
{
    auto found = templateCache_.find(actorTemplateName);

    if (found != templateCache_.end())
        return found->second;

    using json = nlohmann::json;

    auto templateNameWithEx = actorTemplateName + ".json";

    // TODO: File system already prepends rootDir_, but we still have it. Revisit.
    auto templateContents = filesystem_.contents(templateNameWithEx);
    auto templateJson = json::parse(templateContents);

    templateCache_.insert(std::make_pair(actorTemplateName, std::make_shared<json>(templateJson)));

    return templateCache_.at(actorTemplateName);
}

void milk::adapter::ActorTemplateCacheAdapter::freeUnreferencedAssets()
{
    auto templateItr = templateCache_.begin();

    while (templateItr != templateCache_.end())
    {
        if (templateItr->second.use_count() == 0)
            templateCache_.erase(templateItr->first);

        templateItr++;
    }
}

void milk::adapter::ActorTemplateCacheAdapter::free()
{
    templateCache_.clear();
}
