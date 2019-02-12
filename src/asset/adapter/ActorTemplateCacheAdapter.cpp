#include "ActorTemplateCacheAdapter.h"

#include "filesystem/adapter/FilesystemAdapter.h"

std::shared_ptr<json> milk::adapter::ActorTemplateCacheAdapter::load(const std::string& actorTemplateName)
{
    auto found = templateCache_.find(actorTemplateName);

    if (found != templateCache_.end())
        return found->second;

    using json = nlohmann::json;

    auto templateContents = FilesystemAdapter::instance().contents(actorTemplateName);
    auto templateJson = json::parse(templateContents);

    templateCache_.insert(std::make_pair(actorTemplateName, std::make_shared<json>(templateJson)));

    return templateCache_.at(actorTemplateName);
}

void milk::adapter::ActorTemplateCacheAdapter::freeUnreferencedAssets()
{
    auto it = templateCache_.cbegin();
    while (it != templateCache_.cend())
    {
        auto curr = it++;
        if (templateCache_.find(curr->first) != templateCache_.cend())
        {
            templateCache_.erase(curr);
        }
    }
}

void milk::adapter::ActorTemplateCacheAdapter::free()
{
    templateCache_.clear();
}
