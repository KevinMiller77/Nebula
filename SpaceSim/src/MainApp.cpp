#include <Nebula.h>
class MainApp : public Nebula::Application
{
public:
    MainApp() : Nebula::Application(this, "Space Sim")
    {
        //TODO: Settings loader or auto adjuster
        window->SetVSync(true);
        window->SetResizeable(true);
        window->SetWindowSize(320, 280);


    }

    ~MainApp() = default;

    Ref<Nebula::Texture2D> tex;


    void OnGameUpdate(float ts) override
    {
        
    }

    void OnGameImGui() override
    {
        ImGui::Begin("ImGui Window");
        ImGui::Text("See how easy this is?");
        ImGui::End();
    }

};


Ref<Nebula::Application> CreateApplication()
{
    return CreateRef<MainApp>();
} 