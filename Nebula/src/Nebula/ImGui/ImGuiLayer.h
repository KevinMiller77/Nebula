#pragma once
#include <Core/Layer.h>

#include <Events/KeyEvent.h>
#include <Events/MouseEvent.h>
#include <Events/WindowEvent.h>

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

        void BlockEvents(bool block) { m_BlockEvents = block; }

        static void SetStyle(ImGuiStyle* stylePtr);
        
        static ImGuiLayer* Create();

        private:
            bool m_BlockEvents = true;
    };
}