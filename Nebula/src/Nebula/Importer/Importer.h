#pragma once

#include <Core/Files.h>
#include "MeshImporter.h"

namespace Nebula {
    namespace Importer {
        Ref<NebulaFile> ImportFile(const std::string& filepath);
    }
}