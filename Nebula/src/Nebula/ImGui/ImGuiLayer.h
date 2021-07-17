#pragma once
#include <Core/Layer.h>

#include <Events/KeyEvent.h>
#include <Events/MouseEvent.h>
#include <Events/WindowEvent.h>

#include <Core/Window.h>
#include <Core/Ref.h>

#include <../ext/imgui/imgui.h>

namespace Nebula{
    
    class ImGuiRenderer
    {
        public:
            virtual bool Init() = 0;
            virtual void Shutdown() = 0;
            virtual void NewFrame() = 0;
            virtual void Render(ImDrawData* drawData) = 0;

            static Ref<ImGuiRenderer> Create();
    };
    
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
        
        void ShowMenuBar(bool show) {m_MenuBarEnabled = show;}

        void BlockEvents(bool block) { m_BlockEvents = block; }

        static void SetStyle(ImGuiStyle* stylePtr);
        
        static Ref<ImGuiLayer> Create();

    protected:
        static Ref<ImGuiRenderer> m_Renderer;

        bool m_BlockEvents = true;

        //Dock props
        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        bool DockspaceEnabled = false;
        bool m_MenuBarEnabled = false;
    };

}