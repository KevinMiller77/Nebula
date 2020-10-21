
#include "NebulaStudioLayer.h"

namespace Nebula
{
    class Studio : public Application
    {
    public:
        Studio() : Application(this)
        {
            window->SetVSync(true);
            window->SetIcon("assets/icon.png");
            PushLayer(new NebulaStudioLayer());
        }
        ~Studio() = default;
        
        void OnGameUpdate() override
        {
            RendererConfig::SetClearColor(NebulaStudioLayer::clearColor);
        }
    };
}

Nebula::Application* CreateApplication()
{
    return new Nebula::Studio();
}