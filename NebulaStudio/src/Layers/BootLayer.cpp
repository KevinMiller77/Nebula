#include "BootLayer.h"
#include "imgui_internal.h"

namespace Nebula
{
    void BootLayer::OnAttach() 
    {
        memset(projLocation, 0, 256);
    }

    void BootLayer::OnUpdate(float ts) 
    {
        std::string dirToMakeFileIn = std::string(projLocation);
        std::string fileName = std::string(projLocation);

        #ifdef NEB_PLATFORM_WINDOWS
            dirToMakeFileIn = dirToMakeFileIn.substr(0, dirToMakeFileIn.find_last_of("\\") + 1);
            fileName = fileName.substr(fileName.find_last_of("\\") + 1);
        #else
            dirToMakeFileIn = dirToMakeFileIn.substr(0, dirToMakeFileIn.find_last_of("/") + 1);
            fileName = fileName.substr(fileName.find_last_of("/") + 1);
        #endif

        if (!VFS::Exists(dirToMakeFileIn, true) || !EndsWith(projLocation, ".nsp"))
        {
            State = GREET;
        }

        switch(State)
        {
            case(PROJ_READY_NEW):
            {
                
                break;
            }
            case(PROJ_READY_OPEN):
            {
                
                break;
            }
            default:
                break;
        }
    }

    void BootLayer::DrawProjSelection()
    {
        const float itemSpacing = ImGui::GetStyle().ItemSpacing.x;

        ImGui::Begin("Project Selection");

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
            std::string chosenFile = FileDialogs::OpenFile("Nebula Studio Project (*.nsp)\0*.nsp\0All File (*.*)\0*.*\0");
            if (!chosenFile.empty())
            {
                if(VFS::IsFile(chosenFile, true))
                {
                    memcpy(projLocation, chosenFile.c_str(), chosenFile.size() > 256 ? 256 : chosenFile.size());
                    State = PROJ_READY_OPEN;
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
            std::string chosenFile = FileDialogs::SaveFile("Nebula Studio Project (*.nsp)\0*.nsp\0All File (*.*)\0*.*\0");
            if (!chosenFile.empty())
            {
                if(!EndsWith(chosenFile, ".nsp"))
                {
                    chosenFile += ".nsp";
                }
                memcpy(projLocation, chosenFile.c_str(), chosenFile.size() > 256 ? 256 : chosenFile.size());
                State = PROJ_READY_NEW;
            }
        }
        openProjButtonWidth = ImGui::GetItemRectSize().x;

        if (State == GREET)
        {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        }

        x_pos = ImGui::GetWindowWidth() - (itemSpacing * 2 + submitButtonWidth);
        ImGui::SetCursorPosX(x_pos);
        if (ImGui::Button("  >  "))
        {
            State = DONE;
        }
        submitButtonWidth =  ImGui::GetItemRectSize().x;

        if (State == GREET)
        {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, false);
            ImGui::PopStyleVar();
        }

        ImGui::End();
    }

    void BootLayer::DrawNewProjScreen()
    {

    }


    void BootLayer::OnImGuiRender() 
    {
        DrawProjSelection();
    }

    void BootLayer::OnEvent(Event& e) 
    {

    }
}