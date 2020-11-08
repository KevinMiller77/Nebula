#include "SceneHierarchyPanel.h"

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

		Context->Registry.each([&](auto entityID)
		{
			Entity entity{ entityID , Context.get() };
			if (entity.HasComponent<RootEntityComponent>())
			{
				ImGui::PushID(entity.GetComponent<TagComponent>().UUID);
				if(DrawEntityNode(entity))
				{
					Context->RemoveEntity(SelectionContext);
					SelectionContext = {};
				}
				ImGui::PopID();
			}
		});

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

	void SceneHierarchyPanel::DrawVec3Control(std::string label, Vec3f& data)
	{
		float width = ImGui::GetWindowContentRegionWidth();
		float eachSliderWidth = (width / 2.5f) / 3.0f + 0.01f * width;

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
		if (entity.HasComponent<TagComponent>())
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
		}

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

				ImGui::SameLine(ImGui::GetWindowWidth()-30);
				if (ImGui::Button("..."))
				{
					ImGui::OpenPopup("edit_spr_comp");
				}

				if (ImGui::BeginPopup("edit_spr_comp"))
				{
					if (ImGui::Button("Delete"))
					{
						deleteSpriteComp = true;
					}
					ImGui::EndPopup();
				}


				auto& spriteInfo = entity.GetComponent<SpriteRendererComponent>();

				ImGui::Text("Color   :");
				ImGui::PushItemWidth(-1);
				ImGui::SameLine();
				ImGui::ColorEdit4("", spriteInfo.Color.elements);
				ImGui::PopItemWidth();

				std::vector<std::string> texNames = TextureLib->GetAllNames();

				ImGui::Text("Texture :"); ImGui::SameLine();
				
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
					if (ImGui::Button("From File..."))
					{
						std::string NewPath = FileDialogs::OpenFile("png");
						if (VFS::Exists(NewPath, true))
						{
							spriteInfo.Texture = Texture2D::Create(NewPath);
						}
					}
					ImGui::PopItemWidth();

					ImGui::EndPopup();
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

					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);
				}


				ImGui::TreePop();

				if (deleteCamComp)
				{
					entity.RemoveComponent<CameraComponent>();
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

			ImGui::EndPopup();
		}


	}

}
