#include "VFS.h"

namespace Nebula
{
    
    bool VFS::Mounted;
    std::string VFS::AbsoluteRoot;

    void VFS::Init()
	{
        Mounted = false;
        AbsoluteRoot = std::string();
    }
    
    // NFile VFS::CreateFile(std::string path, std::string name)
	// {

	// }

    // NFile VFS::OpenFile()
	// {

	// }

    // NFile VFS::AppendFile()
	// {

	// }

    bool VFS::CD(std::string newDir)
	{
        return true;
	}

    bool VFS::Mount(const std::string root)
	{
        if(std::filesystem::exists(root) && std::filesystem::is_directory(root))
        {
            AbsoluteRoot = root;
            return true;
        }
        return false;
	}

    void VFS::Unmount()
	{
        AbsoluteRoot = std::string();
	}

    std::string VFS::GetRoot()
    {
        return AbsoluteRoot;
    }

    bool VFS::Exists(std::string path, bool absolutePath)
    {
        if (!absolutePath && !IsMounted())
        {
            LOG_ERR("Filesystem not mounted!!");
            return false;
        }
        //TODO: Check that the path string is valid
        return std::filesystem::exists(absolutePath ? path : AbsoluteRoot + path);
    }

    bool VFS::IsDir(std::string path, bool absolutePath)
    {
        if (!absolutePath && !IsMounted())
        {
            LOG_ERR("Filesystem not mounted!!");
            return false;
        }

        if (Exists(path, absolutePath))
        {
            return std::filesystem::is_directory(absolutePath ? path : AbsoluteRoot + path);
        }
        return false;
    }

    bool VFS::IsFile(std::string path, bool absolutePath)
    {
        if (!absolutePath && !IsMounted())
        {
            LOG_ERR("Filesystem not mounted!!");
            return false;
        }
        
        if (Exists(path, absolutePath))
        {
            return !std::filesystem::is_directory(absolutePath ? path : AbsoluteRoot + path);
        }
        return false;
    }

    bool VFS::IsMounted()
	{
        return !AbsoluteRoot.empty();
	}

}