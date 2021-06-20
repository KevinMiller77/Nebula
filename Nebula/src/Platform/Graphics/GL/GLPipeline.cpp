#include "GLPipeline.h"
#include <glad/glad.h>

namespace Nebula {
    
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return GL_FLOAT;
			case ShaderDataType::Float2:   return GL_FLOAT;
			case ShaderDataType::Float3:   return GL_FLOAT;
			case ShaderDataType::Float4:   return GL_FLOAT;
			case ShaderDataType::Mat3:     return GL_FLOAT;
			case ShaderDataType::Mat4:     return GL_FLOAT;
			case ShaderDataType::Int:      return GL_INT;
			case ShaderDataType::Int2:     return GL_INT;
			case ShaderDataType::Int3:     return GL_INT;
			case ShaderDataType::Int4:     return GL_INT;
			case ShaderDataType::Bool:     return GL_BOOL;
		}

		assert(false);
		return 0;
	}

    GLPipeline::GLPipeline(const PipelineSpecification& spec) 
        : m_Specification(spec) 
    {
        Invalidate();
	}

    GLPipeline::~GLPipeline() {
        glDeleteVertexArrays(1, &m_VertexArrayRendererID);
	}

    void GLPipeline::Invalidate() {
        assert(m_Specification.Layout.GetElements().size());

        if (m_VertexArrayRendererID)
            glDeleteVertexArrays(1, &m_VertexArrayRendererID);

        glGenVertexArrays(1, &m_VertexArrayRendererID);
	}

    void GLPipeline::Bind() {
        glBindVertexArray(m_VertexArrayRendererID);

        const BufferLayout& layout = m_Specification.Layout;
        uint32_t attribIndex = 0;
        for (const BufferElement& element : layout)
        {
            auto glBaseType = ShaderDataTypeToOpenGLBaseType(element.Type);
            glEnableVertexAttribArray(attribIndex);

            if (glBaseType == GL_INT)
            {
                glVertexAttribIPointer(attribIndex,
                    element.GetComponentCount(),
                    glBaseType,
                    layout.GetStride(),
                    (const void*)(intptr_t)element.Offset);
            }
            else
            {
                glVertexAttribPointer(attribIndex,
                    element.GetComponentCount(),
                    glBaseType,
                    element.Normalized ? GL_TRUE : GL_FALSE,
                    layout.GetStride(),
                    (const void*)(intptr_t)element.Offset);
            }
            attribIndex++;
        }
	}

}