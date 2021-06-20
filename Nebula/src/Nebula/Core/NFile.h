#pragma once

#include <string>
#include <fstream>

class NFile{
public:
    enum NFileAccess
    {
        R, W, RW,
    };

    enum NFileDataType
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