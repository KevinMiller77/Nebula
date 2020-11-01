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
        Entity entity = Entity(Registry.create(), this);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		entity.AddComponent<ParentEntityComponent>();
		return entity;
    }

	void Scene::OnPlay()
	{
		Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
		{
			if (!nsc.Instance)
			{
				nsc.Instance = nsc.InstantiateScript();
				nsc.Instance->Entity = Entity{ entity, this };
				nsc.Instance->OnCreate();
			}
		});
	}

	
	void Scene::OnStop()
	{
		Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
		{
			if (nsc.Instance)
			{
				nsc.Instance->OnCreate();
				nsc.Instance = nullptr;
			}
		});
	}
	
	void Scene::RemoveEntity(Entity entity)
	{
		if (entity.IsValid())
		{
			if (entity.HasComponent<ParentEntityComponent>())
			{
				auto& pec = entity.GetComponent<ParentEntityComponent>();
				for (Entity ent : pec.children)
				{
					RemoveEntity(ent);
				}
			}
			Registry.destroy(entity);
		}
	}

	void Scene::Render(entt::entity mainCamera)
	{
		entt::entity camToUse;
		Mat4f transform;

		if (mainCamera == entt::null)
		{
			camToUse = Entity { SceneMainCameraEntity, this };
			transform = SceneCameraTransform;
		}
		else
		{
			camToUse = mainCamera;

			auto transfComp = Registry.get<TransformComponent>(camToUse);
			transform = transfComp.GetTransformation();
		}
		
		// Render 2D
		if (Registry.valid(camToUse))
		{
			auto& cam = Registry.get<CameraComponent>(camToUse);
			Render(&cam.Camera, transform);
		}
	}

	void Scene::Render(Camera* camera, Mat4f transform)
	{
		Renderer2D::BeginScene(camera, transform);

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

    
	void Scene::OnUpdate(float ts, SceneStatus status)
    {
		if (status == SceneStatus::PLAYING)
		{
			// Update scripts
			{
				Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{
					nsc.Instance->OnUpdate(ts);
				});
			}
		}

		if (status != SceneStatus::NOT_STARTED)
		{
			auto view = Registry.view<TransformComponent, CameraComponent>();
			bool hasCamera = false;
			for (auto entity : view)
			{
				hasCamera = true;

				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
				
				if (camera.Camera.WantsMainCamera())
				{
					//LOG_INF("Entity %d wanted camera\n", (uint32_t)entity);
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
		}

		Render();
    }

	//TODO: Remove, the scene should only really use camera inside of it, not given an external one
	void Scene::OnEditingUpdate(Camera* camera)
    {
		Render(camera, SceneCameraTransform);
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
			{
				cameraComponent.Camera.SetViewportSize(width, height);
			}
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