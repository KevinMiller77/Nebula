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

		Entity GetSelection() { return SelectionContext; }

		inline bool HasSelection()	  { if(SelectionContext != Entity()) { return SelectionContext.IsValid(); } return false; }
		inline void RemoveSelection() { if (HasSelection()) { Context->RemoveEntity(SelectionContext); } SelectionContext = {}; } 
		inline void ClearSelection() { SelectionContext = {}; }

	private:
		void DrawVec2iControl(std::string label, Vec2i& data, int min1, int max1, int min2, int max2, float extraPadding = 0.0f);
		void DrawVec2fControl(std::string label, Vec2f& data, float min1, float max1, float min2, float max2, float extraPadding = 0.0f);
		void DrawVec3Control(std::string label, Vec3f& data, float extraPadding = 0.0f);

		//Returns true if should delete entity
		bool DrawEntityNode(Entity entity);
		void DrawTileMapChooser(Entity entity);
	
		void DrawComponents(Entity entity);
	private:

		Ref<Scene> Context = nullptr;
		Entity SelectionContext = Entity();
		
		std::string newTextureName; 
		std::string newTexturePath;

		// TileMap selection info
		bool tileMapChoserActive = false;
		std::string newTileMapPath;
		Vec2f alertWindowSize = {700.0f, 720.0f};
		Vec2i tileRes = {16, 16}, tilePos = {0, 0}, tileSize = {1, 1};
		Vec2i tileResMod = {16, 16}, tilePosMod = {0, 0}, tileSizeMod = {1, 1};
		bool mouseDraggingOnInteractiveTile = false;
		Vec2i beginDragIT = {0, 0}, endDragIT = {0, 0};

		Ref<Texture2D> tilemapBase = nullptr;
		Ref<TileMap> tileMap = nullptr;
		Ref<Texture2D> tilePreview = nullptr;

		TextureLibrary* TextureLib = nullptr;
	};

}
