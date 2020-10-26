#include "Scene.h"

#include <Nebula.h>

namespace Nebula
{
    Scene::Scene()
    {
    }
    Scene::~Scene()
    {
    }

    Entity Scene::CreateEntity(const std::string& name)
    {
        Entity entity = { Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
    }

	
	void Scene::RemoveEntity(entt::entity entity)
	{
		Registry.destroy(entity);
	}

    void Scene::OnUpdate(float ts)
    {
        // Update scripts
		{
			Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				// TODO: Move to Scene::OnScenePlay
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->Entity = Entity{ entity, this };
					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(ts);
			});
		}

		auto view = Registry.view<TransformComponent, CameraComponent>();
		bool hasCamera = false;
		for (auto entity : view)
		{
			hasCamera = true;

			auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
			
			if (camera.Camera.WantsMainCamera())
			{
				LOG_INF("Entity %d wanted camera\n", (uint32_t)entity);
				camera.Camera.WantsMainCamera(false);
				SceneMainCameraEntity = entity;
			}

			if (SceneMainCameraEntity == entity)
			{
				SceneCameraTransform = transform.GetTransformation();
			}	
		}

		if (!hasCamera)
		{
			SceneMainCameraEntity = entt::null;
			SceneCameraTransform = Mat4f(1.0f);
		}

		// Render 2D
		if (Registry.valid(SceneMainCameraEntity))
		{
			auto cam = Registry.get<CameraComponent>(SceneMainCameraEntity);
			Renderer2D::BeginScene(&cam.Camera, SceneCameraTransform);

			auto group = Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                if (sprite.Texture == nullptr)
                {
                    Renderer2D::DrawQuad(transform.GetTransformation(), sprite.Color);
                }
                else
                {
                    Renderer2D::DrawQuad(transform.GetTransformation(), (const Texture2D*&)sprite.Texture, sprite.TilingFactor, sprite.Color);
                }
                
			}

			Renderer2D::EndScene();
		}
    }

    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        ViewportWidth = width;
		ViewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
    }

	bool Scene::IsPrimaryCamera(Entity cameraEntity)
	{
		if (!cameraEntity.HasComponent<CameraComponent>()) 
		{
			return false;
		}
		
		auto camera = cameraEntity.GetComponent<CameraComponent>();

		return SceneMainCameraEntity == cameraEntity;
	}

}