#include "RendererAPI.h"

#include <Platform/Graphics/GL/GLRendererAPI.h>

namespace Nebula
{
    RendererAPI::API RendererAPI::NebAPI = RendererAPI::API::OpenGL;

    RendererAPI* RendererAPI::Create()
    {
        switch(RendererAPI::GetAPI())
        {
            case(RendererAPI::API::None): LOG_ERR("No rendering API selected!\n"); return nullptr;
            case(RendererAPI::API::OpenGL): return new GLRendererAPI();
        }
    }

}