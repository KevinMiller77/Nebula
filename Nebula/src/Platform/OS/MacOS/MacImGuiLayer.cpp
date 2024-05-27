#include "MacImGuiLayer.h"

// Include example cpp to built imgui for Win32 
#include <Core/PlatformInfo.h>
#include <Graphics/RendererAPI.h>
#include <Graphics/RendererConfig.h>
#include <Platform/Graphics/Metal/ImGui/MetalImGuiRenderer.h>
#include <Platform/Graphics/Metal/MetalRendererAPI.h>
#include <imgui.h>

#include <imgui.h>
#include <ImGui/Styles/ImGuiStyles.h>

#include <ImGuizmo.h>

#include <Core/Application.h>

#include <Nebula_pch.h>

extern bool ImGui_ImplOSX_Init_CPP(void* view);
extern void ImGui_ImplOSX_Shutdown_CPP();
extern void ImGui_ImplOSX_NewFrame_CPP(void* view);

namespace Nebula{

#ifdef NEB_PLATFORM_MACOS
    void MacImGuiLayer::OnAttach()
    {
		NEB_PROFILE_FUNCTION();
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigDockingWithShift = true;

        NebImGui::AttemptAtStyle_Init();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        Application*app = Application::Get();

        // Setup Platform/Renderer bindings
        ImGui_ImplOSX_Init_CPP((void*)MetalRendererAPI::s_Context->m_View);
        m_Renderer->Init();
    }

    void MacImGuiLayer::OnDetach()
    {
		NEB_PROFILE_FUNCTION();
        m_Renderer->Shutdown();
        ImGui_ImplOSX_Shutdown_CPP();
        ImGui::DestroyContext();
    }

    void MacImGuiLayer::Begin()
    {
		NEB_PROFILE_FUNCTION();
        m_Renderer->NewFrame();
        ImGui_ImplOSX_NewFrame_CPP((void*)MetalRendererAPI::s_Context->m_View);
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();

        ImGui::ShowDemoWindow();
        ShowDockSpace();
    }

    void MacImGuiLayer::End()
    {
        ImGui::End();
		NEB_PROFILE_FUNCTION();
        ImGuiIO& io = ImGui::GetIO();
        Application* app = Application::Get();
        io.DisplaySize = ImVec2((float)app->GetWindow()->GetWidth(), (float)app->GetWindow()->GetHeight());

        // Rendering
        ImGui::Render();
        m_Renderer->Render(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            //HDC backup_current_context = GetDC((HWND)app->GetWindow()->GetNativeWindow());
            // ImGui::UpdatePlatformWindows();
            // ImGui::RenderPlatformWindowsDefault();
            //wglMakeCurrent(backup_current_context, (HGLRC)app->GetWindow()->GetContext()->GetNativeHandle());
        }

        RendererConfig::Clear();
    }


    void MacImGuiLayer::ShowDockSpace()
    {
        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags  =  ImGuiWindowFlags_NoDocking;
        if (m_MenuBarEnabled) {
            window_flags |= ImGuiWindowFlags_MenuBar;
        }
        if (opt_fullscreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        // dockspace_flags |= ImGuiDockNodeFlags_NoDockingInCentralNode;
        dockspace_flags |= ImGuiDockNodeFlags_PassthruCentralNode;
        dockspace_flags |= ImGuiDockNodeFlags_AutoHideTabBar;

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", &DockspaceEnabled, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
    }

#else 
    void MacImGuiLayer::OnAttach() {}
    void MacImGuiLayer::OnDetach() {}
    void MacImGuiLayer::Begin() {}
    void MacImGuiLayer::End() {}
    void MacImGuiLayer::ShowDockSpace() {}
#endif
}
