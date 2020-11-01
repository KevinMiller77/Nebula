#include "Base/NebulaStudioProject.h"

#include "Layers/NebulaStudioLayer.h"
#include "Layers/BootLayer.h"

namespace Nebula
{
    class Studio : public Application
    {
    public:
        
        Studio() : Application(this)
        {
            window->SetVSync(true);

            bootLayer = new BootLayer();
            PushLayer(bootLayer);
            window->SetWindowSize(550, 275);
            window->SetIcon("Icon.png");
        }
        ~Studio() = default;

        void OnGameUpdate() override
        {
            // Will only run one time when the startup menu finishes
            if (bootLayer && !bootLayer->Done().empty())
            {
                //Process info from bootLayer and save project info
                std::string projFile = bootLayer->Done();
                editorLayer = new NebulaStudioLayer(projFile);
                window->SetWindowSize((uint32_t)editorLayer->ViewportSize.x, (uint32_t)editorLayer->ViewportSize.y);

                PushLayer(editorLayer);
                
                bootLayer->Done(false);
                PopLayer(bootLayer);
                
                window->SwapIO(VFS::AbsolutePath("debug/stdin.txt"), VFS::AbsolutePath("debug/stdout.txt"), VFS::AbsolutePath("debug/stderr.txt"));
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