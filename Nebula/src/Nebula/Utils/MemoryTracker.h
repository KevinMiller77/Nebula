#pragma once

#include <memory>
#include <Core/NebulaCommon.h>
namespace Nebula{
    class MemoryMetrics
    {
    private:
        //Total memory tracked with the tracker
        uint32 currentMemoryUsed;
        //Memory tracked with the current tracker
        long int trackingMemoryUsed;

    public:
        //Big killswitch that will be nice to turn off for certain areas of code that we dont need to see alloc for 
        bool enable; 
        //Sets if we're in a tracking session
        bool tracking;

        MemoryMetrics(bool strtTracker = false);

        void off();
        void on();

        void add(size_t size);
        void del(size_t size);

        void track();
        long int doneTracking();
        
        uint32 get();
    };
}
