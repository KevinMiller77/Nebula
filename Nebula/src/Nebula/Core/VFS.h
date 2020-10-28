#pragma once

#include <string>
#include <fstream>

namespace Nebula
{
    static enum class VFSerr
    {
        NONE = 0,
        INVALID_PATH = 1,
        ALREADY_MOUNTED = 2,
        UNKNOWN_ERROR = 3
    };


    class NFile
    {
    public:
        static enum NFileAccess
        {
            R, W, RW,
        };

        static enum NFileDataType
        {
            BIN, TXT 
        };

        NFile() = default;
        NFile(const std::string path, NFileAccess access, NFileDataType type = TXT);

        bool IsValid();
        bool IsOpen();
        bool EndOfFile();

        bool IsWritable();
        bool WriteBytes(uint8_t* bytes, size_t numBytes);
        bool WriteText(std::string text);

        bool IsReadable();
        uint8_t* ReadBytes(size_t numBytes);
        std::string ReadText(size_t numChars);
        std::string GetLine(); 

        inline void Seek(uint32_t newCursor) { Cursor = newCursor; } 
        inline uint32_t Pos() { return Cursor; }

    private:
        std::string Path;
        std::fstream File;
        uint32_t Cursor = 0;
    };

    class VFS
    {
    public:
    
    static NFile CreateFile(std::string path, std::string name = "");
    static bool CD(std::string addition);
    static bool Mount(const std::string Root);
    static bool Unmount();

    private:
        std::string Root;
    };
}