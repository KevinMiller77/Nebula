#include "Entity.h"
#include "Scene.h"

namespace Nebula
{
    Entity::Entity(entt::entity handle, Nebula::Scene* scene)
		: EntityHandle(handle), Scene(scene)
	{
	}

	bool Entity::IsValid()
	{
		return Scene->Registry.valid(EntityHandle);
	}

}