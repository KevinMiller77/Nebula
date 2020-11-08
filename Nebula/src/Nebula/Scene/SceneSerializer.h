#pragma once
#include <Core/PlatformInfo.h>
#include <Scene/Scene.h>
#include <Core/Ref.h>

#include <fstream>

namespace Nebula
{
    class SceneSerializer
    {
    public:
        SceneSerializer(Ref<Scene> scene) : SceneAttached(scene) {}

        void SerializeTxt(std::string path);
        void SerializeBin(std::string path);

        bool DeserializeTxt(std::string path);
        bool DeserializeBin(std::string path);
    private:
        Ref<Scene> SceneAttached;
    };
}