#include "Importer.h"

namespace Nebula {
    namespace Importer {

        FileType GetFileTypeByName(const std::string& name) {
            for (std::pair<FileType, std::vector<std::string>> extensionType : iFileExtensions) {
                for (std::string extStr : extensionType.second) {
                    if (name.substr(name.size() - extStr.size()) == extStr) {
                        return extensionType.first;
                    }
                }
            }
            return FileType::None;
        }

        Ref<NebulaFile> ImportFile(const std::string& filepath) {
            switch(GetFileTypeByName(filepath)) {
                case (FileType::Mesh): {
                    return s_ImportMesh(filepath);
                }
                default: {
                    LOG_ERR("Unknown file type given to the importer!!\n");
                    return nullptr;
                }
            }
        }
    }
}