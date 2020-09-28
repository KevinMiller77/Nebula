#pragma once
#include <chrono>
#include <Core/NebulaCommon.h>

using namespace std;
using namespace chrono;

namespace Nebula
{

    class Timer
    {
    private:
        uint32 frames;
        duration<double> timeSpan;
        steady_clock::time_point mainTimerStart;
        steady_clock::time_point mainTimer;

    public:
        Timer()
        {
            frames = 0;
            mainTimerStart = steady_clock::now();
        }

        void Start()
        {
            timeSpan = (duration<double>)(0);
            mainTimerStart = steady_clock::now();
            frames = 0;
        }

        void Reset()
        {
            mainTimerStart = steady_clock::now();
        }

        double GetTimePassed() { 
            timeSpan = (steady_clock::now() - mainTimerStart);
            return timeSpan.count();
        }

        uint32 FrameKeep()
        {
            frames++;
            if (GetTimePassed() >= 1.0f)
            {
                uint32 thisgoround = frames;
                Reset();
                frames = 0;
                return thisgoround;
            }
            else
            {
                return 0;
            }
        }

        double GetTimePassedReset()
        {
            double time = GetTimePassed();
            Reset();
            return time;
        }

        uint64_t GetTimeMS() {
            return (uint64_t)(GetTimePassed() * 1000.0f);
        }
    };
}
