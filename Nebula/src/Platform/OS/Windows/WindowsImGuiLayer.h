#pragma once
#include <Core/PlatformInfo.h>
#include <ImGui/ImGuiLayer.h>

namespace Nebula{
    
    class WindowsImGuiLayer : public ImGuiLayer
    {
    public:
        WindowsImGuiLayer() : ImGuiLayer() {}

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        virtual void ShowDockSpace() override;

        virtual void Begin() override;
        virtual void End() override;
        
        virtual void OnUpdate() override {}
    private:
        float time = 0.0f;
    };
}