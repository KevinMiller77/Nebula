#include "Entity.h"
#include "Scene.h"

namespace Nebula
{
    Entity::Entity(entt::entity handle, Nebula::Scene* scene)
		: EntityHandle(handle), ParentScene(scene) {}


	bool Entity::IsValid()
	{
		if (!ParentScene) return false;

		return ParentScene->Registry.valid(EntityHandle);
	}

}