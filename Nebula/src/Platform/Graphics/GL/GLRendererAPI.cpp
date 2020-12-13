#include "GLRendererAPI.h"

#include <Utils/Logging.h>
#include <glad/glad.h>

namespace Nebula
{
    static void APIENTRY GLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:         LOG_ERR("%s\n", message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:       LOG_ERR("%s\n", message); return;
			case GL_DEBUG_SEVERITY_LOW:          LOG_WRN("%s\n", message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_INF("%s\n", message); return;
		}
		
		LOG_INF("OpenGL Message: Unknown severity level!");
	}


    void GLRendererAPI::Init() 
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(GLMessageCallback, nullptr);
        
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

        // glDisable(GL_CULL_FACE);
        // glEnable(GL_TEXTURE_2D);
        // glAlphaFunc(GL_GREATER, 0.0f);

        glEnable(GL_MULTISAMPLE);
        glEnable(GL_DEPTH_TEST);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    }

    void GLRendererAPI::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
    {
        glViewport(x, y, width, height);
    }

    void GLRendererAPI::SetClearColor(const Vec4f& color)
    {
        glClearColor(color.X, color.Y, color.Z, color.W);
    }
    
    void GLRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void GLRendererAPI::DrawIndexed(const Ref<VertexArray> vertexArray, PrimativeType type, uint32 indexCount)
    {
        uint32 count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
        
        switch(type)
        {
            case(TRIANGLES):
            {
                glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
                break;
            }
            case(LINES):
            {
                glDrawElements(GL_LINES, count, GL_UNSIGNED_INT, nullptr);
                break;
            }
            default:
                break;
        }
        
        glBindTexture(GL_TEXTURE_2D, 0);
    }
	
}