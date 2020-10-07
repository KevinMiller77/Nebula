#include "Entity.h"

namespace Nebula
{
    Entity::Entity(entt::entity handle, Nebula::Scene* scene)
		: EntityHandle(handle), CurScene(scene)
	{
	}

}