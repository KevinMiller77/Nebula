#pragma once

#include <Core/PlatformInfo.h>
#include <entt/entt.hpp>
#include <string>

namespace Nebula
{
	class Scene;

    class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		uint32_t GetID()
		{
			return (uint32_t)EntityHandle;
		}

		std::string GetStrID()
		{
			return std::to_string((uint32_t)EntityHandle);
		}

		bool IsValid();

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			if(HasComponent<T>())
            {
             LOG_ERR("Entity already has component!");
            }
			return Scene->Registry.emplace<T>(EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			if(!HasComponent<T>())
            {
             LOG_ERR("Entity does not have component for grabbing!");
            }
			return Scene->Registry.get<T>(EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			if (!Scene->Registry.valid(EntityHandle))
			{
				return false;
			}
			return Scene->Registry.has<T>(EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			if(!HasComponent<T>())
            {
             LOG_ERR("Entity does not have component for removal!");
            }
			Scene->Registry.remove<T>(EntityHandle);
		}
		

		operator bool() const { return EntityHandle != entt::null; }
		operator uint32_t() const { return (uint32_t)EntityHandle; }
		operator entt::entity() const { return EntityHandle; }

		bool operator==(const Entity& other) const
		{
			return EntityHandle == other.EntityHandle && Scene == other.Scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}
	private:
		entt::entity EntityHandle{ entt::null };
		Scene* Scene = nullptr;
	};
}