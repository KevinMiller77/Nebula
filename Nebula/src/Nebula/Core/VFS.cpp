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

    
    void VFS::freopen(std::string file, const char* mode, FILE* stream)
    {
        std::string tmp = AbsoluteRoot + file;
        ::freopen(tmp.c_str(), mode, stream);
    }

    bool VFS::CD(std::string newDir)
	{
        return true;
	}

    std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) 
    {
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
        }
        return str;
    }

    bool VFS::Mount(const std::string root)
	{
        if(std::filesystem::exists(root) && std::filesystem::is_directory(root))
        {
            AbsoluteRoot = root;
            Mounted = true;
            return true;
        }
        return false;
	}

    void VFS::Unmount()
	{
        AbsoluteRoot = std::string();
        Mounted = false;
	}

    std::string VFS::GetRoot()
    {
        return AbsoluteRoot;
    }

    
    std::string VFS::AbsolutePath(std::string path)
    {
        
        return AbsoluteRoot + path;
    }

    
    std::string VFS::Path(std::string path)
    {
        if (IsMounted())
        {
            if (path.substr(0, VFS::GetRoot().size()) == AbsoluteRoot)
            {
                return path.substr(AbsoluteRoot.size());
            }
        }
        return path;
    }

    bool VFS::Exists(std::string path, bool absolutePath)
    {
        if (!absolutePath && !IsMounted())
        {
            LOG_ERR("Filesystem not mounted!!\n");
            return false;
        }
        //TODO: Check that the path string is valid
        bool result = std::filesystem::exists(absolutePath ? path : AbsoluteRoot + path);
        return result;
    }

    bool VFS::IsDir(std::string path, bool absolutePath)
    {
        if (!absolutePath && !IsMounted())
        {
            LOG_ERR("Filesystem not mounted!!\n");
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
            LOG_ERR("Filesystem not mounted!!\n");
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