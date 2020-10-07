#pragma once

#include <Core/PlatformInfo.h>
#include <Utils/Logging.h>
#include <Scene/Scene.h>
#include <Scene/Entity.h>

namespace Nebula {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(Scene* scene);

		void SetContext(Scene* scene);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Scene* Context;
		Entity SelectionContext;
	};

}
