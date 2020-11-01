#include "NebulaStudioProject.h"
#include <ext/yaml-cpp/include/yaml-cpp/yaml.h>

namespace Nebula
{
    namespace StudioProject
    {
        Project CreateProjectFile(std::string path)
        {
            std::string dirToMakeFileIn = std::string(path);

            #ifdef NEB_PLATFORM_WINDOWS
                dirToMakeFileIn = dirToMakeFileIn.substr(0, dirToMakeFileIn.find_last_of("\\") + 1);
            #else
                dirToMakeFileIn = dirToMakeFileIn.substr(0, dirToMakeFileIn.find_last_of("/") + 1);
            #endif

            Project proj;
            proj.AbsolutePath = path;
            proj.LastFileSystemMount = dirToMakeFileIn;
            proj.loaded = true;


            YAML::Emitter out;
            out << YAML::BeginMap;
            out << YAML::Key << "Project Name" << YAML::Value << proj.ProjectName;
            out << YAML::Key << "Absolute Path" << YAML::Value << proj.AbsolutePath;
            out << YAML::Key << "Project VFS Loc" << YAML::Value << proj.LastFileSystemMount;
            out << YAML::Key << "Last Opened Scene" << YAML::Value << proj.LastSceneOpened;
            out << YAML::EndMap;

            std::ofstream fout(proj.AbsolutePath);
            fout << out.c_str();

            return proj;
        }

        void SaveProjectFile(Project& proj)
        {
            YAML::Emitter out;
            out << YAML::BeginMap;
            out << YAML::Key << "Project Name" << YAML::Value << proj.ProjectName;
            out << YAML::Key << "Absolute Path" << YAML::Value << proj.AbsolutePath;
            out << YAML::Key << "Project VFS Loc" << YAML::Value << proj.LastFileSystemMount;
            out << YAML::Key << "Last Opened Scene" << YAML::Value << proj.LastSceneOpened;
            out << YAML::EndMap;

            std::ofstream fout(proj.AbsolutePath);
            fout << out.c_str();
        }
        
        
        Project LoadProjectFile(std::string path)
        {
            Project out;

            std::ifstream stream(path);
            std::stringstream strStream;
            strStream << stream.rdbuf();

            YAML::Node data = YAML::Load(strStream.str());

            out.ProjectName = data["Project Name"].as<std::string>();
            out.AbsolutePath = data["Absolute Path"].as<std::string>();
            out.LastFileSystemMount = data["Project VFS Loc"].as<std::string>();
            out.LastSceneOpened = data["Last Opened Scene"].as<std::string>();
            
            out.loaded = true;

            return out;
        }
    
    
    }
}