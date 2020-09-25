#include "ImGuiLayer.h"
#include <Core/PlatformInfo.h>

#include <Platform/Graphics/GL/ImGui/GLImGuiLayer.h>
// #include <Platform/Graphics/Metal/imgui/MetalImGuiLayer.h>
namespace Nebula
{
    ImGuiLayer* ImGuiLayer::Create()
    {
    #ifdef NEB_GAPI_OPENGL
        return new GLImGuiLayer();
    #elif defined NEB_GAPI_METAL
        return new MetalImGuiLayer();
    #endif
    }
}
