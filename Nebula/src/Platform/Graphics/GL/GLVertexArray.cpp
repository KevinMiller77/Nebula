#include "GLVertexArray.h"

#include <glad/glad.h>
#include <Nebula_pch.h>

namespace Nebula
{
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

		LOG_ERR("Unknown ShaderDataType!");
		return 0;
	}


    GLVertexArray::GLVertexArray()
    {
        glGenVertexArrays(1, &ID);
    }
    
    GLVertexArray::~GLVertexArray() 
	{
        glDeleteVertexArrays(1, &ID);
    }

    void GLVertexArray::Bind() const 
	{
        glBindVertexArray(ID);
	}
    void GLVertexArray::Unbind() const 
	{
        glBindVertexArray(0);
	}

    void GLVertexArray::AddVertexBuffer(VertexBuffer* vertexBuffer) 
	{
		NEB_PROFILE_FUNCTION();
		if(!vertexBuffer->GetLayout().GetElements().size())
        {
            LOG_ERR("Vertex Buffer has no layout!\n");
        }    

		glBindVertexArray(ID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			switch (element.Type)
			{
				case ShaderDataType::Float:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4:
				case ShaderDataType::Int:
				case ShaderDataType::Int2:
				case ShaderDataType::Int3:
				case ShaderDataType::Int4:
				case ShaderDataType::Bool:
				{
					glEnableVertexAttribArray(indexPosition);
					glVertexAttribPointer(indexPosition,
						element.GetComponentCount(),
						ShaderDataTypeToOpenGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)element.Offset);
					indexPosition++;
					break;
				}
				case ShaderDataType::Mat3:
				case ShaderDataType::Mat4:
				{
					uint8_t count = element.GetComponentCount();
					for (uint8_t i = 0; i < count; i++)
					{
						glEnableVertexAttribArray(indexPosition);
						glVertexAttribPointer(indexPosition,
							count,
							ShaderDataTypeToOpenGLBaseType(element.Type),
							element.Normalized ? GL_TRUE : GL_FALSE,
							layout.GetStride(),
							(const void*)(element.Offset + sizeof(float) * count * i));
						glVertexAttribDivisor(indexPosition, 1);
						indexPosition++;
					}
					break;
				}
				default:
					LOG_ERR("Unknown ShaderDataType!\n");
			}
		}

		vertexBuffers.push_back(vertexBuffer);
	}

    void GLVertexArray::SetIndexBuffer(IndexBuffer* indexBuffer) 
	{
		glBindVertexArray(ID);
		indexBuffer->Bind();

		this->indexBuffer = indexBuffer;
	}

}