#pragma once
#include <string>
#include "SoundBuffer.h"

namespace Nebula
{
    class SoundCore
    {
    public:
        static void Init();
        static SoundBuffer CreateBuffer();
    };
}

