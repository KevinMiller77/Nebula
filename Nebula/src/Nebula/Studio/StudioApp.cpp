
#include "NebulaStudioLayer.h"

// TODO: Make a seperate project containing this
class Studio : public Nebula::Application
{
public:
    Studio() : Application(this)
    {
        window->SetVSync(true);
        PushLayer(new NebulaStudioLayer());
    }
    ~Studio() = default;
    
    void OnGameUpdate() override
    {
        Nebula::RendererConfig::SetClearColor(NebulaStudioLayer::clearColor);
    }
};

Nebula::Application* CreateApplication()
{
    return new Studio();
}