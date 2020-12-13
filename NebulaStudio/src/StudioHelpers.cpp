#include "StudioApp.h"

#include <iostream>
#include <fstream>

#include <ext/yaml-cpp/include/yaml-cpp/yaml.h>


namespace Nebula
{
    EditorConfig s_EditorConfig = EditorConfig();

    void OpenEditorConfig()
    {
        EditorConfig out;

        auto& in = std::ifstream(EDITOR_CONFIG_PATH);
        std::stringstream strStream;
        strStream << in.rdbuf();

        YAML::Node data = YAML::Load(strStream.str());

        out.recentProjectLocations = data["Recents"].as<std::vector<std::string>>();

        if(out.recentProjectLocations.size() == 1 && out.recentProjectLocations[0] == "Empty")
        {
            out.recentProjectLocations.clear();
        }

        for (int i = 0; i < out.recentProjectLocations.size(); i++)
        {
            if (!std::filesystem::exists(out.recentProjectLocations[i]))
            {
                LOG_ERR("Project file %s not found\n", out.recentProjectLocations[i]);
                out.recentProjectLocations.erase(out.recentProjectLocations.begin() + i);
            }
        }

        s_EditorConfig = out;
        SaveEditorConfig();
    }

    void SaveEditorConfig()
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
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

        auto& outFile = std::ofstream(EDITOR_CONFIG_PATH);    
        outFile << out.c_str();
        outFile.close();
    }

    void CreateEditorConfig()
    {
        s_EditorConfig = EditorConfig();

        // TODO: Default setting when it's more than just recent projects
        SaveEditorConfig();
    }
}