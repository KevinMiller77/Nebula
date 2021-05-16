#include <Nebula.h>
#include <GameLayer.h>
class MainApp : public Nebula::Application
{
public:
    //Game Layer
    Ref<GameLayer> s_Game;

    MainApp() : Nebula::Application(this, "Space Sim")
    {
        window->SetVSync(true);
        window->SetResizeable(false);
        window->SetWindowSize(1280, 720);

        s_Game = CreateRef<GameLayer>(); 
        PushLayer(s_Game);

        Nebula::RendererConfig::SetClearColor({0.3f, 0.3f, 0.3f, 1.0f});
        Nebula::RendererConfig::SetAlphaBlend(true);
        Nebula::RendererConfig::SetMSAA(false);

    }

    ~MainApp() = default;

    void OnAppUpdate(float ts) override
    {
        
    }

    void OnAppImGui() override
    {

    }

};


Ref<Nebula::Application> CreateApplication()
{
    return CreateRef<MainApp>();
} 