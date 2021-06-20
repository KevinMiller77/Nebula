#include "SceneHierarchyPanel.h"
#include "../StudioApp.h"

#include <imgui.h>
#include <Scene/Components.h>


namespace Nebula {

	SceneHierarchyPanel::SceneHierarchyPanel(Ref<Scene> context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(Ref<Scene> context)
	{
		Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		auto view = Context->Registry.view<RootEntityComponent>();

		for (auto entityHandle : view)
		{
			Entity e = { entityHandle, Context.get() };

			if (!e.GetComponent<RootEntityComponent>().VisibleOutsideRenderer)
			{
				continue;
			}

			ImGui::PushID(e.GetComponent<TagComponent>().UUID);
			if(DrawEntityNode(e))
			{
				RemoveSelection();
			}
			ImGui::PopID();
		}

		if (ImGui::BeginPopupContextWindow("##entity_context_add", 1, false))
		{
			if (ImGui::MenuItem("Add Entity"))
			{
				Context->CreateEntity("Blank Entity").AddComponent<RootEntityComponent>();
			}

			ImGui::EndPopup();
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			SelectionContext = {};

		ImGui::End();

		if (SelectionContext)
		{
			ImGui::Begin("Properties");
			DrawComponents(SelectionContext);

			ImGui::End();
		}
	}
	void SceneHierarchyPanel::DrawVec2iControl(std::string label, Vec2i& data, int min1, int max1, int min2, int max2, float extraPadding)
	{
		float width = ImGui::GetWindowContentRegionWidth();
		float eachSliderWidth = (width / 2.5f) / 2.0f + 0.01f * width - extraPadding;

		ImGui::PushID(&data);
		std::string labelOut = label + ":"; 
		std::string placeholderStr = std::string(labelOut.size(), ' ');
		
		ImGui::Text("%s", labelOut.c_str()); 		ImGui::SameLine();
		{
			
			ImGui::PushID(&data.X);		
			ImGui::PushItemWidth(eachSliderWidth);

			ImGui::BeginGroup();
			ImGui::Button("X");						ImGui::SameLine();
			ImGui::DragInt("", &data.X, 0.2f, min1, max1);	
			ImGui::EndGroup();						ImGui::SameLine();

			ImGui::PopItemWidth();
			ImGui::PopID();
		}
		{
			ImGui::PushID(&data.Y);
			ImGui::PushItemWidth(eachSliderWidth);

			ImGui::BeginGroup();
			ImGui::Button("Y");						ImGui::SameLine();
			ImGui::DragInt("", &data.Y, 0.2f, min2, max2);	
			ImGui::EndGroup();

			ImGui::PopItemWidth();
			ImGui::PopID();
		}

		ImGui::PopID();
	}

	void SceneHierarchyPanel::DrawVec2fControl(std::string label, Vec2f& data, float min1, float max1, float min2, float max2, float extraPadding)
	{
		float width = ImGui::GetWindowContentRegionWidth();
		float eachSliderWidth = (width / 2.5f) / 2.0f + 0.01f * width - extraPadding;

		ImGui::PushID(&data);
		std::string labelOut = label + ":"; 
		std::string placeholderStr = std::string(labelOut.size(), ' ');
		
		ImGui::Text("%s", labelOut.c_str()); 		ImGui::SameLine();
		{
			
			ImGui::PushID(&data.X);		
			ImGui::PushItemWidth(eachSliderWidth);

			ImGui::BeginGroup();
			ImGui::Button("X");						ImGui::SameLine();
			ImGui::DragFloat("", &data.X, 0.2f, min1, max1);	
			ImGui::EndGroup();						ImGui::SameLine();

			ImGui::PopItemWidth();
			ImGui::PopID();
		}
		{
			ImGui::PushID(&data.Y);
			ImGui::PushItemWidth(eachSliderWidth);

			ImGui::BeginGroup();
			ImGui::Button("Y");						ImGui::SameLine();
			ImGui::DragFloat("", &data.Y, 0.2f, min2, max2);	
			ImGui::EndGroup();

			ImGui::PopItemWidth();
			ImGui::PopID();
		}

		ImGui::PopID();
	}

	void SceneHierarchyPanel::DrawVec3Control(std::string label, Vec3f& data, float extraPadding)
	{
		float width = ImGui::GetWindowContentRegionWidth();
		float eachSliderWidth = (width / 2.5f) / 3.0f + 0.01f * width - extraPadding;

		ImGui::PushID(&data);
		std::string labelOut = label + ":"; 
		std::string placeholderStr = std::string(labelOut.size(), ' ');
		
		ImGui::Text("%s", labelOut.c_str()); 		ImGui::SameLine();
		{
			
			ImGui::PushID(&data.X);		
			ImGui::PushItemWidth(eachSliderWidth);

			ImGui::BeginGroup();
			ImGui::Button("X");						ImGui::SameLine();
			ImGui::DragFloat("", &data.X, 0.2f);	
			ImGui::EndGroup();						ImGui::SameLine();

			ImGui::PopItemWidth();
			ImGui::PopID();
		}
		{
			ImGui::PushID(&data.Y);
			ImGui::PushItemWidth(eachSliderWidth);

			ImGui::BeginGroup();
			ImGui::Button("Y");						ImGui::SameLine();
			ImGui::DragFloat("", &data.Y, 0.2f);	
			ImGui::EndGroup();						ImGui::SameLine();

			ImGui::PopItemWidth();
			ImGui::PopID();
		}
		{
			ImGui::PushID(&data.Z);
			ImGui::PushItemWidth(eachSliderWidth);

			ImGui::BeginGroup();
			ImGui::Button("Z");						ImGui::SameLine();
			ImGui::DragFloat("", &data.Z, 0.2f);
			ImGui::EndGroup();						

			ImGui::PopItemWidth();
			ImGui::PopID();
		}

		ImGui::PopID();
	}


	bool SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		
		ImGuiTreeNodeFlags flags = ((SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

		bool deleteEntity = false;

		bool opened = ImGui::TreeNodeEx((void*)entity.GetComponent<TagComponent>().UUID, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			SelectionContext = entity;
		}
		if (ImGui::IsItemClicked(1))
		{
			SelectionContext = entity;
		}
		if (ImGui::BeginPopupContextWindow(tag.c_str(), 1, true))
		{
			if (ImGui::MenuItem("Add Child"))
			{
				if (!SelectionContext.HasComponent<ParentEntityComponent>())
				{
					SelectionContext.AddComponent<ParentEntityComponent>();
				}
				SelectionContext.GetComponent<ParentEntityComponent>().children.push_back(Context->CreateEntity("Child"));
			}
			if (ImGui::MenuItem("Remove Entity"))
			{
				deleteEntity = true;
			}

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			if (entity.HasComponent<ParentEntityComponent>())
			{
				std::vector<Entity> deleteQueue;
				auto children = entity.GetComponent<ParentEntityComponent>().children;
				ImGui::Indent(0.3f);
				for (auto child : children)
				{
					if (!child.IsValid()) continue;

					ImGui::PushID(child.GetComponent<TagComponent>().UUID);
					if (DrawEntityNode(child))
					{
						deleteEntity = true;
					}
					ImGui::PopID();
				}
				ImGui::Unindent(0.3f);
			}
			ImGui::TreePop();
		}

		return deleteEntity;
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		#ifdef NEB_PLATFORM_WINDOWS
		strcpy_s(buffer, sizeof(buffer), tag.c_str());
		#else
		strcpy(buffer, tag.c_str());
		#endif

		ImGui::PushID(&tag);
		ImGui::Text(" Tag	:");
		ImGui::SameLine();
		if (ImGui::InputText("", buffer, sizeof(buffer)))
		{
			tag = std::string(buffer);
		}
		
		ImGui::PopID();

		if (entity.HasComponent<TransformComponent>())
		{
			ImGui::Separator();
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.GetComponent<TransformComponent>();

				ImGui::PushItemWidth(-1);
				DrawVec3Control("Position", transform.Translation);
				
				Vec3f rot;
				rot = transform.Rotation / Vec3f(PI2R_FACTOR, PI2R_FACTOR, PI2R_FACTOR);
				DrawVec3Control("Rotation", rot);
				transform.Rotation = rot * Vec3f(PI2R_FACTOR, PI2R_FACTOR, PI2R_FACTOR);

				DrawVec3Control("Scale   ", transform.Scale);

				ImGui::SameLine(); 
				ImGui::PushID(&transform.InheritScale);
				ImGui::Checkbox("", &transform.InheritScale);
				ImGui::PopID();

				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Text("Inherit Scale From Parent");
					ImGui::EndTooltip();
				}
				
				ImGui::PopItemWidth();

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			ImGui::Separator();
			if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Info"))
			{
				bool deleteSpriteComp = false;

				auto& spriteInfo = entity.GetComponent<SpriteRendererComponent>();

				ImGui::SameLine(ImGui::GetWindowWidth()-30);
				if (ImGui::Button("..."))
				{
					ImGui::OpenPopup("edit_spr_comp");
				}

				if (ImGui::BeginPopup("edit_spr_comp"))
				{	
					if (ImGui::Button("Delete Component"))
					{
						deleteSpriteComp = true;
					}
					ImGui::EndPopup();
				}



				ImGui::Text("Hidden? : "); ImGui::SameLine(); 
				ImGui::PushID(&tag);
				ImGui::Checkbox("", &spriteInfo.Hidden);	
				ImGui::PopID();

				ImGui::Text("Color        :");
				ImGui::PushItemWidth(-1);
				ImGui::SameLine();
				ImGui::ColorEdit4("", spriteInfo.Color.elements);
				ImGui::PopItemWidth();

				// Unsafe if, be careful
				if (spriteInfo.IsTileMap) 
					{ ImGui::Text("Tilemap File :"); ImGui::SameLine(); }
				else 
					{ ImGui::Text("Texture      :"); ImGui::SameLine(); }

				std::string currentPath = std::string();
				if (spriteInfo.Texture.get()) currentPath = VFS::Path(spriteInfo.Texture->GetPath());
				ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth()  * 0.5f);
				ImGui::InputText("", currentPath.data(), currentPath.size() + 1);

				ImGui::PopItemWidth();
				
				ImGui::SameLine();
				if (ImGui::Button(" Set... "))
				{
					ImGui::OpenPopup("texture_opts");
				}
				if (ImGui::BeginPopup("texture_opts"))
				{
					ImGui::PushItemWidth(-1);
					if (ImGui::Button("From Image..."))
					{
						std::string NewPath = FileDialogs::OpenFile("png");
						if (VFS::Exists(NewPath, true))
						{
							spriteInfo.IsTileMap = false;
							spriteInfo.Texture = Texture2D::Create(NewPath);
						}
					}
					if (ImGui::Button("From Tilemap... "))
					{
						ImGui::OpenPopup("Tilemap");
					}
					DrawTileMapChooser(entity);

					ImGui::EndPopup();
				}

				if (spriteInfo.IsTileMap)
				{

					int maxPosX = (spriteInfo.ParentTileMap->GetTexture()->GetWidth() /  spriteInfo.ParentTileMap->GetResolution().X ) - 1;
					int maxPosY = (spriteInfo.ParentTileMap->GetTexture()->GetHeight() /  spriteInfo.ParentTileMap->GetResolution().Y) - 1;

					DrawVec2iControl("Tile Position  ", spriteInfo.TilePos, 0,  maxPosX, 0, maxPosY);
					DrawVec2iControl("Tile Size      ", spriteInfo.TileSize, 1, NMax(maxPosX - spriteInfo.TilePos.X, 1), 1, NMax(maxPosY - spriteInfo.TilePos.Y, 1));
					spriteInfo.LoadSelectedTile();
					
					Ref<Texture2D> preview = spriteInfo.Texture;
					Vec2f sizeSmall = { min(2.0f * preview->GetWidth(), 64.0f), min(2.0f * preview->GetHeight(), 64.0f) };
					Vec2f sizeLarge = { min(8.0f * preview->GetWidth(), 256.0f), min(8.0f * preview->GetHeight(), 256.0f) };
					
					ImVec2 uv1 = { preview->GetTexCoords()[0].X, preview->GetTexCoords()[2].Y };
					ImVec2 uv2 = { preview->GetTexCoords()[2].X, preview->GetTexCoords()[0].Y };

					float x_pos = (ImGui::GetContentRegionAvail().x / 2.0f) - (sizeSmall.X / 2.0f);

					ImGui::Dummy({x_pos, 0.0f}); ImGui::SameLine();
					ImGui::Image((void*)preview->GetRendererID(), NebToImVec(sizeSmall), uv1, uv2);

					if (ImGui::IsItemHovered())
					{

						ImGui::BeginTooltip();
						ImGui::Image((void*)preview->GetRendererID(), NebToImVec(sizeLarge), uv1, uv2);
						ImGui::EndTooltip();
					}

					if (ImGui::IsItemClicked())
					{
						ImGui::OpenPopup("Tilemap");
					}
					DrawTileMapChooser(entity);
				}
					

				ImGui::TreePop();

				if(deleteSpriteComp)
				{
					entity.RemoveComponent<SpriteRendererComponent>();
				}
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			ImGui::Separator();

			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				bool deleteCamComp = false;

				auto& cameraComponent = entity.GetComponent<CameraComponent>();
				auto& camera = cameraComponent.Camera;

				ImGui::SameLine(ImGui::GetWindowWidth()-30);
				if (ImGui::Button("..."))
				{
					ImGui::OpenPopup("edit_cam_comp");
				}

				if (ImGui::BeginPopup("edit_cam_comp"))
				{
					if (ImGui::MenuItem("Delete Component"))
					{
						deleteCamComp = true;
					}
					if (ImGui::MenuItem("Make Main Camera"))
					{
						camera.WantsMainCamera(true);
					}
					ImGui::EndPopup();
				}

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float verticalFov = camera.GetPerspectiveVerticalFOV();
					if (ImGui::DragFloat("Vertical FOV", &verticalFov))
					{
						camera.SetPerspectiveVerticalFOV(verticalFov);
					}
					float perspNear = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &perspNear))
					{
						camera.SetPerspectiveNearClip(perspNear);
					}

					float perspFar = camera.GetPerspectiveFarClip();
					if(ImGui::DragFloat("Far", &perspFar))
					{					
						camera.SetPerspectiveFarClip(perspFar);
					}
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					float orthoNear = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near", &orthoNear))
						camera.SetOrthographicNearClip(orthoNear);

					float orthoFar = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far", &orthoFar))
						camera.SetOrthographicFarClip(orthoFar);

					bool fixedARChanged = false;
					if (ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio))
						fixedARChanged = true;

					bool trueARNotReflected = cameraComponent.FixedAspectRatio != cameraComponent.Camera.GetFixedAspectRatio(); 
					bool updateCameraFixedAspectRatio = fixedARChanged || trueARNotReflected;
					
					if (updateCameraFixedAspectRatio)
					{
						cameraComponent.Camera.SetFixedAspectRatio(cameraComponent.FixedAspectRatio);
					}
				}


