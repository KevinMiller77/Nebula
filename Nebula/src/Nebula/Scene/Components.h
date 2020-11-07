#pragma once
#include <Math/math.h>
#include <Graphics/Texture.h>
#include "Scriptable.h"
#include "SceneCamera.h"

namespace Nebula
{
    struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		Vec3f Translation 	= {	0.0f, 0.0f, 0.0f };
		Vec3f Rotation		= { 0.0f, 0.0f, 0.0f };
		Vec3f Scale			= { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const Vec3f & translation)
			: Translation(translation) {}

		const Mat4f GetTransformation() const
		{
			Mat4f rot = Mat4f(1.0f) * Mat4f::rotation(Rotation.X, { 1.0f, 0.0f, 0.0f }) * Mat4f::rotation(Rotation.Y, { 0.0f, 1.0f, 0.0f }) * Mat4f::rotation(Rotation.Z, { 0.0f, 0.0f, 1.0f });
			return Mat4f(1.0f) * Mat4f::scale(Scale) * rot * Mat4f::translation(Translation);
		}
	};

	struct SpriteRendererComponent
	{
		Vec4f Color { 1.0f, 1.0f, 1.0f, 1.0f };
        Texture2D* Texture = nullptr;
        float TilingFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const Vec4f& color)
			: Color(color) {}
        SpriteRendererComponent(Texture2D* texture, const Vec4f& color, float tilingFactor = 1.0f)
			: Color(color), Texture(texture), TilingFactor(tilingFactor) {}
	};

	struct CameraComponent
	{

		SceneCamera Camera;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(Vec2f ViewportSize) { Camera.SetViewportSize((uint32_t)ViewportSize.X, (uint32_t)ViewportSize.Y); } 
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity*(*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

	struct ParentEntityComponent
	{
		std::vector<Entity> children;

		ParentEntityComponent() = default;
		ParentEntityComponent(const ParentEntityComponent&) = default;
	};
}