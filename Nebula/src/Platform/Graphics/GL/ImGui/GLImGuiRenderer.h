#pragma once

#include <ImGui/ImGuiLayer.h>

namespace Nebula{
    class GLImGuiRenderer : public ImGuiRenderer
    {
        public:
            virtual bool Init() override;
            virtual void Shutdown() override;
            virtual void NewFrame() override;
            virtual void Render(ImDrawData* draw_data) override;
    };
}
