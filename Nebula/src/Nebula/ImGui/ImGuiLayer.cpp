#include "ImGuiLayer.h"
#include <Core/PlatformInfo.h>

#include <Graphics/RendererConfig.h>
#include <Core/Window.h>

#include <Platform/Graphics/GL/ImGui/GLImGuiRenderer.h>
#include <Platform/OS/Windows/WindowsImGuiLayer.h>

#include <Nebula_pch.h>
namespace Nebula{

    Ref<ImGuiRenderer> ImGuiLayer::m_Renderer = nullptr;

    Ref<ImGuiRenderer> ImGuiRenderer::Create() {
        switch(RendererAPI::GetAPI()) {
            
            case RendererAPI::API::None:    LOG_ERR("RendererAPI::None is currently not supported!\n"); return nullptr; 
            case RendererAPI::API::OpenGL:  return CreateRef<GLImGuiRenderer>();
        }
    }

    Ref<ImGuiLayer> ImGuiLayer::Create() {
        // If this is the first imgui layer, create a renderer
        if (!ImGuiLayer::m_Renderer) {
            ImGuiLayer::m_Renderer = ImGuiRenderer::Create();
        }
        switch (Window::GetWindowType())
		{
			case WindowType::None:    LOG_ERR("RendererAPI::None is currently not supported!\n"); return nullptr;
			case WindowType::Windows:  return CreateRef<WindowsImGuiLayer>();
		}
    }
}
