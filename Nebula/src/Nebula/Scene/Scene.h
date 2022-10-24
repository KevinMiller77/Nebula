#pragma once

#include <string>

#include <entt/entt.hpp>
#include <Graphics/EditorCamera.h>

#include <Graphics/Framebuffer.h>
#include <Graphics/RenderPass.h>

#include <Core/Ref.h>
#include <deque>


namespace Nebula{
    class Entity;


    enum class SceneStatus
    {
        NOT_STARTED = 0,
        PLAYING = 1,
        PAUSED = 2,
    };
    
    class Scene
    {
    public:
        Scene(RenderPassSpecification sceneRenderSpec);
        virtual ~Scene();

        virtual Entity CreateEntity( const std::string& name = std::string());
        virtual void RemoveEntity(const Entity entity);
        virtual bool IsPrimaryCamera(Entity cameraEntity);
        virtual Entity GetPrimaryCamera();
        virtual void SetPrimaryCamera(Entity entity);

        virtual void OnPlay();

        // Returns only if there was an error and stop should be issued
        virtual void OnUpdateRuntime(float ts);
        virtual void OnUpdateEditor(float ts, EditorCamera& camera);

        // Currently Unused
        void OnPhysicsUpdate(float ts) {}
        void EvaluateChildren();

        void BeginScene(entt::entity mainCamera = entt::null);
        void BeginScene(Camera& camera, Mat4f transform);
        void BeginScene(Mat4f viewProj);

        virtual void Render();

        void EndScene();

        virtual void SubmitEntity(Entity entity, const Mat4f& modelMat = Mat4f(1.0f));

        struct WideRenderLayer
        {
            std::deque<entt::entity> e = std::deque<entt::entity>();
            std::deque<Mat4f> t = std::deque<Mat4f>();
        };

        // TODO: Remove
        virtual void RenderWide(Mat4f transform);
        virtual WideRenderLayer SubmitEntityWide(Entity entity, const Mat4f& modelMat = Mat4f(1.0f));
        //

        virtual void OnStop();
        virtual void OnViewportResize(uint32_t width, uint32_t height);
        virtual Vec2f GetViewportSize() { return Vec2f((float)ViewportWidth, (float)ViewportHeight); }

        virtual std::string GetFilePath() { return AssociatedFilePath; }
        virtual void SetFilePath(std::string path) { AssociatedFilePath = path; }

        template <typename T>
        inline entt::basic_view<entt::entity, entt::exclude_t<>, T> GetView() { return Registry.view<T>(); }

        Ref<Framebuffer> GetFrameBuffer() { return m_RenderPass->GetSpecification().TargetFramebuffer; }

    private:
        entt::registry Registry;
        std::string AssociatedFilePath = std::string();
        
        uint32_t ViewportWidth = 0;
        uint32_t ViewportHeight = 0;

        entt::entity SceneMainCameraEntity = entt::null;
        Mat4f SceneCameraTransform = Mat4f(1.0f);

        Ref<RenderPass> m_RenderPass = nullptr;
    
        friend class Entity;
        friend class SceneSerializer;
		friend class SceneHierarchyPanel;
    };
}