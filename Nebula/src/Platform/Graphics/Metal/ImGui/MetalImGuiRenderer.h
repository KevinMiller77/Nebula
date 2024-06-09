#pragma once

#include <ImGui/ImGuiLayer.h>
#include <Platform/OS/MacOS/MacImGuiLayer.h>

namespace MTL {
    class CommandQueue;
    class CommandBuffer;
    class RenderCommandEncoder;
    class RenderPassDescriptor;
    class Device;
}

namespace MTK {
    class View;
}

extern MTL::CommandQueue* imgui_command_queue;
extern MTL::CommandBuffer* imgui_command_buffer;
extern MTL::RenderCommandEncoder* imgui_render_command_encoder;
extern MTL::RenderPassDescriptor* imgui_render_pass_descriptor;
extern MTK::View* imgui_view;

namespace Nebula{
    class MetalImGuiRenderer : public ImGuiRenderer
    {
        public:
            virtual bool Init() override;
            virtual void Shutdown() override;
            virtual void NewFrame() override;
            virtual void Render(ImDrawData* draw_data) override;
    };

}
