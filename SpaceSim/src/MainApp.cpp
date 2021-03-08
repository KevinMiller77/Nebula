#include <Nebula.h>
#include <GameLayer.h>
class MainApp : public Nebula::Application
{
public:
    //Game Layer
    Ref<GameLayer> s_Game;

    MainApp() : Nebula::Application(this, "Space Sim")
    {
        //TODO: Settings loader or auto adjuster
        Nebula::RendererConfig::SetClearColor({0.3f, 0.3f, 0.3f, 1.0f});

        window->SetVSync(true);
        window->SetResizeable(false);

        s_Game = CreateRef<GameLayer>(); 
        PushLayer(s_Game);

    }

    ~MainApp() = default;

    void OnAppUpdate(float ts) override
    {
        
    }

    void OnAppImGui() override
    {
        ImGui::Begin("ImGui Window");
        ImGui::Text("See how easy this is? %f", GetFPS());
        ImGui::End();
    }

};


Ref<Nebula::Application> CreateApplication()
{
    return CreateRef<MainApp>();
} 