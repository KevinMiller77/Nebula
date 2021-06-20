#pragma once

#include "NFile.h"
#include <filesystem>

#include <Utils/Logging.h>

namespace Nebula{
    enum class VFSerr
    {
        NONE = 0,
        FILESYSTEM_NOT_MOUNTED = 1,
        INVALID_PATH = 2,
        ALREADY_MOUNTED = 3,
        UNKNOWN_ERROR = 4

    };

    std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);
    class VFS
    {
    public:
        static void Init();


        static NFile CreateFile(std::string path, std::string name = "");
        static NFile OpenFile();
        static NFile AppendFile();

        static bool CD(std::string newDir);
        static bool Mount(const std::string root);
        static void Unmount();

        static bool IsMounted();
        static std::string GetRoot();
        static std::string AbsolutePath(std::string path);
        static std::string Path(std::string path);

        static bool Exists(std::string path, bool absolutePath = false);
        static bool IsDir(std::string path, bool absolutePath = false);
        static bool IsFile(std::string path, bool absolutePath = false);


        static void freopen(std::string file, const char* mode, FILE* stream);

        friend class VFSExplorer;
    private:
        static bool Mounted;
        static std::string AbsoluteRoot;
    };

    class VFSExplorer
    {
    public:
        // dir is from the root of the currently mounted VFS
        VFSExplorer(std::string dir);

        std::string GetCurPath();
        std::string GetSelectedObject();

    private:
        std::string Selection;
        std::string CurDirectory;
    };
}