#include "MetalContext.h"

#include <Nebula_pch.h>

namespace Nebula {

    MetalContext::MetalContext(void* window)
    {
        m_Window = window;
    }

    void MetalContext::Init()
    {
        NEB_PROFILE_FUNCTION();


        if(!InitContext_Int(m_Window)) {
            LOG_ERR("Could not init Metal context!");
            assert((false));
        }

        LOG_INF("Metal Init Success!\n");
        LOG_INF("TODO: Give Metal Info\n");
    }

    bool MetalContext::SwapBuffers()
    {
        return SwapBuffers_Int();
    }

    void MetalContext::SetVSync(bool vsync) {
        SetVSync_Int(vsync);
    }
}