#include "GLRendererAPI.h"

#include <Utils/Logging.h>
#include <glad/glad.h>

namespace Nebula{
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

        SetDepthTest(false);
        SetMSAA(false);
        SetAlphaBlend(false);

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
    
    void GLRendererAPI::SetAlphaBlend(bool enabled)
    {
        if (enabled)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
            return;
        }
        
        glDisable(GL_BLEND);
    }

    void GLRendererAPI::SetDepthTest(bool enabled)
    {
        if (enabled)
        {
            glEnable(GL_DEPTH_TEST);
            return;
        }

        glDisable(GL_DEPTH_TEST);
    }
    
    void GLRendererAPI::SetMSAA(bool enabled)
    {
        if (enabled)
        {
            glEnable(GL_MULTISAMPLE);
            return;
        }
        glDisable(GL_MULTISAMPLE);
    }

    void GLRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    void GLRendererAPI::Clear(Vec4f color) {
        glClearColor(color.X, color.Y, color.Z, color.W);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    
    void GLRendererAPI::DrawIndexed(const int numIndices, PrimativeType type, bool depthTest) { 

        SetDepthTest(depthTest);
        
        switch(type)
        {
            case(PrimativeType::TRIANGLES):
            {
                //glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
                break;
            }
            case(PrimativeType::LINES):
            {
                //glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_INT, nullptr);
                break;
            }
            default:
                break;
        }

        if (!depthTest)
			glEnable(GL_DEPTH_TEST);
        
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void GLRendererAPI::DrawElementsBaseVertex(PrimativeType mode, uint32_t size, PrimativeDataType type, void* indices, uint32_t base) {
        
        GLenum drawmode;
        switch(mode) {
            case(PrimativeType::TRIANGLES):
                drawmode = GL_TRIANGLES;
                break;
            case(PrimativeType::LINES):
                drawmode = GL_LINES;
                break;
            default:
                drawmode = GL_TRIANGLES;
                break;
        }

        GLenum drawDataType;
        switch(type) {
            case(PrimativeDataType::UNSIGNED_BYTE):
                drawDataType = GL_UNSIGNED_BYTE;
                break;
            case(PrimativeDataType::UNSIGNED_SHORT):
                drawDataType = GL_UNSIGNED_SHORT;
                break;
            case(PrimativeDataType::UNSIGNED_INT):
                drawDataType = GL_UNSIGNED_INT;
                break;
            default:
                drawDataType = GL_UNSIGNED_BYTE;
                break;
        }

        //glDrawElementsBaseVertex(drawmode, size, drawDataType, indices, base);
    }

	
}