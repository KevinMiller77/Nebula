#include "MemoryTracker.h"

namespace Nebula{
    MemoryMetrics::MemoryMetrics(bool strtTracker) 
    {
        enable = true; 
        tracking = strtTracker; 
        currentMemoryUsed= 0; 
        trackingMemoryUsed = 0; 
    }

    void MemoryMetrics::off()               { enable = false; }
    void MemoryMetrics::on()                { enable = true; }
    void MemoryMetrics::add(size_t size)    { currentMemoryUsed += size; trackingMemoryUsed += tracking ? size : 0; }
    void MemoryMetrics::del(size_t size)    { currentMemoryUsed -= size; trackingMemoryUsed -= tracking ? size : 0; }
    unsigned int MemoryMetrics::get()       { return currentMemoryUsed; }

    void MemoryMetrics::track() 
    {
        tracking = true;
        trackingMemoryUsed = 0;
    }

    long int MemoryMetrics::doneTracking() 
    { 
        long int tracked = trackingMemoryUsed;
        tracking = false; 
        tracking = 0;
        return tracked;
    }
}