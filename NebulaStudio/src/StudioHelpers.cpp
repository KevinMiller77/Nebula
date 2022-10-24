#include "StudioApp.h"

#include <iostream>
#include <fstream>

#include <ext/yaml-cpp/include/yaml-cpp/yaml.h>


namespace Nebula
{
    EditorConfig s_EditorConfig = EditorConfig();

    
    void AddRecentProject(std::string projectPath)
    {
        std::vector<std::string>& rpl = s_EditorConfig.recentProjectLocations;

        for (int i = 0; i < rpl.size(); i++)
        {
            std::string s = rpl[i];
            if (s == projectPath)
            {
                // If this already exists, we remove it from it's current place
                // in the list and it will be added at the end below
                rpl.erase(rpl.begin() + i);
                break;
            } 
        }
        rpl.push_back(projectPath);
        SaveEditorConfig();
    }


    std::string GetMostRecentProject()
    {
        auto rpl = s_EditorConfig.recentProjectLocations;
        int rplSize = rpl.size(); 

        if (rplSize > 0)
        {
            return rpl[rplSize - 1];
        }
        return std::string();
    }
    
    std::vector<std::string> GetRecentProjects();

    void OpenEditorConfig()
    {
        EditorConfig out;

        auto in = std::ifstream(EDITOR_CONFIG_PATH);
        std::stringstream strStream;
        strStream << in.rdbuf();

        YAML::Node data = YAML::Load(strStream.str());

        out.installDirectory = data["Installed"].as<std::string>();
        out.recentProjectLocations = data["Recents"].as<std::vector<std::string>>();

        if(out.recentProjectLocations.size() == 1 && out.recentProjectLocations[0] == "Empty")
        {
            out.recentProjectLocations.clear();
        }

        for (int i = 0; i < out.recentProjectLocations.size(); i++)
        {
            if (!std::filesystem::exists(out.recentProjectLocations[i]))
            {
                LOG_INF("Project file %s not found. Removing from recents.\n", out.recentProjectLocations[i].c_str());
                out.recentProjectLocations.erase(out.recentProjectLocations.begin() + i);
            }
        }

        s_EditorConfig = out;

        WorkingDirSet();

        SaveEditorConfig();
    }

    void SaveEditorConfig()
    {
        YAML::Emitter out;
        out << YAML::BeginMap;

        out << YAML::Key << "Installed" << YAML::Value << s_EditorConfig.installDirectory;
        if(s_EditorConfig.recentProjectLocations.size() == 0)
        {
            LOG_WRN("[Editor Config] No previous projects to sove\n");
            std::vector<std::string> empty = {"Empty"};
            out << YAML::Key << "Recents" << YAML::Value << empty;    
        }
        else
        {
            out << YAML::Key << "Recents" << YAML::Value << s_EditorConfig.recentProjectLocations;
        }
        out << YAML::EndMap;

        auto outFile = std::ofstream(EDITOR_CONFIG_PATH);    
        outFile << out.c_str();
        outFile.close();
    }

    void CreateEditorConfig()
    {
        s_EditorConfig = EditorConfig();

        auto tempDir = FileDialogs::PickDir();
        s_EditorConfig.installDirectory = tempDir;

        if (!tempDir.empty())
        {
            s_EditorConfig.installDirectory += "/";

            WorkingDirSet();

            // TODO: Default setting when it's more than just recent projects
            SaveEditorConfig();
        }
        else
        {
                CreateEditorConfig();
        }
    }
    
    void WorkingDirSet()
    {
        std::filesystem::create_directory(s_EditorConfig.installDirectory + "NSAssets/");
        std::filesystem::create_directory(s_EditorConfig.installDirectory + "NSAssets/textures/");
        std::filesystem::create_directory(s_EditorConfig.installDirectory + "NSAssets/debug/");

        std::string in =  (s_EditorConfig.installDirectory + "NSAssets/debug/stdin.txt");
        std::string out = (s_EditorConfig.installDirectory + "NSAssets/debug/stdout.txt");
        std::string err = (s_EditorConfig.installDirectory + "NSAssets/debug/stderr.txt");

        Application::Get()->GetWindow()->SwapIO(in, out, err);
        Application::Get()->GetWindow()->SetIcon(s_EditorConfig.installDirectory + "NSAssets/textures/icon.png");
        
        InitInternalTextures();
    }

