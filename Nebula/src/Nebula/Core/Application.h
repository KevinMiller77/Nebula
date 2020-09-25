#pragma once

//Will be ifdef WIN32
#include <Core/PlatformInfo.h>
#include <Core/LayerStack.h>
#include <Core/Window.h>
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

        inline static Application* Get() { return curEngine; }
        inline Window* GetWindow() { return window; }

        //60 times a second
        void OnUpdate(float ts);
        void OnEvent(Event& e);
        void OnImGuiRender();

        virtual void OnGameUpdate() {};
        virtual void OnGameDraw()  {};

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        //TODO: Reset engine
        virtual void ResetEngine() { EngineSwap(); };

    private:
        static Application* curEngine;
        
        static void EngineSwap();
        friend int ::main(int argv, char** argc);
    };

    static Vec4f clearColor = {0.1f, 0.1f, 0.1f, 1.0f};
    class TempLayer : public Layer
    {
    public:
    
        TempLayer() = default;

        void OnUpdate(float ts) override
        {
        }

        void OnImGuiRender() override
        {
            ImGui::Begin("THIS IS IMGUI");
            ImGui::Text("This is imgui mf");
            ImGui::ColorPicker4("Background", clearColor.elements);
            ImGui::End();
        }
    };



    // TODO: Make a seperate project containing this
    class Sandbox : public Application
    {
    public:
        Sandbox() : Application(this)
        {
            window->SetVSync(1);
            PushLayer(new TempLayer());

            // GetWindow()->GetContext()->ClearColor(clearColor);
        }
        ~Sandbox() = default;
        
        void OnGameUpdate() override
        {
            GetWindow()->GetContext()->ClearColor(clearColor);
        }
    };


}
