#include "IdGenerator.h"

milk::IdGenerator::IdGenerator()
        : next_(0)
{
}

int milk::IdGenerator::popId()
{
    if (availableIds_.empty())
        return next_++;

    int id = availableIds_.back();
    availableIds_.pop_back();

    return id;
}

void milk::IdGenerator::pushId(int id)
{
    for (auto& it : availableIds_)
    {
        // No duplicate ids allowed.
        if (it == id)
            return;
    }

    availableIds_.emplace_back(id);
}