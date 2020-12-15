#include "SceneSerializer.h"

#include <Nebula.h>

namespace YAML {
	template<>
	struct convert<Nebula::Vec2i>
	{
		static Node encode(const Nebula::Vec2i& rhs)
		{
			Node node;
			node.push_back(rhs.X);
			node.push_back(rhs.Y);
			return node;
		}

		static bool decode(const Node& node, Nebula::Vec2i& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.X = node[0].as<int>();
			rhs.Y = node[1].as<int>();
			return true;
		}
	};
	template<>
	struct convert<Nebula::Vec2f>
	{
		static Node encode(const Nebula::Vec2f& rhs)
		{
			Node node;
			node.push_back(rhs.X);
			node.push_back(rhs.Y);
			return node;
		}

		static bool decode(const Node& node, Nebula::Vec2f& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.X = node[0].as<float>();
			rhs.Y = node[1].as<float>();
			return true;
		}
	};
	template<>
	struct convert<Nebula::Vec3f>
	{
		static Node encode(const Nebula::Vec3f& rhs)
		{
			Node node;
			node.push_back(rhs.X);
			node.push_back(rhs.Y);
			node.push_back(rhs.Z);
			return node;
		}

		static bool decode(const Node& node, Nebula::Vec3f& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.X = node[0].as<float>();
			rhs.Y = node[1].as<float>();
			rhs.Z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<Nebula::Vec4f>
	{
		static Node encode(const Nebula::Vec4f& rhs)
		{
			Node node;
			node.push_back(rhs.X);
			node.push_back(rhs.Y);
			node.push_back(rhs.Z);
			node.push_back(rhs.W);
			return node;
		}

		static bool decode(const Node& node, Nebula::Vec4f& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.X = node[0].as<float>();
			rhs.Y = node[1].as<float>();
			rhs.Z = node[2].as<float>();
			rhs.W = node[3].as<float>();
			return true;
		}
	};
}

namespace Nebula
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const Vec2i& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.X << v.Y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const Vec2f& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.X << v.Y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const Vec3f& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.X << v.Y << v.Z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const Vec4f& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.X << v.Y << v.Z << v.W << YAML::EndSeq;
		return out;
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap; // Entity

		auto& tagComp = entity.GetComponent<TagComponent>();
		out << YAML::Key << "Entity" << YAML::Value << tagComp.UUID; // TODO: Entity ID goes here

		out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // TagComponent
			out << YAML::Key << "Tag" << YAML::Value << tagComp.Tag;
			out << YAML::EndMap; // TagComponent

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

			out << YAML::EndMap; // TransformComponent
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap; // Camera
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
			out << YAML::EndMap; // Camera

			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

			out << YAML::EndMap; // CameraComponent
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap; // SpriteRendererComponent

			auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;
			Ref<Texture2D> tex = spriteRendererComponent.Texture;

			out << YAML::Key << "Has Tex" << YAML::Value << (bool)tex;
			if (tex)
			{
				out << YAML::Key << "Tex Path" << YAML::Value << VFS::Path(tex->GetPath());
				out << YAML::Key << "Tex Width" << YAML::Value << tex->GetWidth();
				out << YAML::Key << "Tex Height" << YAML::Value << tex->GetHeight();

				out << YAML::Key << "Is TileMap" << YAML::Value << spriteRendererComponent.IsTileMap;
				if (spriteRendererComponent.IsTileMap)
				{
					out << YAML::Key << "Tile Pos" << YAML::Value << spriteRendererComponent.TilePos;
					out << YAML::Key << "Tile Size" << YAML::Value << spriteRendererComponent.TileSize;
					out << YAML::Key << "Tile Resolution" << YAML::Value << spriteRendererComponent.ParentTileMap->GetResolution();
				}
				
			}
			
			out << YAML::EndMap; // SpriteRendererComponent
		}

		if (entity.HasComponent<RootEntityComponent>())
		{
			out << YAML::Key << "RootEntityComponent";
			out << YAML::BeginMap; // TagComponent
			out << YAML::EndMap;
		
		}
		
		if (entity.HasComponent<ParentEntityComponent>())
		{
			if (entity.GetComponent<ParentEntityComponent>().children.size() > 0)
			{
				out << YAML::Key << "ParentEntityComponent";

				out << YAML::BeginMap << YAML::Key << "Children" << YAML::Value << YAML::BeginSeq;
				for (auto entity : entity.GetComponent<ParentEntityComponent>().children)
				{
					if (entity.IsValid())
					{
						SerializeEntity(out, entity);
					}
				}
				out << YAML::EndSeq << YAML::EndMap;
			}
		}


