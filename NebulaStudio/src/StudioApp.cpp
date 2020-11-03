#include "Base/NebulaStudioProject.h"

#include "Layers/NebulaStudioLayer.h"
#include "Layers/BootLayer.h"

#include <filesystem>

namespace Nebula
{
    class Studio : public Application
    {
    public:
        
        Studio() : Application(this)
        {
            window->SetVSync(true);

            window->SetWindowSize(550, 275);
            if(std::filesystem::exists("icon.png"))
            {
                window->SetIcon("icon.png");
            }
            bootLayer = new BootLayer();
            PushLayer(bootLayer);

            LOG_INF("Application created\n");
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