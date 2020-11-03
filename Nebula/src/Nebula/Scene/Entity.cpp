#include "Entity.h"
#include "Scene.h"

namespace Nebula
{
    Entity::Entity(entt::entity handle, Nebula::Scene* scene)
		: EntityHandle(handle), ParentScene(scene)
	{
	}

	bool Entity::IsValid()
	{
		return ParentScene->Registry.valid(EntityHandle);
	}

}