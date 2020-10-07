#pragma once

#include "Entity.h"

namespace Nebula {

	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& GetComponent()
		{
			return EntityAttachedTo.GetComponent<T>();
		}
	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(float ts) {}
	private:
		Entity EntityAttachedTo;
		friend class Scene;
	};

}

