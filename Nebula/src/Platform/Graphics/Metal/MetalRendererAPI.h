#pragma once

#include <Graphics/RendererAPI.h>
#include <Core/NebulaCommon.h>
#include <Metal/MTLLibrary.hpp>
#include <Metal/MTLRenderPass.hpp>
#include <Metal/MTLTexture.hpp>
#include <Platform/Graphics/Metal/MetalBuffers.h>
#include <Platform/Graphics/Metal/MetalFramebuffer.h>
#include <Platform/Graphics/Metal/MetalVertexArray.h>
#include <Platform/Graphics/Metal/MetalTexture2D.h>


namespace Nebula {
    class MetalContext;
    class MetalRendererAPI : public RendererAPI {
    public:
        virtual void Init(Ref<GraphicsContext>) override;

        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        virtual void SetClearColor(const Vec4f& color) override;
        
        virtual void Clear() override;
        virtual void Clear(Vec4f color) override;
        
        virtual void Prepare() override;
        virtual void Commit() override;
        
        virtual void SetAlphaBlend(bool enabled) override;
        virtual void SetDepthTest(bool enabled) override;
        virtual void SetMSAA(bool enabled) override;
        
        virtual void DrawIndexed(const int numIndices, PrimativeType type, bool depthTest) override;
        virtual void DrawElementsBaseVertex(PrimativeType mode, uint32_t size, PrimativeDataType type, void* indices, uint32_t base) override;

        static MTL::Device* GetDevice();
    private:
        static void BindRenderPass(MTL::RenderPassDescriptor* renderPass, MTL::Viewport viewport);
        static void UnbindRenderPass();

        static void BindVertexBuffer(MTL::Buffer* vertextBuffer);
        static void BindIndexBuffer(MTL::Buffer* indexBuffer);
        static void BindTexture2D(MTL::Texture* texture, uint32_t slot);

        static void BindShader(MTL::RenderPipelineState* shader);
        static void BindShaderVertexBuffer(MTL::Buffer* buffer, uint32_t offset, uint32_t index);
        static void BindShaderFragmentBuffer(MTL::Buffer* buffer, uint32_t offset, uint32_t index);
        static void BindShaderFragmentTexture(MTL::Texture* texture, uint32_t slot);

        static Ref<MetalContext> s_Context;

        friend class MetalIndexBuffer;
        friend class MetalVertexBuffer;
        friend class MetalVertexArray;
        friend class MetalShader;
        friend class MetalTexture2D;
        friend class MetalImGuiRenderer;
        friend class MacImGuiLayer;
        friend class MetalFramebuffer;
    };
}