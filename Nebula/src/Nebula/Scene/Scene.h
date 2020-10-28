#pragma once
#include <entt/entt.hpp>
#include <Graphics/Camera.h>

namespace Nebula
{
    class Entity;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity( const std::string& name = std::string());
        void RemoveEntity(const Entity entity);
        bool IsPrimaryCamera(Entity cameraEntity);

        void OnPlay();
        void OnUpdate(float ts);
        void OnEditingUpdate(Camera* camera);
        void OnPausedUpdate();
        void OnStop();
        void OnViewportResize(uint32_t width, uint32_t height);
        Vec2f GetViewportSize() { return Vec2f((float)ViewportWidth, (float)ViewportHeight); }

        std::string GetFilePath() { return AssociatedFilePath; }
        void SetFilePath(std::string path) { AssociatedFilePath = path; }
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