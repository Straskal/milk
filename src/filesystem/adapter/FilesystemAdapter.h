#ifndef MILK_FILESYSTEMADAPTER_H
#define MILK_FILESYSTEMADAPTER_H

#include "filesystem/Filesystem.h"

namespace milk
{
    namespace adapter
    {
        class FilesystemAdapter : public Filesystem
        {
        public:
            static FilesystemAdapter& getInstance()
            {
                static FilesystemAdapter instance;
                return instance;
            }

            std::string contents(const std::string& filename) override;

        private:
            FilesystemAdapter() = default;
        };
    }
}


#endif