    void InitInternalTextures()
    {
        Tex_PlayButton  = Texture2D::Create(s_EditorConfig.installDirectory + "NSAssets/textures/nebula-play-button.png");
        Tex_PauseButton = Texture2D::Create(s_EditorConfig.installDirectory + "NSAssets/textures/nebula-pause-button.png");
        Tex_StopButton  = Texture2D::Create(s_EditorConfig.installDirectory + "NSAssets/textures/nebula-stop-button.png");
        Tex_Minimize    = Texture2D::Create(s_EditorConfig.installDirectory + "NSAssets/textures/nebula-minimize-button.png");
        Tex_Maximize    = Texture2D::Create(s_EditorConfig.installDirectory + "NSAssets/textures/nebula-maximize-button.png");
        Tex_Exit        = Texture2D::Create(s_EditorConfig.installDirectory + "NSAssets/textures/nebula-close-button.png");
        Tex_Cancel      = Texture2D::Create(s_EditorConfig.installDirectory + "NSAssets/textures/nebula-cancel-button.png");
        Tex_Continue    = Texture2D::Create(s_EditorConfig.installDirectory + "NSAssets/textures/nebula-next-button.png");

        return;
    }

    float workingDirButtonSize = 50.0f;
    void DrawWorkingDirSelection()
    {
        ImGui::Begin("Welcome to Nebula Studio!");
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {0, 0});
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});

        ImGui::Dummy({0, 50});

        std::string text = "Welcome to Nebula Studio!";
        float xPos = (ImGui::GetWindowSize().x / 2.0f) - (ImGui::CalcTextSize(text.c_str()).x / 2.0f);
        ImGui::Dummy( { NMax<float>(xPos, 0.0f), 0.0f }); ImGui::SameLine();
        ImGui::Text(text.c_str());

        ImGui::Dummy({ 0, 50 });

        text = "To begin, you must first choose";
        xPos = (ImGui::GetWindowSize().x / 2.0f) - (ImGui::CalcTextSize(text.c_str()).x / 2.0f);
        ImGui::Dummy( { NMax<float>(xPos, 0.0f), 0.0f }); ImGui::SameLine();
        ImGui::Text(text.c_str());

        text = "a directory where your configs";
        xPos = (ImGui::GetWindowSize().x / 2.0f) - (ImGui::CalcTextSize(text.c_str()).x / 2.0f);
        ImGui::Dummy( { NMax<float>(xPos, 0.0f), 0.0f }); ImGui::SameLine();
        ImGui::Text(text.c_str());

        text = "and projects will be saved!";
        xPos = (ImGui::GetWindowSize().x / 2.0f) - (ImGui::CalcTextSize(text.c_str()).x / 2.0f);
        ImGui::Dummy( { NMax<float>(xPos, 0.0f), 0.0f }); ImGui::SameLine();
        ImGui::Text(text.c_str());

        ImGui::Dummy( {0, 35} );

        ImGui::Dummy( { (ImGui::GetWindowSize().x / 2.0f) - (workingDirButtonSize / 2.0f), 0.0f }); ImGui::SameLine();
        if (ImGui::Button("Choose Base Dir..."))
        {
            
            CreateEditorConfig();
            if(!s_EditorConfig.installDirectory.empty())
            {
                hasInternalAssetDir = true;
            } 
        }
        workingDirButtonSize = ImGui::GetItemRectSize().x; 

        ImGui::PopStyleVar(2);
        ImGui::End();
    }

    
    ImVec2 NebToImVec(Vec2f in)
    {
        return {in.X, in.Y};
    }
}