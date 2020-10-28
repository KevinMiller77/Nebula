#include <string>

namespace Nebula
{
    class FileDialogs
    {
    public:
        //Return empty str if failed/canceled
        static std::string OpenFile(const char* filter);
        static std::string SaveFile(const char* filter);
    };
}