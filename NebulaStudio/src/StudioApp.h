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
        std::string installDirectory;
        std::vector<std::string> recentProjectLocations;

    };

    extern EditorConfig s_EditorConfig;

    extern bool hasWorkingDir;

    void OpenEditorConfig();
    void CreateEditorConfig();
    void SaveEditorConfig();

    void DrawWorkingDirSelection();

    // Done as soon as the working directory has been set/found 
    void WorkingDirSet();
    // Done right after the NSAssets folder has been verified 
    void InitInternalTextures();

    // Engine Needed Textures, should be set in InitInternalTextures
    extern Ref<Texture2D> Tex_PlayButton;
    extern Ref<Texture2D> Tex_PauseButton;
    extern Ref<Texture2D> Tex_StopButton;
    extern Ref<Texture2D> Tex_Minimize;
    extern Ref<Texture2D> Tex_Maximize;
    extern Ref<Texture2D> Tex_Exit;
    extern Ref<Texture2D> Tex_Cancel;
    extern Ref<Texture2D> Tex_Continue;

}