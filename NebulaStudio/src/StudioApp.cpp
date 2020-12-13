#include <Nebula.h>
#include "StudioApp.h"
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

            bootLayer = CreateRef<BootLayer>();
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
                editorLayer = CreateRef<NebulaStudioLayer>(projFile);
                window->SetWindowSize((uint32_t)editorLayer->ViewportSize.X, (uint32_t)editorLayer->ViewportSize.Y);

                PushLayer(editorLayer);
                
                bootLayer->Done(false);
                PopLayer(bootLayer);
                
                std::string in = VFS::AbsolutePath("debug/stdin.txt");
                std::string out = VFS::AbsolutePath("debug/stdout.txt");
                std::string err = VFS::AbsolutePath("debug/stderr.txt");

                window->SwapIO(in, out, err);
            }

            RendererConfig::SetClearColor(NebulaStudioLayer::clearColor);
        }

        Ref<BootLayer> bootLayer = nullptr;
        Ref<NebulaStudioLayer> editorLayer = nullptr;
    };

}

Ref<Nebula::Application> CreateApplication()
{
    return CreateRef<Nebula::Studio>();
}