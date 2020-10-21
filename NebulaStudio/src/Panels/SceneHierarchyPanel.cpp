#include "SceneHierarchyPanel.h"

#include <imgui.h>

#include <Scene/Components.h>

namespace Nebula {

	SceneHierarchyPanel::SceneHierarchyPanel(Scene* context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(Scene* context)
	{
		Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		Context->Registry.each([&](auto entityID)
		{
			Entity entity{ entityID , Context };
			DrawEntityNode(entity);
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			SelectionContext = {};

		ImGui::End();

		ImGui::Begin("Properties");
		if (SelectionContext)
			DrawComponents(SelectionContext);

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawVec3Control(std::string label, Vec3f& data)
	{
		ImGui::PushID(&data);
		std::string labelOut = label + ":"; 
		std::string placeholderStr = std::string(labelOut.size(), ' ');
		
		ImGui::Text("%s", labelOut.c_str()); 		ImGui::SameLine();
		{

			ImGui::PushID(&data.x);		
			ImGui::PushItemWidth(50);

			ImGui::Button("X");						ImGui::SameLine();
			ImGui::DragFloat("", &data.x, 0.2f);	ImGui::SameLine();
			
			ImGui::PopItemWidth();
			ImGui::PopID();
		}
		{
			ImGui::PushID(&data.y);
			ImGui::PushItemWidth(50);

			ImGui::Button("Y");						ImGui::SameLine();
			ImGui::DragFloat("", &data.y, 0.2f);	ImGui::SameLine();

			ImGui::PopItemWidth();
			ImGui::PopID();
		}
		{
			ImGui::PushID(&data.z);
			ImGui::PushItemWidth(50);

			ImGui::Button("Z");						ImGui::SameLine();
			ImGui::DragFloat("", &data.z, 0.2f);

			ImGui::PopItemWidth();
			ImGui::PopID();
		}

		ImGui::PopID();
	}


	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		
		ImGuiTreeNodeFlags flags = ((SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			SelectionContext = entity;
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			
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

				DrawVec3Control("Position", transform.Translation);
				
				Vec3f rot;
				rot = transform.Rotation / Vec3f(PI2R_FACTOR, PI2R_FACTOR, PI2R_FACTOR);
				DrawVec3Control("Rotation", rot);
				transform.Rotation = rot * Vec3f(PI2R_FACTOR, PI2R_FACTOR, PI2R_FACTOR);

				DrawVec3Control("Scale   ", transform.Scale);

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			ImGui::Separator();
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& cameraComponent = entity.GetComponent<CameraComponent>();
				auto& camera = cameraComponent.Camera;

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
			}
		}
	}

}
