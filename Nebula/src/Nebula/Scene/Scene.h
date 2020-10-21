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
        bool IsPrimaryCamera(Entity cameraEntity);

        void OnUpdate(float ts);
        void OnViewportResize(uint32_t width, uint32_t height);
    private:
        entt::registry Registry;
        
        uint32_t ViewportWidth = 0;
        uint32_t ViewportHeight = 0;

        Camera* SceneMainCamera = nullptr;
        Mat4f SceneCameraTransform = Mat4f(1.0f);
    
        friend class Entity;
		friend class SceneHierarchyPanel;
    };
}