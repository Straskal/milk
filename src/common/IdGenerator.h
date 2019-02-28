#ifndef MILK_ID_GENERATOR_H
#define MILK_ID_GENERATOR_H

#include <vector>

namespace milk
{
    /// A class for generating unique ids. It is the resposibility of the caller to recycle unused ids.
    class IdGenerator
    {
    public:
        IdGenerator();

        // \returns a unique Id
        int popId();

        /// Push Id back onto stack for re use.
        void pushId(int id);

    private:
        int next_;
        std::vector<int> availableIds_;
    };
}

#endif
