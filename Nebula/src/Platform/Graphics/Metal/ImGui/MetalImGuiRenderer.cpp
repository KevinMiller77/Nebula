#include "MetalImGuiRenderer.h"
#include <Platform/Graphics/Metal/MetalRendererAPI.h>
#include <imgui.h>

extern bool ImGui_ImplMetal_Init_CPP(MTL::Device* device);
extern void ImGui_ImplMetal_Shutdown_CPP();
extern void ImGui_ImplMetal_NewFrame_CPP(MTL::RenderPassDescriptor* renderPassDescriptor);
extern void ImGui_ImplMetal_RenderDrawData_CPP(ImDrawData* drawData, MTL::CommandBuffer* commandBuffer, MTL::RenderCommandEncoder* commandEncoder);

namespace Nebula{

    bool MetalImGuiRenderer::Init() { 
        return ImGui_ImplMetal_Init_CPP(
            MetalRendererAPI::s_Context->m_Device
        );
    }

    void MetalImGuiRenderer::Shutdown() { 
        ImGui_ImplMetal_Shutdown_CPP();
    }

    void MetalImGuiRenderer::NewFrame() { 
        ImGui_ImplMetal_NewFrame_CPP(
            MetalRendererAPI::s_Context->m_RenderPassDescriptor
        );
    }

    void MetalImGuiRenderer::Render(ImDrawData* draw_data) {
        ImGui_ImplMetal_RenderDrawData_CPP(
            draw_data,
            MetalRendererAPI::s_Context->m_FrameCommandBuffer,
            MetalRendererAPI::s_Context->m_RenderCommandEncoder
        );
    }

}
