#include "Scene.h"
#include <numeric>
#include <string_view>
#include <vector>
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
		return entity;
    }

	void Scene::OnPlay()
	{
		Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
		{
			if (!nsc.Instance)
			{
				nsc.Instance = nsc.InstantiateScript();
				nsc.Instance->AttachedEntity = Entity{ entity, this };
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
					if (ent.IsValid())
					{
						RemoveEntity(ent);
					}
				}
				pec.children.clear();
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

		auto view = Registry.view<RootEntityComponent>();
		if (!view.empty())
		{
			for (auto entity : view)
			{
				Entity toSubmit { entity, this };
				
				if (toSubmit.HasComponent<SpriteRendererComponent>())
				{
					SubmitEntity(toSubmit);
				}
			}
		}

		Renderer2D::EndScene();
	}

	void Scene::SubmitEntity(Entity entity, const Mat4f& modelMat)
	{
			auto transform = entity.GetComponent<TransformComponent>();
			auto sprite = entity.GetComponent<SpriteRendererComponent>();

			Mat4f transformMat = transform.GetTransformation() * modelMat;
			if (sprite.Texture)
			{
				Renderer2D::DrawQuad(transformMat, sprite.Texture, sprite.TilingFactor, sprite.Color);
			}
			else
			{
				Renderer2D::DrawQuad(transformMat, sprite.Color);
			}

		if (entity.HasComponent<ParentEntityComponent>())
		{
			Mat4f newModel = entity.GetComponent<TransformComponent>().GetTransformation() * modelMat;

			auto& children = entity.GetComponent<ParentEntityComponent>().children;
			for (auto child : children)
			{
				Entity toSubmit{ child, this };

				if (toSubmit.HasComponent<SpriteRendererComponent>())
				{
					SubmitEntity(toSubmit, newModel);
				}
			}
		}
	}
    
	void Scene::OnUpdate(float ts, SceneStatus status)
    {
		OnUpdateCommon(ts);
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

	void Scene::OnUpdateCommon(float ts)
	{
		EvaluateChildren();
	}

	void Scene::EvaluateChildren()
	{

		auto& parentsView = Registry.view<ParentEntityComponent>();

		for (auto& parentComp : parentsView)
		{
			auto& parent = parentsView.get<ParentEntityComponent>(parentComp);
			std::vector<int> stale;

			auto& children = parent.children;
			for (int i = 0; i < children.size(); i++)
			{
				auto child = children[i];
				if (!child.IsValid())
				{
					stale.push_back(i);
				}
			}

			for (int idx : stale)
			{
				children.erase(children.begin() + idx);
			}
		}

	}

	//TODO: Remove, the scene should only really use camera inside of it, not given an external one
	void Scene::OnEditingUpdate(float ts, Camera* camera)
    {
		OnUpdateCommon(ts);
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