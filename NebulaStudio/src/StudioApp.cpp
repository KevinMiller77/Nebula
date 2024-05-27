#include <Graphics/OrthographicCamera.h>
#include <Graphics/Renderer2D.h>
#include <Graphics/RendererAPI.h>
#include <Graphics/RendererConfig.h>
#include <Nebula.h>
#include "StudioApp.h"
#include "Base/NebulaStudioProject.h"

#include "Layers/NebulaStudioLayer.h"
#include "Layers/BootLayer.h"

#include <Scene/Scene.h>
#include <filesystem>

namespace Nebula
{
    bool hasInternalAssetDir = false;
    class Studio : public Application
    {
    public:

        Studio() : Application(this, "Nebula Studio")
        {
            NEB_PROFILE_FUNCTION();
            //TODO: Save editor configuration in the docu folder
            if (std::filesystem::exists(EDITOR_CONFIG_PATH))
            {
                // First ever use
                OpenEditorConfig();
                hasInternalAssetDir = true;
            }
            else
            {
                hasInternalAssetDir = false;
            }

            createNewProject = CreateRef<bool>();
            *createNewProject = true;

            RendererConfig::SetAlphaBlend(true);
            RendererConfig::SetMSAA(MSAA);

            window->SetVSync(true);

            // Set the size of the welcome screen
            window->SetWindowSize(550, 270);
            window->SetResizeable(false);

            LOG_INF("Application created\n");
        }

        ~Studio() = default;

        void OnAppUpdate(float ts) override
        {
            // Will only return true after project selection has finished
            if(NewProjectUpdate())
            {

            }

            // Create a testing view projection 
            // RendererConfig::SetClearColor({ 0.5f, 0.1f, 0.1f, 1.0f });
            // RendererConfig::Clear();


            // Mat4f view = Mat4f::translation(Vec3f(0.0f, 0.0f, 0.0f));
            // OrthographicCamera camera = OrthographicCamera(-1.0f, 1.0f, 1.0f, -1.0f);
            // Renderer2D::BeginScene(camera, view, false);

            // Mat4f model = Mat4f::translation(Vec3f(0.0f, 0.0f, -3.0f));
            // int entityID = 0;
            // Vec3f position = Vec3f(0.0f, 0.0f, 0.0f);
            // Vec3f scale = Vec3f(1.0f, 1.0f, 1.0f);
            // Vec3f rotation = Vec3f(0.0f, 0.0f, 0.0f);
            // Vec4f color = Vec4f(0.5f, 1.0f, 1.0f, 1.0f);
            
            // Renderer2D::DrawQuad(
            //     model,
            //     entityID,
            //     position,
            //     scale,
            //     rotation,
            //     color
            // );

            // Mat4f modelA = Mat4f::translation(Vec3f(0.0f, 0.0f, 3.0f));
            // int entityIDA = 0;
            // Vec3f positionA = Vec3f(0.0f, 0.0f, 0.0f);
            // Vec3f scaleA = Vec3f(1.0f, 1.0f, 1.0f);
            // Vec3f rotationA = Vec3f(0.0f, 0.0f, 0.0f);
            // Vec4f colorA = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
            
            // Renderer2D::DrawQuad(
            //     modelA,
            //     entityIDA,
            //     positionA,
            //     scaleA,
            //     rotationA,
            //     colorA
            // );
            
            // Renderer2D::EndScene();
        }

        void OnAppImGui() override
        {
            if (!hasInternalAssetDir)
            {
                DrawWorkingDirSelection();
            }
        }

        bool NewProjectUpdate()
        {
            if (!hasInternalAssetDir)
            {
                return false;
            }

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

                    window->SetResizeable(true);
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
                
               
                *createNewProject = false;

                return true;
            }

            return false;
        }

        int numAvgCounts = 0;
        float avgFps = 0.0f;

        bool MSAA = false;

        Ref<BootLayer> bootLayer = nullptr;
        Ref<NebulaStudioLayer> editorLayer = nullptr;
        Ref<bool> createNewProject = nullptr;
    };

}

Nebula::Application* CreateApplication()
{
    return new Nebula::Studio();
}