#pragma once

#include <Nebula_pch.h>
#include "Scene.h"
#include <entt/entt.hpp>

namespace Nebula
{
    class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			if(HasComponent<T>())
            {
				#ifdef NEB_PLATFORM_WINDOWS
             	LOG_ERR("Entity already has component!");
				#endif
            }
			return CurScene->Registry.emplace<T>(EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			if(!HasComponent<T>())
            {
				#ifdef NEB_PLATFORM_WINDOWS
				LOG_ERR("Entity does not have component for grabbing!");
				#endif
            }
			return CurScene->Registry.get<T>(EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return CurScene->Registry.has<T>(EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			if(!HasComponent<T>())
            {
				#ifdef NEB_PLATFORM_WINDOWS
				LOG_ERR("Entity does not have component for removal!");
				#endif
            }
			CurScene->Registry.remove<T>(EntityHandle);
		}

		operator bool() const { return EntityHandle != entt::null; }
		operator uint32_t() const { return (uint32_t)EntityHandle; }

		bool operator==(const Entity& other) const
		{
			return EntityHandle == other.EntityHandle && CurScene == other.CurScene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}
	private:
		entt::entity EntityHandle{ entt::null };
		Scene* CurScene = nullptr;
	};
}