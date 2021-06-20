#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <Core/Ref.h>

namespace Nebula {

    enum class FileType {
        None,
        AudioSource,
        Image,
        Mesh,
        Text,
        Binary
    };

    enum BinaryFileType {
        BYTE, INT, FLOAT
    };

    const std::unordered_map<FileType, std::vector<std::string>> iFileExtensions = { { FileType::AudioSource, {"ogg", "mp3"} } , { FileType::Image, {"bmp", "png", "jpg"} }, { FileType::Mesh, {"fbx", "obj"} }, { FileType::Text, {"txt"} }, { FileType::Binary, {"bin"} } }; 

    FileType GetFileTypeByName(std::string name);

    class NebulaFile {
    public:
        FileType type;
        std::string filepath;

    };

    struct iMeshScene;
    class MeshFile : public NebulaFile {
    public:
        Ref<iMeshScene> meshScene;        
    };
    // struct AudioFile : NebulaFile {

    // };
    // struct ImageFile : NebulaFile {
        
    // };
    // struct TextFile : NebulaFile {
        
    // };
    class BinaryFile : public NebulaFile {
    public:
        BinaryFileType type;
        uint32_t size;
        void* data;
    };

}