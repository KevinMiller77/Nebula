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

        bool NewProjectUpdate()
        {
            if (*createNewProject && !bootLayer)
            {
                bootLayer = CreateRef<BootLayer>();
                PushLayer(bootLayer);
            }

            // If the bootlayer isnt up, we clearly dont need a new project, we have one
            if (!bootLayer) return false;

            // If the done text is empty, were waiting on the user
            if (*createNewProject && !bootLayer->Done().empty())
            {
                if (bootLayer->Done() == "Cancel")
                {
                    *createNewProject = false;
                    
                    if (editorLayer)
                    {
                        PopLayer(bootLayer);
                        bootLayer = nullptr;
                        return false;
                    }
                    else
                    {
                        running = false;
                    }
                }

                // Process info from bootLayer and save project info
                std::string projFile = bootLayer->Done();
                if (!editorLayer)
                {
                    editorLayer = CreateRef<NebulaStudioLayer>(projFile, createNewProject);
                    window->SetWindowSize((uint32_t)editorLayer->ViewportSize.X, (uint32_t)editorLayer->ViewportSize.Y);

                    PushLayer(editorLayer);
                }
                else
                {
                    editorLayer->OpenProject(projFile);
                }
                

                
                bootLayer->Done(false);
                PopLayer(bootLayer);
                bootLayer = nullptr;
                
                // VFS should be set in the bootLayer
                std::string in = VFS::AbsolutePath("debug/stdin.txt");
                std::string out = VFS::AbsolutePath("debug/stdout.txt");
                std::string err = VFS::AbsolutePath("debug/stderr.txt");

                window->SwapIO(in, out, err);
                *createNewProject = false;

                return true;
            }

            return false;
        }

        Studio() : Application(this)
        {
            createNewProject = CreateRef<bool>();
            *createNewProject = true;

            window->SetVSync(true);

            window->SetWindowSize(550, 275);
            if(std::filesystem::exists("icon.png"))
            {
                window->SetIcon("icon.png");
            }

            LOG_INF("Application created\n");
        }
        ~Studio() = default;

        void OnGameUpdate() override
        {
            // Will only return true after project selection has finished
            if(NewProjectUpdate())
            {

            }

            RendererConfig::SetClearColor(NebulaStudioLayer::clearColor);
        }

        Ref<BootLayer> bootLayer = nullptr;
        Ref<NebulaStudioLayer> editorLayer = nullptr;
        Ref<bool> createNewProject = nullptr;
    };

}

Ref<Nebula::Application> CreateApplication()
{
    return CreateRef<Nebula::Studio>();
}