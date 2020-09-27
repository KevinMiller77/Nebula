#include "GLRendererAPI.h"

#include <Utils/Logging.h>
#include <glad/glad.h>

namespace Nebula
{
    void GLMessageCallback(
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
			case GL_DEBUG_SEVERITY_HIGH:         LOG_ERR("%s", message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:       LOG_ERR("%s", message); return;
			case GL_DEBUG_SEVERITY_LOW:          LOG_WRN("%s", message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_INF("%s", message); return;
		}
		
		LOG_INF("OpenGL Message: Unknown severity level!");
	}


    void GLRendererAPI::Init() 
    {
        // glEnable(GL_DEBUG_OUTPUT);
        // glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        // glDebugMessageCallback(GLMessageCallback, nullptr);
        
        // glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POLYGON_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    }

    void GLRendererAPI::SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
    {
        glViewport(x, y, width, height);
    }

    void GLRendererAPI::SetClearColor(const Vec4f& color)
    {
        glClearColor(color.x, color.y, color.z, color.w);
    }
    
    void GLRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void GLRendererAPI::DrawIndexed(const VertexArray* vertexArray, unsigned int indexCount)
    {
        unsigned int count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
	
}