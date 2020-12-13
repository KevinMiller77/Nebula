#pragma once
#include <Nebula.h>

#include <vector>
#include <string>
#include <filesystem>

#define EDITOR_CONFIG_PATH "NebulaEditorSettings.yaml"

namespace Nebula
{
    struct EditorConfig
    {
        std::vector<std::string> recentProjectLocations;

    };

    extern EditorConfig s_EditorConfig;

    void OpenEditorConfig();
    void CreateEditorConfig();
    void SaveEditorConfig();

}