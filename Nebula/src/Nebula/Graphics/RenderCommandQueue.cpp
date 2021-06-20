#include "RenderCommandQueue.h"

namespace Nebula {
    RenderCommandQueue::RenderCommandQueue() {
        m_CommandBuffer = new uint8_t[10 * 1024 * 1024]; // Start with 10mb buffer
        m_CommandBufferPtr = m_CommandBuffer;
        memset(m_CommandBuffer, 0, 10 * 1024 * 1024);
    }

    RenderCommandQueue::~RenderCommandQueue() { 
        delete[] m_CommandBuffer;
    }

    void* RenderCommandQueue::Allocate(RenderCommandFn func, uint32_t size) {
        //Put the function ptr in the queue
        *(RenderCommandFn*)m_CommandBufferPtr = func;
        m_CommandBufferPtr += sizeof(RenderCommandFn);

        //Size of function is added to the queue
        *(uint32_t*)m_CommandBufferPtr = size;
        m_CommandBufferPtr += sizeof(uint32_t);

        //Function parameters are stored and output to run func immediatly
        void* mem = m_CommandBufferPtr;
        m_CommandBufferPtr += size;

        m_CommandCount++;

        //Return the ptr to the function so it can be run immediatly
        return mem;
    }

    void RenderCommandQueue::Execute() {

        uint8_t* buffer = m_CommandBuffer;

        for (uint32_t i = 0; i < m_CommandCount; i++) {
            RenderCommandFn func = *(RenderCommandFn*)buffer;
            buffer += sizeof(RenderCommandFn);

            uint32_t size = *(uint32_t*)buffer;
            buffer += sizeof(uint32_t);
            func(buffer);
            buffer += size;
        }

        m_CommandBufferPtr = m_CommandBuffer;
        m_CommandCount = 0;
    }
}