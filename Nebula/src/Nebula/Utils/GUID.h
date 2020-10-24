#pragma once
#include <unordered_map>

namespace Nebula
{
    typedef unsigned long GUID_t;

    static unsigned long xorshf96();
    
    namespace GUID
    {
        static std::unordered_map<GUID_t, bool> GUID_Map;

        bool GUIDExits(GUID_t item);
        GUID_t Generate();
        void Delete(GUID_t item);
    } 
}
