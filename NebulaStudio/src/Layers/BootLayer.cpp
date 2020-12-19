#include "BootLayer.h"
#include "imgui_internal.h"

#include "../StudioApp.h"
namespace Nebula
{
    void BootLayer::OnAttach() 
    {
        memset(projLocation, 0, 256);
        std::string lastUsed = GetMostRecentProject();

        if (lastUsed.empty())
        {
            return;
        }

        hasRecents = true;
        State = PROJ_READY_OPEN;
        memcpy(projLocation, lastUsed.data(), lastUsed.size());
    }

    void BootLayer::OnUpdate(float ts) 
    {
        std::string dirToMakeFileIn = std::string(projLocation);
        if (dirToMakeFileIn == "Cancel")
        {
            return;
        }
        std::string fileName = std::string(projLocation);

        dirToMakeFileIn = dirToMakeFileIn.substr(0, dirToMakeFileIn.find_last_of("/") + 1);
        fileName = fileName.substr(fileName.find_last_of("/") + 1);
        

        if (!VFS::Exists(dirToMakeFileIn, true) || !EndsWith(projLocation, ".nsp"))
        {
            State = GREET;
        }
    }

    void Neb_PushButtonStyle()
    {
        ImGui::PushStyleColor(ImGuiCol_Button, {0, 0, 0, 0});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.3, 0.3, 0.3, 0.5});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0, 0, 0, 0});
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 1, 1 });
    }

    void Neb_PopButtonStyle()
    {
        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar();
    }

    void BootLayer::DrawProjSelection()
    {

        const float itemSpacing = ImGui::GetStyle().ItemSpacing.x;

        ImGui::Begin("Project Selection");

        // Put exit button in the top right
        ImGui::Dummy({ ImGui::GetContentRegionAvailWidth() - cancelButtonWidth - ImGui::GetStyle().ItemSpacing.x, 0.0f }); ImGui::SameLine();

        Neb_PushButtonStyle();
        if (ImGui::ImageButton((void*)Tex_Cancel->GetRendererID(), {32, 32}))
        {
            State = DONE;
            std::string n = "Cancel";
            memset(projLocation, 0, 256);
            memcpy(projLocation, n.data(), n.size());
        }
        Neb_PopButtonStyle();
        cancelButtonWidth = ImGui::GetItemRectSize().x;

        //Set cursor for text 
        float x_pos = (ImGui::GetWindowWidth() / 2.0f) - (greetingTextWidth / 2.0f);
        float y_pos = ImGui::GetWindowHeight() / 4.0f;
        ImGui::SetCursorPosX(x_pos);
        ImGui::SetCursorPosY(y_pos);

        //Text
        ImGui::Text("  Please open an existing  ");
        ImGui::SetCursorPosX(x_pos);
        ImGui::SetCursorPosY(y_pos + ImGui::GetItemRectSize().y + itemSpacing);

        ImGui::Text("project or create a new one");
        greetingTextWidth = ImGui::GetItemRectSize().x;

        //Set Cursor for buttons
        x_pos = (ImGui::GetWindowWidth() / 2.0f) - (newProjButtonWidth + itemSpacing * 2);
        y_pos = 2.0f * ImGui::GetWindowHeight() / 3.2f;
        ImGui::SetCursorPosY(y_pos);

        if (State != GREET)
        {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        }

        ImGui::PushItemWidth(-1);
        ImGui::InputTextWithHint("", "Project directory...", projLocation, 256);
        ImGui::Text("");
        ImGui::PopItemWidth();

        if (State != GREET)
        {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, false);
            ImGui::PopStyleVar();
        }

        ImGui::SetCursorPosX(x_pos);
        if (ImGui::Button("Open Project..."))
        {
            std::string chosenFile = FileDialogs::OpenFile("nsp");
            if (!chosenFile.empty())
            {
                if(VFS::IsFile(chosenFile, true))
                {
                    memset(projLocation, 0, 256);
                    memcpy(projLocation, chosenFile.c_str(), chosenFile.size() > 256 ? 256 : chosenFile.size());
                    State = PROJ_READY_OPEN;

                    SaveEditorConfig();
                }
            }
        }
        newProjButtonWidth = ImGui::GetItemRectSize().x;

        ImGui::SameLine();

        
        //Set Cursor for buttons
        x_pos = (ImGui::GetWindowWidth() / 2.0f) + (itemSpacing * 2);
        ImGui::SetCursorPosX(x_pos);

        if (ImGui::Button("New  Project..."))
        {
            std::string chosenFile = FileDialogs::SaveFile("nsp");
            if (!chosenFile.empty())
            {
                if(!EndsWith(chosenFile, ".nsp"))
                {
                    chosenFile += ".nsp";
                }
                memset(projLocation, 0, 256);
                memcpy(projLocation, chosenFile.c_str(), chosenFile.size() > 256 ? 256 : chosenFile.size());
                State = PROJ_READY_NEW;
            }
        }
        openProjButtonWidth = ImGui::GetItemRectSize().x;
        float openProjButtonHeight = ImGui::GetItemRectSize().y;

        if (State == GREET)
        {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        }

        ImGui::SameLine();
        float x_off = ImGui::GetContentRegionAvailWidth() - submitButtonWidth - ImGui::GetStyle().ItemSpacing.x;
        x_pos += openProjButtonWidth + itemSpacing * 2 + x_off;
        y_pos = ImGui::GetCursorPosY() + (openProjButtonHeight / 2.0f) - 18;
        ImGui::SetCursorPos( { x_pos, y_pos } );


        Neb_PushButtonStyle();
        if (ImGui::ImageButton((void*)Tex_Continue->GetRendererID(), {48, 36}))
        {
            State = DONE;
        }
        submitButtonWidth =  ImGui::GetItemRectSize().x;
        Neb_PopButtonStyle();

        if (State == GREET)
        {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, false);
            ImGui::PopStyleVar();
        }

        ImGui::End();
    }


    void BootLayer::OnImGuiRender() 
    {
        DrawProjSelection();
    }

    void BootLayer::OnEvent(Event& e) 
    {

    }
}