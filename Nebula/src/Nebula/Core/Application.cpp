#include "Application.h"
#include "EntryPoint.h"

#include <Nebula.h>

namespace Nebula
{
    Application* Application::curEngine = nullptr;


    Application::Application(Application* child)
        : childInstance(child)
    {
        LOG_INF("Making engine instance\n");
        if (curEngine != nullptr)
        {
            LOG_ERR("Engine already exists!!\n");
        }
        curEngine = this;
        window = Window::Create(WindowInfo());
        window->SetEventCallback(NEB_BIND_EVENT_FN(Application::OnEvent));

        Nebula::Renderer::Init();

        imGuiLayer = ImGuiLayer::Create();
        PushOverlay(imGuiLayer);
        
        //Frame and update counters
        fps.Start();
        ups.Start();
        lastFrameTime = 0.0f;
    }

    Application::~Application()
    {
        for (uint32 i = 0; i < EngLayerStack.GetSize(); i++)
        {
            if (EngLayerStack[i] == imGuiLayer)
            {
                continue;
            }
            EngLayerStack.erase(i);
        }
        
       delete childInstance;
    }

    void Application::EngineSwap()
    {
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
        imGuiLayer->Begin();
        for (Layer* layer : EngLayerStack)
        {
            layer->OnImGuiRender();
        }  
        imGuiLayer->End();
    }

    void Application::OnUpdate(float ts)
    {
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

    void Application::Run()
    {
        while (running)
        {
            float time = (float)glfwGetTime();
            float ts = time - lastFrameTime;
            lastFrameTime = time;

            if (!minimized)
            {
                OnUpdate(ts);
                OnImGuiRender();
            }

            window->OnUpdate();
        }
    }
}
