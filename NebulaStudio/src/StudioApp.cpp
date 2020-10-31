#include "NebulaStudioLayer.h"
#include "BootLayer.h"

namespace Nebula
{
    class Studio : public Application
    {
    public:
        
        Studio() : Application(this)
        {
            window->SetVSync(true);
            window->SetIcon("assets/internal/icon.png");

            if (!VFS::IsMounted())
            {
                bootLayer = new BootLayer();
                PushLayer(bootLayer);
                window->SetWindowSize(550, 275);
            }
            else
            {
                editorLayer = new NebulaStudioLayer();
                window->SetWindowSize((uint32_t)editorLayer->ViewportSize.x, (uint32_t)editorLayer->ViewportSize.y);
                PushLayer(editorLayer);
            }
            
        }
        ~Studio() = default;
        
        void PushEditorLayers()
        {
            //All of the layers that comprise the editor program should be added here
            PushLayer(new NebulaStudioLayer());
        }

        void OnGameUpdate() override
        {
            // Will only run one time when the startup menu finishes
            if (bootLayer && bootLayer->Done())
            {
                editorLayer = new NebulaStudioLayer();
                window->SetWindowSize((uint32_t)editorLayer->ViewportSize.x, (uint32_t)editorLayer->ViewportSize.y);

                bootLayer->Done(false);
                PopLayer(bootLayer);
                
                PushLayer(editorLayer);
            }

            RendererConfig::SetClearColor(NebulaStudioLayer::clearColor);
        }

        BootLayer* bootLayer = nullptr;
        NebulaStudioLayer* editorLayer = nullptr;
    };
}

Nebula::Application* CreateApplication()
{
    return new Nebula::Studio();
}