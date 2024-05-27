#pragma once

#include <ImGui/ImGuiLayer.h>
#include <Platform/OS/MacOS/MacImGuiLayer.h>

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
