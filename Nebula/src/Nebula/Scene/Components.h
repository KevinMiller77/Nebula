#pragma once
#include <Math/math.h>

#include <Graphics/Texture.h>
#include <Graphics/Mesh.h>

#include <Utils/GUID.h>
#include "Scriptable.h"
#include "SceneCamera.h"
namespace Nebula{
    struct TagComponent
	{
		std::string Tag;
		uint64_t UUID = xorshf96();

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag), UUID(xorshf96()) {}
	};

	struct TransformComponent
	{
		// Anything can be either a quad or line
		// Quad info
		Vec3f Translation 	= {	0.0f, 0.0f, 0.0f };
		Vec3f Rotation		= { 0.0f, 0.0f, 0.0f };
		Vec3f Scale			= { 1.0f, 1.0f, 1.0f };

		// Line info
		Vec2<Vec3f> LineCoords = { {0.0f, 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f }};


		bool InheritScale = false;

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const Vec3f & translation)
			: Translation(translation) {}

		const Mat4f GetTransformation() const
		{
			Mat4f rot = Quat(Rotation).AsMat4f();
            // Mat4f rot = Quat(Rotation * (3.1412f)).AsMat4f().transpose();
			return Mat4f(1.0f) * Mat4f::translation(Translation) * rot * Mat4f::scale(Scale);
		}

		bool SetTransformation(const Mat4f& newMat)
		{
			return DecomposeTransform(newMat, Translation, Rotation, Scale);
		}
	};

	struct SpriteRendererComponent
	{

		// Render type is really only here in order to draw lines internally
		// TODO: Expose this in the ui
		enum class RenderType
		{
			NONE,
			LINE,
			QUAD,
			SOMETHING_MORE_IDK_IS_THIS_ALL_THERE_IS_TO_LIFE
		};
		

		// Remove in favor of make type NONE
		bool Hidden = false;
		RenderType Type = RenderType::QUAD;
		
		Vec4f Color { 1.0f, 1.0f, 1.0f, 1.0f };
        Ref<Texture2D> Texture = nullptr;

        float TilingFactor = 1.0f;

		// Tilemap entrance info
		bool IsTileMap = false;
		Ref<TileMap> ParentTileMap = nullptr;
		Vec2i TilePos = {0, 0};
		Vec2i TileSize = {1, 1};

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const Vec4f& color)
			: Color(color) {}
        SpriteRendererComponent(Texture2D* texture, const Vec4f& color, float tilingFactor = 1.0f)
			: Color(color), Texture(texture), TilingFactor(tilingFactor) {}
		SpriteRendererComponent(Ref<TileMap> tileMap, Vec2i tilePos, Vec2i tileSize, const Vec4f& color, float tilingFactor = 1.0f)
			: Color(color), TilingFactor(tilingFactor), TilePos(tilePos), TileSize(tileSize), ParentTileMap(tileMap),  IsTileMap(true)
			{ LoadSelectedTile(); }

		void LoadSelectedTile()
		{
			if (!IsTileMap) return;

			if (!ParentTileMap) LOG_ERR("Tilemap was invalid in Sprite Rendering Tile Load\n");
			Texture = ParentTileMap->GetTileAt(TilePos.X, TilePos.Y, TileSize.X, TileSize.Y);
		}
	};

	struct MeshComponent {
		Ref<Mesh> Mesh = nullptr;

		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;
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
		bool deleteSelf = false;

		ParentEntityComponent() = default;
		ParentEntityComponent(const ParentEntityComponent&) = default;
	};

	struct RootEntityComponent
	{
		bool VisibleOutsideRenderer = true;
		
		RootEntityComponent() = default;
		RootEntityComponent(const RootEntityComponent&) = default;
	};


    struct AudioSourceComponent
    {
        Ref<AudioSource> Source = CreateRef<AudioSource>();

        AudioSourceComponent() = default;
        AudioSourceComponent(const AudioSourceComponent&) = default;

        AudioSourceComponent(std::string& source, bool spatial) 
        : Source(AudioSource::LoadFromFile(source, spatial)) {}
        AudioSourceComponent(std::string& source, bool spatial, bool loop, float gain, float pitch) 
        : Source(AudioSource::LoadFromFile(source, spatial, loop, gain, pitch)) {}
    };
    struct AudioListenerComponent
    {
        bool IsActiveListener = false;

        AudioListenerComponent() = default;
        AudioListenerComponent(const AudioListenerComponent&) = default;
    };


}