#pragma once
#include <entt/entt.hpp>
#include <Graphics/Camera.h>
#include <Core/Ref.h>

namespace Nebula
{
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
        Scene();
        virtual ~Scene();

        virtual Entity CreateEntity( const std::string& name = std::string());
        virtual void RemoveEntity(const Entity entity);
        virtual bool IsPrimaryCamera(Entity cameraEntity);

        virtual void OnPlay();
        virtual void OnUpdate(float ts, SceneStatus status);
        virtual void OnEditingUpdate(float ts, Camera* camera);

        virtual void Render(entt::entity mainCamera = entt::null);
        virtual void Render(Camera* camera, Mat4f transform);

        virtual void OnStop();
        virtual void OnViewportResize(uint32_t width, uint32_t height);
        virtual Vec2f GetViewportSize() { return Vec2f((float)ViewportWidth, (float)ViewportHeight); }

        virtual std::string GetFilePath() { return AssociatedFilePath; }
        virtual void SetFilePath(std::string path) { AssociatedFilePath = path; }
    private:
        entt::registry Registry;
        std::string AssociatedFilePath = std::string();
        
        uint32_t ViewportWidth = 0;
        uint32_t ViewportHeight = 0;

        entt::entity SceneMainCameraEntity = entt::null;
        Mat4f SceneCameraTransform = Mat4f(1.0f);
    
        friend class Entity;
        friend class SceneSerializer;
		friend class SceneHierarchyPanel;
    };
}