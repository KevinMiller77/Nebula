
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

	Entity Scene::GetPrimaryCamera() 
	{ 
		return { SceneMainCameraEntity, this }; 
	}

	void Scene::SetPrimaryCamera(Entity entity) 
	{ 	
		SceneMainCameraEntity = entity; 
		if (entity.GetID() != entt::null)
		{
			Registry.get<CameraComponent>(entity).Camera.RecalculateProjection();
		}
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

			EvaluateChildren();
		}
	}

	void Scene::Render(entt::entity mainCamera)
	{
		Entity camToUse;
		Mat4f transform;

		if (mainCamera == entt::null)
		{

			camToUse = { SceneMainCameraEntity, this };
			transform = SceneCameraTransform;

			if (!camToUse.IsValid())
			{
				return;
			}

			auto& camComp = Registry.get<CameraComponent>(camToUse);
			camComp.Camera.RecalculateProjection();
		}
		else
		{
			camToUse = { mainCamera, this };

			auto transfComp = Registry.get<TransformComponent>(camToUse);
			transform = transfComp.GetTransformation();
		}
		
		// Render 2D
		if (Registry.valid(camToUse))
		{
			auto& cam = Registry.get<CameraComponent>(camToUse);
			Render(cam.Camera, transform);
		}
	}

	void Scene::Render(Camera& camera, Mat4f transform)
	{
		NEB_PROFILE_FUNCTION();
		
		Renderer2D::BeginScene(camera, transform);

//	Using RenderWide to make sure children render properly
#if 1
		auto view = Registry.view<RootEntityComponent>();
		if (!view.empty())
		{
			for (auto entity : view)
			{
				Entity toSubmit { entity, this };
				SubmitEntity(toSubmit);
			}
		}
#else
		RenderWide(transform);
#endif
		Renderer2D::EndScene();
	}

