#pragma once

#include <Graphics/Shader.h>
#include <Metal/MTLRenderPipeline.hpp>
#include <unordered_map>
#include <vector>

namespace MTL {
    class Library;
    class Buffer;
}

namespace Nebula{
    enum MetalUniformLocationEnum {
        VERTEX,
        FRAGMENT
    };

    struct MetalUniformData {
        uint32_t Index;
        MetalUniformLocationEnum Type;
        MTL::Buffer *Buffer;

        MetalUniformData() = default;
        MetalUniformData(uint32_t index, MetalUniformLocationEnum type, MTL::Buffer *buffer)
            : Index(index), Type(type), Buffer(buffer) {}
    };

    class MetalShader : public Shader, public std::enable_shared_from_this<MetalShader>
    {
    public:
		using ShaderReloadedCallback = std::function<void()>;

        MetalShader() = default;
        // MetalShader(const std::string filepath);
        MetalShader(const std::string& shaderSrc, Ref<VertexArray> vertexArray);
        MetalShader(const Builtin::BuiltInShader& shaderSrc, Ref<VertexArray> vertexArray);
        virtual ~MetalShader() override;

        virtual void Bind() override;
        virtual void Unbind() override;
        virtual void Reload(bool forceReload = false) override;
		virtual void AddShaderReloadedCallback(const ShaderReloadedCallback& callback) override;
        virtual void SetUniformBuffer(const std::string& name, const void* data, uint32_t size) override;
        
        virtual void SetUniform(const std::string name, const int& value) override;
        virtual void SetUniform(const std::string name, int* values, int count) override;
        virtual void SetUniform(const std::string name, const float& value) override;
        virtual void SetUniform(const std::string name, const Vec2f& value) override;
        virtual void SetUniform(const std::string name, const Vec3f& value) override;
        virtual void SetUniform(const std::string name, const Vec4f& value) override;
        virtual void SetUniform(const std::string name, const Mat3f& value) override;
        virtual void SetUniform(const std::string name, const Mat4f& value) override;

        virtual const std::string& GetName() const override;

        void UploadUniformInt(const std::string& name, int value);
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const Vec2f& value);
		void UploadUniformFloat3(const std::string& name, const Vec3f& value);
		void UploadUniformFloat4(const std::string& name, const Vec4f& value);

		void UploadUniformMat3(const std::string& name, const Mat3f& matrix);
		void UploadUniformMat4(const std::string& name, const Mat4f& matrix);

		virtual const std::unordered_map<std::string, ShaderBuffer>& GetShaderBuffers() const override { return m_Buffers; }
		virtual const std::unordered_map<std::string, ShaderResourceDeclaration>& GetResources() const override { return m_Resources; }
    private:
        
		void Load(const std::string& source);
        void Reflect();
        
        std::string ReadShaderFromFile(const std::string& filepath) const;

    private:
        Ref<VertexArray> m_VertexArray;
        MTL::Library* m_Library = nullptr;
        MTL::RenderPipelineState* m_RenderPipelineState = nullptr;

        std::string m_Name, m_AssetPath;
		bool m_Loaded = false;

        std::vector<ShaderReloadedCallback> m_ShaderReloadedCallbacks;
        
        inline static std::unordered_map<uint32_t, ShaderUniformBuffer> s_UniformBuffers;
		std::unordered_map<std::string, ShaderBuffer> m_Buffers;

		std::unordered_map<std::string, ShaderResourceDeclaration> m_Resources;

		std::unordered_map<std::string, MetalUniformData> m_VertexUniforms;
        std::unordered_map<std::string, MetalUniformData> m_FragmentUniforms;
    };
}