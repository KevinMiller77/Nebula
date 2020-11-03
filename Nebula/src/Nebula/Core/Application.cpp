#include "EntryPoint.h"

#include <Nebula.h>

namespace Nebula
{
    Application* Application::curEngine = nullptr;


    Application::Application(Application* child)
        : childInstance(child)
    {
		NEB_PROFILE_FUNCTION();
        LOG_INF("Making engine instance\n");
        if (curEngine != nullptr)
        {
            LOG_ERR("Engine already exists!!\n");
        }
        curEngine = this;
        window = Window::Create(WindowInfo());
        window->DisableConsole();
        window->SetEventCallback(NEB_BIND_EVENT_FN(Application::OnEvent));

        VFS::Init();

        Renderer::Init();

        imGuiLayer = ImGuiLayer::Create();
        PushOverlay(imGuiLayer);
        
        //Frame and update counters
        fps.Start();
        ups.Start();
        lastFrameTime = 0.0f;
    }

    Application::~Application()
    {
		NEB_PROFILE_FUNCTION();
        for (uint32 i = 0; i < EngLayerStack.GetSize(); i++)
        {
            if (EngLayerStack[i] == imGuiLayer)
            {
                continue;
            }
            EngLayerStack.erase(i);
        }
    }

    void Application::Close()
    {
        running = false;
    }

    void Application::EngineSwap()
    {
		NEB_PROFILE_FUNCTION();
        if (curEngine == nullptr)
        {
            return;
        }
        
        delete curEngine;
        curEngine = CreateApplication();
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        if (e.GetScreenSize().x == 0 || e.GetScreenSize().y == 0)
        {
            minimized = true;
            return false;
        }

        Renderer::OnWindowResize(e.GetScreenSize().x, e.GetScreenSize().y);
        return false;
    }


    void Application::OnImGuiRender()
    {
		NEB_PROFILE_FUNCTION();
        imGuiLayer->Begin();
        for (Layer* layer : EngLayerStack)
        {
            layer->OnImGuiRender();
        }  
        imGuiLayer->End();
    }

    void Application::OnUpdate(float ts)
    {
        NEB_PROFILE_FUNCTION();

        uint32 newUpdate = ups.FrameKeep();
        
        for (Layer* layer : EngLayerStack)
        {
            layer->OnUpdate(ts);
        }


        if (childInstance != nullptr)
        {
            childInstance->OnGameUpdate();
        }
    }

    void Application::OnEvent(Event& e)
    {

        EventDispatcher dispatch(e);
        dispatch.Dispatch<WindowCloseEvent>(NEB_BIND_EVENT_FN(Application::OnWindowClose));
        dispatch.Dispatch<WindowResizeEvent>(NEB_BIND_EVENT_FN(Application::OnWindowResize));
        

        for (auto layer = EngLayerStack.rbegin(); layer != EngLayerStack.rend(); ++layer)
        {
            if (!e.IsHandled())
            {
                (*layer)->OnEvent(e);
            }
            else
            {
                break;
            }
        }

        if (!e.IsHandled())
        {
            e.Handle();
        }
    }

    void Application::PushLayer(Layer* layer)
    {
        EngLayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        EngLayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::PopLayer(Layer* layer)
    {
        EngLayerStack.PopLayer(layer);
    }

    void Application::PopOverlay(Layer* layer)
    {
        EngLayerStack.PopOverlay(layer);
    }

    void Application::Run()
    {
        NEB_PROFILE_FUNCTION();
        while (running)
        {
            {
                NEB_PROFILE_SCOPE("RunLoop");
                float time = (float)glfwGetTime();
                float ts = time - lastFrameTime;
                lastFrameTime = time;

                if (!minimized)
                {
                    {
                        NEB_PROFILE_SCOPE("Update");
                        OnUpdate(ts);
                    }
                    {
                        NEB_PROFILE_SCOPE("ImGui Render");
                        OnImGuiRender();
                    }
                }

                {
                    NEB_PROFILE_SCOPE("Window update");
                    window->OnUpdate();
                }
            }
        }
    }
}