///
	void Scene::Render(Mat4f viewProj)
	{
		NEB_PROFILE_FUNCTION();
		
		Renderer2D::BeginScene(viewProj);

//	Using RenderWide to make sure children render properly
#if 1
		auto view = Registry.view<RootEntityComponent>();
		if (!view.empty())
		{
			for (auto entity : view)
			{
				Entity toSubmit { entity, this };
				SubmitEntity(toSubmit);
			}
		}
#else
		RenderWide(transform);
#endif
		Renderer2D::EndScene();
	}

	void Scene::SubmitEntity(Entity entity, const Mat4f& modelMat)
	{
		auto transform = entity.GetComponent<TransformComponent>();

		if (entity.HasComponent<SpriteRendererComponent>())
		{
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
		}

		if (entity.HasComponent<ParentEntityComponent>())
		{
			Mat4f newModel = transform.GetTransformation() * modelMat;

			auto& children = entity.GetComponent<ParentEntityComponent>().children;
			for (auto child : children)
			{
				Entity toSubmit{ child, this };
				if (!toSubmit.GetComponent<TransformComponent>().InheritScale)
				{
					Vec3f pos, rot, scale;
					DecomposeTransform(transform.GetTransformation(), pos, rot, scale);
					if (scale.IsNonzero()) scale = 1.0f / scale;

					Mat4f invScale = modelMat * Mat4f::scale(scale);
					Mat4f modelDescaled = transform.GetTransformation() * invScale;
					SubmitEntity(toSubmit, modelDescaled);
				}
				else
					SubmitEntity(toSubmit, newModel);
			}
		}
	}

	void Scene::RenderWide(Mat4f transform)
	{
		WideRenderLayer nextLayer = WideRenderLayer();

		auto view = Registry.view<RootEntityComponent>();
		if (!view.empty())
		{
			for (auto entity : view)
			{
				Entity toSubmit { entity, this };

				WideRenderLayer tempWRL = SubmitEntityWide(toSubmit);
				for (auto k : tempWRL.e)
					nextLayer.e.push_back(k);
				for (auto k : tempWRL.t)
					nextLayer.t.push_back(k);
			}
		}

		while (nextLayer.e.size() != 0)
		{
			Entity entity = { nextLayer.e.front(), this } ; nextLayer.e.pop_front();
			Mat4f transf = nextLayer.t.front(); nextLayer.t.pop_front();

			WideRenderLayer tempWRL = SubmitEntityWide(entity, transf);
			for (auto k : tempWRL.e)
				nextLayer.e.push_back(k);
			for (auto k : tempWRL.t)
				nextLayer.t.push_back(k);
		}

	}

	Scene::WideRenderLayer Scene::SubmitEntityWide(Entity entity, const Mat4f& modelMat)
	{
		NEB_PROFILE_FUNCTION();
		
		WideRenderLayer out = WideRenderLayer();

		auto transform = entity.GetComponent<TransformComponent>();

		if (entity.HasComponent<SpriteRendererComponent>())
		{
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
		}

		if (entity.HasComponent<ParentEntityComponent>())
		{
			Mat4f newModel = transform.GetTransformation() * modelMat;

			auto& children = entity.GetComponent<ParentEntityComponent>().children;
			for (auto child : children)
			{
				Entity toSubmit{ child, this };

				out.e.push_back(toSubmit);
				out.t.push_back(newModel);
			}
		}

		return out;
	}
    
	void Scene::OnUpdateRuntime(float ts)
    {
		NEB_PROFILE_FUNCTION();

		// Update scripts
		{
			NEB_PROFILE_SCOPE("Update scripts");
			Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				nsc.Instance->OnUpdate(ts);
			});
		}
	
		// Check for a new main camera in the scene
		auto view = Registry.view<TransformComponent, CameraComponent>();
		
		if (SceneMainCameraEntity == entt::null)
		{
			for (auto entity : view)
			{
				auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
				camera.Camera.WantsMainCamera(false);
				camera.Camera.RecalculateProjection();
				SceneMainCameraEntity = entity;
				SceneCameraTransform = transform.GetTransformation();

				break;
			}
		}

		for (auto entity : view)
		{
			auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
			
			if (camera.Camera.WantsMainCamera())
			{
				LOG_INF("Entity %d wanted camera\n", (uint32_t)entity);
				camera.Camera.WantsMainCamera(false);
				camera.Camera.RecalculateProjection();
				SceneMainCameraEntity = entity;
			}

			if (SceneMainCameraEntity == entity)
			{
				SceneCameraTransform = transform.GetTransformation();
			}	
		}

		EvaluateChildren();
		Render();
    }

	void Scene::EvaluateChildren()
	{

		auto parentsView = Registry.view<ParentEntityComponent>();

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
	void Scene::OnUpdateEditor(float ts, Camera& camera)
    {
		NEB_PROFILE_FUNCTION();
		Mat4f vp = camera.GetViewProjection();
		Renderer2D::BeginScene(vp);
		
		auto view = Registry.view<RootEntityComponent>();
		if (!view.empty())
		{
			for (auto entity : view)
			{
				Entity toSubmit { entity, this };
				SubmitEntity(toSubmit);
			}
		}

		Renderer2D::DrawLine(Vec3f(-5000.0f, 0.0f, 0.0f), Vec3f(5000.0f, 0.0f, 0.0f), Vec4f(1.0f, 0.0f, 0.0f, 0.5f));
		Renderer2D::DrawLine(Vec3f(0.0f, -5000.0f, 0.0f), Vec3f(0.0f, 5000.0f, 0.0f), Vec4f(0.0f, 1.0f, 0.0f, 0.5f));
		Renderer2D::DrawLine(Vec3f(0.0f, 0.0f, -5000.0f), Vec3f(0.0f, 0.0f, 5000.0f), Vec4f(0.0f, 0.0f, 1.0f, 0.5f));
		Renderer2D::EndScene();
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