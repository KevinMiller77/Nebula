#include "EntryPoint.h"

#include <Nebula.h>

namespace Nebula
{
    Ref<Application> Application::curEngine = nullptr;


    Application::Application(Application* child)
        : childInstance(child)
    {
		NEB_PROFILE_FUNCTION();

#if 1
        freopen("tmpout.txt", "w", stdout);
#endif
        LOG_INF("Making engine instance\n");
        if (curEngine != nullptr)
        {
            LOG_ERR("Engine already exists!!\n");
        }
        curEngine = Ref<Application>(this);
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

        curEngine = CreateApplication();
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        Renderer::OnWindowResize(e.GetScreenSize().X, e.GetScreenSize().Y);
        return false;
    }


    void Application::OnImGuiRender()
    {
		NEB_PROFILE_FUNCTION();
        imGuiLayer->Begin();

        OnGameImGui();
        
        for (Ref<Layer> layer : EngLayerStack)
        {
            layer->OnImGuiRender();
        }  
        imGuiLayer->End();
    }

    void Application::OnUpdate(float ts)
    {
        NEB_PROFILE_FUNCTION();

        uint32 newUpdate = ups.FrameKeep();
        
        for (Ref<Layer> layer : EngLayerStack)
        {
            layer->OnUpdate(ts);
        }


        if (childInstance != nullptr)
        {
            childInstance->OnGameUpdate(ts);
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

    void Application::PushLayer(Ref<Layer> layer)
    {
        EngLayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Ref<Layer> layer)
    {
        EngLayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::PopLayer(Ref<Layer> layer)
    {
        EngLayerStack.PopLayer(layer);
    }

    void Application::PopOverlay(Ref<Layer> layer)
    {
        EngLayerStack.PopOverlay(layer);
    }

    std::deque<float> rollingAvg;

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

                if (rollingAvg.size() == 60)
                {
                    rollingAvg.pop_front();
                }
                if (ts == 0.0f)
                {
                    rollingAvg.push_back(0.1f);
                }
                else
                {
                    rollingAvg.push_back(1.0f / ts);
                }
                
                
                float avg = 0.0f;
                for (auto t : rollingAvg)
                {
                    avg += t;
                }
                avg /= rollingAvg.size();

                fpsNumber = avg;
                upsNumber = avg;

                if (!window->IsMinimized())
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
