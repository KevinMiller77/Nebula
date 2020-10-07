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
		Mat4f Transform { 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const Mat4f & transform)
			: Transform(transform) {}

		operator Mat4f& () { return Transform; }
		operator const Mat4f& () const { return Transform; }
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
		bool Primary = true; // TODO: think about moving to Scene
		bool FixedAspectRatio = false;

		CameraComponent() = default;
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
}