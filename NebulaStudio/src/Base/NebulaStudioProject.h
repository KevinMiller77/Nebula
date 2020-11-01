#pragma once

#include <Nebula.h>
#include <string>
#include <fstream>

namespace Nebula
{
    namespace StudioProject
    {
        struct Project
        {
            bool loaded = false;

            std::string ProjectName = std::string();
            std::string LastSceneOpened = std::string();
            
            std::string AbsolutePath = std::string();
            std::string LastFileSystemMount = std::string();
        };

        //All project paths are absolute since the VFS relies on them
        Project CreateProjectFile(std::string path);
        void SaveProjectFile(Project& proj);
        Project LoadProjectFile(std::string path);

    }
}