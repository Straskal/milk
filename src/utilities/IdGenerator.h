#ifndef MILK_ID_GENERATOR_H
#define MILK_ID_GENERATOR_H

#include <vector>

namespace milk
{
    class IdGenerator
    {
    public:
        IdGenerator();

        // Get unique id.
        int popId();

        // Push id back onto stack for re use.
        void pushId(int id);

    private:
        std::vector<int> availableIds_;
        int count_;
    };
}

#endif
