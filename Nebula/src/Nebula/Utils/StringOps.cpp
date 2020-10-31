#include "StringOps.h"

namespace Nebula
{
    bool EndsWith(std::string str1, std::string str2)
    {
        size_t sizeStr1 = str1.size();
        size_t sizeStr2 = str2.size();
        
        if (sizeStr1 == sizeStr2)
        {
            return str1 == str2;
        }
        else if (sizeStr2 > sizeStr1)
        {
            return false;
        }

        std::string cutoff = str1.substr(sizeStr1 - sizeStr2);

        if (cutoff == str2)
        {
            return true;
        }
        return false;
    }
}