				ImGui::TreePop();

				if (deleteCamComp)
				{
					if (Context->GetPrimaryCamera().GetID() == entity.GetID())
					{
						Context->SetPrimaryCamera( { entt::null, nullptr } );
					}

					entity.RemoveComponent<CameraComponent>();
				}
			}
		}

        if (entity.HasComponent<AudioSourceComponent>()) {
            ImGui::Separator();

            if (ImGui::TreeNodeEx((void*)typeid(AudioSourceComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Audio Source" )) {
                auto& audioSrc = entity.GetComponent<AudioSourceComponent>();

                bool deleteAudioSource = false;
                ImGui::SameLine(ImGui::GetWindowWidth()-30);
				if (ImGui::Button("..."))
				{
					ImGui::OpenPopup("edit_cam_comp");
				}

				if (ImGui::BeginPopup("edit_cam_comp"))
				{
					if (ImGui::MenuItem("Delete Component"))
					{
                        deleteAudioSource = true;
    				}
					ImGui::EndPopup();
				}
                

                ImGui::Text("Audio File : "); ImGui::SameLine();

                
                bool hasAudioFile = audioSrc.Source->IsLoaded() && audioSrc.Source->GetFilePath() != "";
                bool currentlyLoading = audioSrc.Source->GetFilePath() != "" && !audioSrc.Source->IsLoaded();

				std::string currentPath = VFS::Path(audioSrc.Source->GetFilePath());
				ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth()  * 0.5f);
				ImGui::InputText("", currentPath.data(), currentPath.size() + 1);

                ImGui::SameLine();
                if (ImGui::Button(" Set... "))
				{
                    std::string NewPath = FileDialogs::OpenFile("ogg,mp3");
                    if (VFS::Exists(NewPath, true))
                    {
                        audioSrc.Source->Invalidate();
                        audioSrc.Source = AudioSource::LoadFromFile(NewPath, audioSrc.Source->GetSpatial());
                    }
				}

                if (currentlyLoading) {
                    ImGui::NewLine(); 
                    ImGui::Text("    AUDIO FILE LOADING...   ");
                    ImGui::NewLine();
                }

                if (hasAudioFile) {
                    std::pair<uint32_t, uint32_t> duration = audioSrc.Source->GetLengthMinutesAndSeconds();
                    ImGui::Text("Duration   : %d:%02d", duration.first, duration.second);
                    ImGui::NewLine();

                    bool modifyingB = audioSrc.Source->GetLoop();
                    ImGui::Text("Loop?      : "); ImGui::SameLine();
                    ImGui::PushID(duration.first + 0);
                    if(ImGui::Checkbox("", &modifyingB)) {
                        audioSrc.Source->SetLoop(modifyingB);
                    }
                    ImGui::PopID();
                    
                    modifyingB = audioSrc.Source->GetSpatial();
                    ImGui::PushID(duration.first + 1);
                    ImGui::Text("Spatial?   : "); ImGui::SameLine();
                    if(ImGui::Checkbox("", &modifyingB)) {
                        audioSrc.Source->SetSpatial(modifyingB);
                    }
                    ImGui::PopID();
                    

                    float modifyingF = audioSrc.Source->GetGain();
                    ImGui::Text("Gain       : "); ImGui::SameLine();
                    ImGui::PushID(duration.first + 2);
                    if(ImGui::DragFloat("", &modifyingF, 0.005f, 0.0f, 1.0f)) {
                        audioSrc.Source->SetGain(modifyingF);
                    }
                    ImGui::PopID();


                    modifyingF = audioSrc.Source->GetPitch();
                    ImGui::Text("Pitch      : "); ImGui::SameLine();
                    ImGui::PushID(duration.first + 3);
                    if(ImGui::DragFloat("", &modifyingF, 0.02f, 0.0f, 10.0f)) {
                        audioSrc.Source->SetPitch(modifyingF);
                    }
                    ImGui::PopID();
                }
                
                ImGui::TreePop();

                if (deleteAudioSource) {
                    audioSrc.Source->Invalidate();        
                    entity.RemoveComponent<AudioSourceComponent>();
                }

            }
        }

        if (entity.HasComponent<AudioListenerComponent>()) {
            ImGui::Separator();

            if (ImGui::TreeNodeEx((void*)typeid(AudioListenerComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Audio Listener")) {
                auto& audioListener = entity.GetComponent<AudioListenerComponent>();
                bool deleteAudioListener = false;

                ImGui::Text("Active Listener? :"); ImGui::SameLine();
                if(ImGui::Checkbox(" ", &audioListener.IsActiveListener)) {
                    auto& view = Context->GetView<AudioListenerComponent>();
                    for (auto entityID : view) {
                        if (entity == entityID) {
                            continue;
                        }
                        auto& e = view.get(entityID);
                        e.IsActiveListener = false;
                    }
                }

                ImGui::SameLine(ImGui::GetWindowWidth()-30);
				if (ImGui::Button("..."))
				{
					ImGui::OpenPopup("edit_cam_comp");
				}

				if (ImGui::BeginPopup("edit_cam_comp"))
				{
					if (ImGui::MenuItem("Delete Entity"))
					{
                        deleteAudioListener = true;
    				}
					ImGui::EndPopup();
				}

                ImGui::TreePop();

                if (deleteAudioListener) {        
                    entity.RemoveComponent<AudioSourceComponent>();
                }
            }
        }


		ImGui::Separator();
		ImGui::SetCursorPosX(ImGui::GetColumnWidth() / 2 - 55);
		if(ImGui::Button("Add A Component"))
		{
			ImGui::OpenPopup("new_comp");
		}

		ImGui::SameLine();
		if(ImGui::BeginPopup("new_comp"))
		{
			if(ImGui::Button("Sprite Rendering Component"))
			{
				if (!entity.HasComponent<SpriteRendererComponent>())
				{
					entity.AddComponent<SpriteRendererComponent>();
				}
			}
			if(ImGui::Button("Camera Component"))
			{	
				if(!entity.HasComponent<CameraComponent>())
				{
					entity.AddComponent<CameraComponent>(Context->GetViewportSize());
				}
			}
            if(ImGui::Button("Audio Source Component"))
			{	
				if(!entity.HasComponent<AudioSourceComponent>())
				{
					entity.AddComponent<AudioSourceComponent>();
				}
			}
            if(ImGui::Button("Audio Listener Component"))
			{	
				if(!entity.HasComponent<AudioListenerComponent>())
				{
					entity.AddComponent<AudioListenerComponent>();
				}
			}

			ImGui::EndPopup();
		}
	}

	void SceneHierarchyPanel::DrawTileMapChooser(Entity entity)
	{
		auto& spriteInfo = entity.GetComponent<SpriteRendererComponent>();
		
		bool open = true;
		if (ImGui::IsPopupOpen("Tilemap")) ImGui::SetNextWindowSize( { alertWindowSize.X, alertWindowSize.Y } );
		else 
		{
			newTileMapPath = std::string();
			alertWindowSize = {700.0f, 720.0f};
			tileRes = {16, 16}, tilePos = {0, 0}, tileSize = {1, 1};
			tilemapBase = nullptr;
			tileMap = nullptr;
			tilePreview = nullptr;
		}
		if (ImGui::BeginPopupModal("Tilemap", &open ))
		{
			if (spriteInfo.IsTileMap && !tileMapChoserActive)
			{
				tileMapChoserActive = true;

				tileMap = spriteInfo.ParentTileMap;
				tilemapBase = tileMap->GetTexture();
				tilePreview = spriteInfo.Texture;
				
				newTileMapPath = tilemapBase->GetPath();

				tileRes = tileMap->GetResolution();
				tilePos = spriteInfo.TilePos;
				tileSize = spriteInfo.TileSize;
			}

			if (ImGui::Button("Select File..."))
			{
				std::string NewPath = FileDialogs::OpenFile("png");
				if (VFS::Exists(NewPath, true))
				{
					newTileMapPath = NewPath;
					tilemapBase = Texture2D::Create(newTileMapPath);
				}
			} ImGui::SameLine();

			ImGui::Text("Currently Selected :"); ImGui::SameLine();
			ImGui::InputText("", newTileMapPath.data(), newTileMapPath.size() + 1);
			ImGui::Dummy(ImVec2(0.0f, 20.0f));

			if (!newTileMapPath.empty() && tilemapBase->IsValid())
			{	
				DrawVec2iControl("Tile Resolution", tileRes, 1, tilemapBase->GetWidth(), 1, tilemapBase->GetHeight());


				int maxTileX = (tilemapBase->GetWidth() / tileRes.X) - 1;
				int maxTileY = (tilemapBase->GetHeight() / tileRes.Y) - 1;
				DrawVec2iControl("Tile Position  ", tilePos, 0, maxTileX, 0, maxTileY);
				DrawVec2iControl("Tile Size      ", tileSize, 1, maxTileX , 1, maxTileY);
				ImGui::Dummy( { 0.0f, 20.0f });

				if (!tileMap || (tileRes != tileResMod))
				{
					tileResMod = tileRes;
					tileMap = CreateRef<TileMap>(tilemapBase, tileRes.X, tileRes.Y);
					tilePreview = nullptr;
				}

				bool valChanged = (tilePos != tilePosMod) || (tileSize != tileSizeMod);
				if (!tilePreview || valChanged)
				{
					tilePosMod = tilePos;
					tileSizeMod = tileSize;

					tilePreview = tileMap->GetTileAt(tilePos.X, tilePos.Y, tileSize.X, tileSize.Y);
				}

				bool tileMapReady = tileMap && tileMap->GetTexture()->IsValid();
				bool tilePreviewReady = (bool)tilePreview;
				bool openingPicker = false;
				bool selected = false;

				if (tileMapReady)
				{
					ImGui::Text("Select from tilemap: "); 
					if (tilePreviewReady)
					{
						ImGui::SameLine(); ImGui::Dummy({12, 0}); ImGui::SameLine();
						ImGui::Text("Preview:");
						if(ImGui::Button("Confirm selection"))
							selected = true;
					}

					float textSize = ImGui::CalcTextSize("Select from tilemap: ").x;
					if(ImGui::ImageButton((void*) tileMap->GetTexture()->GetRendererID(), { textSize, textSize }, { 0, 1 }, { 1, 0 }))
					{
						ImGui::OpenPopup("Interactive Picker");
						openingPicker = true;
					}
				}							

				if (tilePreviewReady)
				{
					ImVec2 uv1 = { tilePreview->GetTexCoords()[0].X, tilePreview->GetTexCoords()[2].Y };
					ImVec2 uv2 = { tilePreview->GetTexCoords()[2].X, tilePreview->GetTexCoords()[0].Y };
				
					ImGui::SameLine();
					ImGui::Image((ImTextureID)tilePreview->GetRendererID(), { (float)(128 * tileSize.X), (float)(128 * tileSize.Y) }, uv1, uv2);
				
					if(!openingPicker && selected)
					{
						spriteInfo.IsTileMap = true;
						spriteInfo.ParentTileMap = tileMap;
						spriteInfo.TilePos = tilePos;
						spriteInfo.TileSize = tileSize;
						// spriteInfo.Texture = tilePreview;
						spriteInfo.LoadSelectedTile();

						newTileMapPath = std::string();
						alertWindowSize = {700.0f, 720.0f};
						tileRes = {16, 16}, tilePos = {0, 0}, tileSize = {1, 1};
						tilemapBase = nullptr;
						tileMap = nullptr;
						tilePreview = nullptr;

						ImGui::CloseCurrentPopup();
						tileMapChoserActive = false;
					}
				}

				bool someoneIsActive = false;
				bool someoneIsHovered = false;
				bool open2 = true;
				if (ImGui::BeginPopup("Interactive Picker"))
				{
					ImGui::Text("Please select the tile(s) you would like to use. The preview in the other window is still active."); 
				
					float w = tileMap->GetTexture()->GetWidth();
					float h = tileMap->GetTexture()->GetHeight();

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(-4, -2));
					// ImGui::PushStyleColor(ImGuiCol_Button, {0, 0, 0, 0});
					// ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0, 0, 0, 0});
					// ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0, 0, 0, 0});
					
					//TODO: Make an interactive picker
					for (int y = maxTileY * tileRes.Y ; y >= 0 ; y -= tileRes.Y)
					{
						for (int x = 0; x <= maxTileX * tileRes.X; x += tileRes.X)
						{
							// Mat42f out = { topL, topR, botR, botL};
							ImVec2 uv1 = { x / w, ( y + tileRes.Y ) / h };
							ImVec2 uv2 = { ( x + tileRes.X) / w, y / h };

							ImGui::PushID(x + y * 500);
							
							bool mouseDown = Input::IsMouseButtonPressed(MouseCode::ButtonLeft);

							ImGui::ImageButton((void*)tileMap->GetTexture()->GetRendererID(), {32, 32}, uv1, uv2, 0, {1, 1, 1, 0.3f});


							if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
							{
								if (mouseDraggingOnInteractiveTile)
								{
									// If we're dragging and hovering over this tile, it is our new end point
									endDragIT = { x / tileRes.X, y / tileRes.Y};

									//Find lowest corner
									int posx = beginDragIT.X < endDragIT.X ? beginDragIT.X : endDragIT.X; 
									int posy = beginDragIT.Y < endDragIT.Y ? beginDragIT.Y : endDragIT.Y;
									int sizex = abs(endDragIT.X - beginDragIT.X) + 1;
									int sizey = abs(endDragIT.Y - beginDragIT.Y) + 1;

									LOG_INF("Size of current selection: %d, %d\n", sizex, sizey);

									tilePos = {posx, posy};
									tileSize = {sizex, sizey};

									tilePreview = tileMap->GetTileAt(posx, posy, sizex, sizey);
								}
							}
							
							if(ImGui::IsItemActive())
							{
								someoneIsActive = true;
								if(ImGui::IsItemClicked(0))
								{
									tilePos = {x / tileRes.X, y / tileRes.Y};
									tileSize = {1, 1};
									tilePreview = tileMap->GetTileAt(tilePos.X, tilePos.Y, 1, 1);
									LOG_INF("One tile was selected\n");	
								}

								if(ImGui::IsMouseDragging(0))
								{
									if (!mouseDraggingOnInteractiveTile)
									{
										beginDragIT = { x / tileRes.X, y / tileRes.Y };
										mouseDraggingOnInteractiveTile = true;
										// LOG_INF("Dragging begun %d, %d\n", beginDragIT.X, beginDragIT.Y);
									}
								}
								else
								{
									if (mouseDraggingOnInteractiveTile)
									{
										mouseDraggingOnInteractiveTile = false;

										LOG_TMI("Multiple tiles were selected.\n");
										LOG_TMI("Begin drag: %d, %d\n", beginDragIT.X, beginDragIT.Y);
										LOG_TMI("End drag: %d, %d\n", 	endDragIT.X, endDragIT.Y);
										
										LOG_INF("Dragging ceased\n");
									}
								}
								
							}
							ImGui::PopID();
							ImGui::SameLine();
						}

						ImGui::Dummy({0.0f, 0.0f});
					}
					// If not a single button was active, we aren't dragging anymore
					if (!someoneIsActive)
					{
						LOG_TMI("Someone was active\n");
						mouseDraggingOnInteractiveTile = false;
					}


					ImGui::PopStyleVar(); //ImGui::PopStyleColor(3);
					ImGui::EndPopup();
				}


			}

			alertWindowSize = { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };
			ImGui::EndPopup();
		}
	}

}
