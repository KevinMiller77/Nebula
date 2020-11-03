#pragma once

#include <ImGui/ImGuiLayer.h>

namespace Nebula
{
    class GLImGuiLayer : public ImGuiLayer
    {
    public:
        GLImGuiLayer() : ImGuiLayer() {}

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        virtual void ShowDockSpace() override;

        virtual void Begin() override;
        virtual void End() override;
        
        virtual void OnUpdate() override {}
    private:
        float time = 0.0f;
        
        //Dock props
        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        bool DockspaceEnabled = false;
    };
}