		out << YAML::EndMap; // Entity
	}

    void SceneSerializer::SerializeTxt(std::string path)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		SceneAttached->Registry.each([&](auto entityID)
		{
			Entity entity = { entityID, SceneAttached.get() };
			if (!entity)
				return;

			if (entity.HasComponent<RootEntityComponent>())
			{
				SerializeEntity(out, entity);
			}
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(path);
		fout << out.c_str();
	}

    void SceneSerializer::SerializeBin(std::string path)
	{
	}


	//	Map to ensure that we dont load the same tilemap a million times
	std::unordered_map<std::string, Ref<TileMap>> deserializeTileMaps;

    bool SceneSerializer::DeserializeTxt(std::string path)
	{
		std::ifstream stream(path);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();

		deserializeTileMaps.clear();
		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				DeserializeEntity(entity);
			}
		}
		deserializeTileMaps.clear();

		return true;
	}	

	
	Entity SceneSerializer::DeserializeEntity(YAML::detail::iterator_value& entity)
	{
		uint64_t uuid = entity["Entity"].as<uint64_t>();

		std::string name;
		auto tagComponent = entity["TagComponent"];
		if (tagComponent)
		{
			name = tagComponent["Tag"].as<std::string>();
		}

		Entity deserializedEntity = SceneAttached->CreateEntity(name);


		auto transformComponent = entity["TransformComponent"];
		if (transformComponent)
		{
			// Entities always have transforms
			auto& tc = deserializedEntity.GetComponent<TransformComponent>();
			tc.Translation = transformComponent["Translation"].as<Vec3f>();
			tc.Rotation = transformComponent["Rotation"].as<Vec3f>();
			tc.Scale = transformComponent["Scale"].as<Vec3f>();
		}

		auto cameraComponent = entity["CameraComponent"];
		if (cameraComponent)
		{
			auto& cc = deserializedEntity.AddComponent<CameraComponent>();

			auto cameraProps = cameraComponent["Camera"];
			cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

			cc.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
			cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
			cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

			cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
			cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
			cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

			cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
			cc.Camera.SetFixedAspectRatio(cc.FixedAspectRatio);
		}

		auto spriteRendererComponent = entity["SpriteRendererComponent"];
		if (spriteRendererComponent)
		{
			auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
			src.Color = spriteRendererComponent["Color"].as<Vec4f>();
			{
				if (spriteRendererComponent["Has Tex"].as<bool>())
				{
					std::string path = spriteRendererComponent["Tex Path"].as<std::string>();
					if (path.empty() || !VFS::Exists(path)) 
					{
						LOG_ERR("While loading entity, the following texture was unable to be loaded ..\n %s\n", path.c_str());
					}
					else
					{
						src.IsTileMap = spriteRendererComponent["Is TileMap"].as<bool>();
						if (spriteRendererComponent["Is TileMap"].as<bool>())
						{
							Ref<TileMap> tm;
							if (deserializeTileMaps.find(path) != deserializeTileMaps.end())
							{
								tm = deserializeTileMaps[path];
							}	
							else
							{
								Vec2i res = spriteRendererComponent["Tile Resolution"].as<Vec2i>();
								tm = CreateRef<TileMap>(VFS::AbsolutePath(path), res.X, res.Y);
								deserializeTileMaps[path] = tm;
							}
							src.TilePos = spriteRendererComponent["Tile Pos"].as<Vec2i>();
							src.TileSize = spriteRendererComponent["Tile Size"].as<Vec2i>();
							src.ParentTileMap = tm;

							src.Texture = tm->GetTileAt(src.TilePos.X, src.TilePos.Y, src.TileSize.X, src.TileSize.Y);
						}
						else
						{
							src.Texture = Texture2D::Create(VFS::AbsolutePath(path));
						}
						
					}
				}
			}
		}

		auto rootEntityComponent = entity["RootEntityComponent"];
		if (rootEntityComponent)
		{
			deserializedEntity.AddComponent<RootEntityComponent>();
		}
		
		auto parentEntityComponent = entity["ParentEntityComponent"];
		if (parentEntityComponent)
		{
			deserializedEntity.AddComponent<ParentEntityComponent>();

			auto children = parentEntityComponent["Children"];
			for (auto child : children)
			{
				Entity newChild = DeserializeEntity(child);
				deserializedEntity.GetComponent<ParentEntityComponent>().children.push_back(newChild);
			}
		}

		return deserializedEntity;
	}

    
	bool SceneSerializer::DeserializeBin(std::string path)
	{
		return false;
	}
}