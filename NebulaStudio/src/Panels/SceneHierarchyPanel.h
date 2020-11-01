#pragma once

#include <Nebula.h>

namespace Nebula {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(Ref<Scene> scene);

		void SetContext(Ref<Scene> scene);
		void SetTextureLib(TextureLibrary* lib) { TextureLib = lib; }

		void OnImGuiRender();

		void ClearSelection() { SelectionContext = {}; }
	private:
		void DrawVec3Control(std::string label, Vec3f& data);

		//Returns true if should delete entity
		bool DrawEntityNode(Entity entity);
		
		void DrawComponents(Entity entity);
	private:
		Timer Autosave = Timer();

		Ref<Scene> Context = nullptr;;
		Entity SelectionContext;
		std::string newTextureName; 
		std::string newTexturePath;
		TextureLibrary* TextureLib = nullptr;
	};

}
