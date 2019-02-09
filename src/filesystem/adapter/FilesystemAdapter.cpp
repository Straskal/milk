#include "FilesystemAdapter.h"

#include "SDL_rwops.h"
#include "SDL_types.h"

std::string milk::adapter::FilesystemAdapter::contents(const std::string& filename)
{
    SDL_RWops* rwops;
    rwops = SDL_RWFromFile(filename.c_str(), "r");

    if (rwops == nullptr)
        return nullptr;

    Sint64 fileSize = SDL_RWsize(rwops);

    auto fileContents = (char*)malloc((size_t)(fileSize + 1));

    Sint64 readTotal = 0;
    Sint64 read = 1;

    char* buffer = fileContents;

    while (readTotal < fileSize && read != 0)
    {
        read = SDL_RWread(rwops, buffer, 1, ((size_t)(fileSize - readTotal)));

        readTotal += read;
        buffer += read;
    }

    SDL_RWclose(rwops);

    if (readTotal != fileSize)
    {
        free(fileContents);
        return nullptr;
    }

    fileContents[readTotal] = '\0';

    return std::string(fileContents);
}
