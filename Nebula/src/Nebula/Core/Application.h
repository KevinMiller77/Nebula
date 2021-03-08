#pragma once

//Will be ifdef WIN32
#include <Core/PlatformInfo.h>
#include <Core/LayerStack.h>
#include <Core/Window.h>
#include <Core/NebulaCommon.h>
#include <Core/Ref.h>
#include <Utils/Timer.h>
#include <Utils/Logging.h>
#include <ImGui/ImGuiLayer.h>


#include <stdio.h>
#include <string>
#include <vector>
#include <map>

int main(int argv, char** argc);

namespace Nebula
{
    class Application
    {
    protected:
        Ref<Window> window;
        Ref<ImGuiLayer> imGuiLayer;
        bool running = true;
        
        Timer fps;
        Timer ups;
        float fpsNumber;
        float upsNumber;

        float lastFrameTime;

        Application* childInstance;
        LayerStack EngLayerStack;

        void Run();
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

    public:
        Application(Application* child, std::string mainWindowName = "Nebula Deafult Window");
        virtual ~Application();


        void Close();

        inline static Ref<Application> Get() { return curEngine; }
        inline Ref<ImGuiLayer> GetImGuiLayer() { return Get()->imGuiLayer; }
        inline Ref<Window> GetWindow() { return window; }

        //60 times a second
        void OnUpdate(float ts);
        void OnEvent(Event& e);
        void OnImGuiRender();

        virtual void OnGameUpdate(float ts) {};
        virtual void OnGameDraw()  {};
        virtual void OnGameImGui() {};

        float GetUPS() { return upsNumber; }
        float GetFPS() { return fpsNumber; }
        

        void PushLayer(Ref<Layer> layer);
        void PushOverlay(Ref<Layer> layer);
        void PopLayer(Ref<Layer> layer);
        void PopOverlay(Ref<Layer> layer);

        //TODO: Reset engine
        virtual void ResetEngine() { EngineSwap(); };

    private:
        static Ref<Application> curEngine;
        
        static void EngineSwap();
        friend int ::main(int argv, char** argc);
    };
}
