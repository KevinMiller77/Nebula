#include "MetalVertexArray.h"

#include <Foundation/NSTypes.hpp>
#include <Graphics/VertexArray.h>
#include <Metal/MTLVertexDescriptor.hpp>
#include <Metal/Metal.hpp>
#include <Platform/Graphics/Metal/MetalRendererAPI.h>

namespace Nebula {


    static MTL::VertexFormat ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return MTL::VertexFormat::VertexFormatFloat;
			case ShaderDataType::Float2:   return MTL::VertexFormat::VertexFormatFloat2;
			case ShaderDataType::Float3:   return MTL::VertexFormat::VertexFormatFloat3;
			case ShaderDataType::Float4:   return MTL::VertexFormat::VertexFormatFloat4;
			case ShaderDataType::Mat3:     return MTL::VertexFormat::VertexFormatFloat3;
			case ShaderDataType::Mat4:     return MTL::VertexFormat::VertexFormatFloat4;
			case ShaderDataType::Int:      return MTL::VertexFormat::VertexFormatInt;
			case ShaderDataType::Int2:     return MTL::VertexFormat::VertexFormatInt2;
			case ShaderDataType::Int3:     return MTL::VertexFormat::VertexFormatInt3;
			case ShaderDataType::Int4:     return MTL::VertexFormat::VertexFormatInt4;
			case ShaderDataType::Bool:     return MTL::VertexFormat::VertexFormatChar;
            default: break;
		}

		LOG_ERR("Unknown ShaderDataType!");
		return MTL::VertexFormat::VertexFormatInvalid;
	}

    MetalVertexArray::MetalVertexArray() {
        m_VertexDescriptor = MTL::VertexDescriptor::vertexDescriptor();
    }

    MetalVertexArray::~MetalVertexArray() {
        if (m_VertexDescriptor) m_VertexDescriptor->reset();
    }

    void MetalVertexArray::Bind() const {
        if (m_VertexBuffers.size() == 0) {
            LOG_ERR("No Vertex Buffers!\n");
            abort();
        }

        if (m_VertexBuffers.size() > 1) {
            LOG_ERR("Multiple Vertex Buffers not supported!\n");
            abort();
        }
        
        m_VertexBuffers[0]->Bind();
        m_IndexBuffer->Bind();
    }

    void Unbind() {}

    void MetalVertexArray::AddVertexBuffer(Ref<VertexBuffer> vertexBuffer) {
        if (!vertexBuffer->GetLayout().GetElements().size()) {
            LOG_ERR("Vertex Buffer has no layout!\n");
            abort();
        }

        const auto& layout = vertexBuffer->GetLayout();
        
        MTL::VertexBufferLayoutDescriptor* layoutDesc = MTL::VertexBufferLayoutDescriptor::alloc()->init();
        layoutDesc->setStepFunction(MTL::VertexStepFunction::VertexStepFunctionPerVertex);
        layoutDesc->setStride(layout.GetStride());
        layoutDesc->setStepRate(1);

        MTL::VertexBufferLayoutDescriptorArray* descriptorLayoutArray = m_VertexDescriptor->layouts();
        descriptorLayoutArray->setObject(layoutDesc, 0);

        for (const auto& element : layout) {
            MTL::VertexAttributeDescriptorArray* descriptorAttributeArray =  m_VertexDescriptor->attributes();


            if (ShaderDataTypeToOpenGLBaseType(element.Type) == MTL::VertexFormat::VertexFormatInvalid) {
                LOG_ERR("Unknown ShaderDataType!\n");
                // TODO: Should we exit here? We will likely crash anyway
                return;
            }

            // If we have a matrix or other 2d structure, we should put vectors of the first dimension
            // into the array the count of the second dimension
            uint32_t secondaryDimesions = 1;
            if (element.Type == ShaderDataType::Mat3 || element.Type == ShaderDataType::Mat4) {
                secondaryDimesions = element.GetComponentCount();
            }

            for (uint32_t i = 0; i < secondaryDimesions; i++) {
                MTL::VertexAttributeDescriptor* attributeDesc = MTL::VertexAttributeDescriptor::alloc()->init();

                attributeDesc->setBufferIndex(0);
                attributeDesc->setOffset(element.Offset);
                attributeDesc->setFormat(ShaderDataTypeToOpenGLBaseType(element.Type));

                descriptorAttributeArray->setObject(attributeDesc, m_IndexPosition);

                m_IndexPosition++;
            }
        }

        m_VertexBuffers.push_back(vertexBuffer);
    }

    void MetalVertexArray::SetIndexBuffer(Ref<IndexBuffer> indexBuffer) {
        m_IndexBuffer = indexBuffer;
    }
}
