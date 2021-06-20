#pragma once

#include <stdio.h>
#include <cstring>
#include <cstdlib>
#include <Utils/Logging.h>

// Palatform FileIO interface header

namespace Nebula {
    struct FileContents {
        unsigned int length;
        char *data;
    };

    FileContents ReadFileContents(const char *filePath);
    void DestroyFileContents(FileContents fileContents);
    void DestroyFileContentsPtr(FileContents *fileContents);
}
