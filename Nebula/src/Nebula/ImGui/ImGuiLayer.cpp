#include "ImGuiLayer.h"
#include <Core/PlatformInfo.h>

#include <Platform/Graphics/GL/ImGui/GLImGuiLayer.h>
// #include <Platform/Graphics/Metal/imgui/MetalImGuiLayer.h>

#include <Nebula_pch.h>
namespace Nebula
{
    Ref<ImGuiLayer> ImGuiLayer::Create()
    {
        NEB_PROFILE_FUNCTION();
    #ifdef NEB_GAPI_OPENGL
        return CreateRef<GLImGuiLayer>();
    #elif defined NEB_GAPI_METAL
        return new MetalImGuiLayer();
    #endif
    }
}
