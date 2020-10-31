#pragma once

//Will be ifdef WIN32
#include <Core/PlatformInfo.h>
#include <Core/LayerStack.h>
#include <Core/Window.h>
#include <Core/NebulaCommon.h>
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
        Window* window;
        ImGuiLayer* imGuiLayer;
        bool running = true;
        bool minimized = false;
        
        Timer fps;
        Timer ups;
        float lastFrameTime;

        Application* childInstance;
        LayerStack EngLayerStack;

        void Run();
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

    public:
        Application(Application* child);
        virtual ~Application();

        void Close();

        inline static Application* Get() { return curEngine; }
        inline ImGuiLayer* GetImGuiLayer() { return Get()->imGuiLayer; }
        inline Window* GetWindow() { return window; }

        //60 times a second
        void OnUpdate(float ts);
        void OnEvent(Event& e);
        void OnImGuiRender();

        virtual void OnGameUpdate() {};
        virtual void OnGameDraw()  {};

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* layer);

        //TODO: Reset engine
        virtual void ResetEngine() { EngineSwap(); };

    private:
        static Application* curEngine;
        
        static void EngineSwap();
        friend int ::main(int argv, char** argc);
    };
}
