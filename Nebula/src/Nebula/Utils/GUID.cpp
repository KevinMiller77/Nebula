#include "GUID.h"

namespace Nebula{
    static unsigned long x=123456789, y=362436069, z=521288629;

    unsigned long xorshf96() 
    {
        unsigned long t;
        x ^= x << 16;
        x ^= x >> 5;
        x ^= x << 1;

        t = x;
        x = y;
        y = z;
        z = t ^ x ^ y;

        return z;
    }
    
    namespace GUID
    {
        bool GUIDExits(GUID_t item)
        {
            if (GUID_Map.find(item) == GUID_Map.end())
            {
                return false;
            }
            return true;
        }
        
        GUID_t Generate()
        {
            GUID_t randNum = xorshf96();
            if (GUIDExits(randNum))
            {
                return Generate();
            }
            GUID_Map[randNum] = true;
            return randNum;
        }

        
        void Delete(GUID_t item)
        {
            
        }
         
    } 
}
