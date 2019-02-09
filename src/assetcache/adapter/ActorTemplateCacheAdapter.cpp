#include "ActorTemplateCacheAdapter.h"

#include "filesystem/adapter/FilesystemAdapter.h"

std::shared_ptr<json> milk::adapter::ActorTemplateCacheAdapter::load(const std::string& actorTemplateName)
{
    auto found = templateCache_.find(actorTemplateName);

    if (found != templateCache_.end())
        return found->second;

    using json = nlohmann::json;

    auto templateContents = FilesystemAdapter::getInstance().contents(actorTemplateName);
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
