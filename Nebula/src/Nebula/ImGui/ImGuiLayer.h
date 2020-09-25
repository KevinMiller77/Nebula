#pragma once
#include <Core/Layer.h>

#include <events/KeyEvent.h>
#include <events/MouseEvent.h>
#include <events/WindowEvent.h>

#include <Core/Window.h>

#include <../ext/imgui/imgui.h>

namespace Nebula
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer() : Layer("ImGui Layer") {};
        
        virtual ~ImGuiLayer() = default;

        virtual void OnAttach() = 0;
        virtual void OnDetach() = 0;

        virtual void ShowDockSpace() = 0;

        virtual void Begin() = 0;
        virtual void End() = 0;
        
        virtual void OnUpdate() = 0;
        
        static ImGuiLayer* Create();
    };
}