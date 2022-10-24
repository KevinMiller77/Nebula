#pragma once

#include <Graphics/Shader.h>
#include <vector>
#include <array>
#include <spirv_glsl.hpp>
// #include <Core/NebulaCommon.h>

typedef uint32_t GLenum;

namespace Nebula{
    class GLShader : public Shader, public std::enable_shared_from_this<GLShader>
    {
    public:
		using ShaderReloadedCallback = std::function<void()>;

        GLShader() = default;
        // GLShader(const std::string filepath);
        GLShader(const std::string& shaderSrc);
        GLShader(const Builtin::BuiltInShader& shaderSrc);
        // GLShader(const std::string name, const std::string& vertexSrc, const std::string& fragmentSrc);
        virtual ~GLShader() override;

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
        
		void Load(const std::string& source, bool forceRecompile);

        std::unordered_map<GLenum, std::string> PreProcess(const std::string& src);

        void Reflect(std::vector<uint32_t>& data);

		void CompileOrGetVulkanBinary(std::array<std::vector<uint32_t>, 2>& outputBinary, bool forceCompile = false);
		void CompileOrGetOpenGLBinary(const std::array<std::vector<uint32_t>, 2>& vulkanBinaries, bool forceCompile = false);

        std::string ReadShaderFromFile(const std::string& filepath) const;

        void ParseConstantBuffers(const spirv_cross::CompilerGLSL& compiler);

		int32_t GetUniformLocation(const std::string& name) const;

		void CompileAndUploadShader();
		static GLenum ShaderTypeFromString(const std::string& type);

    private:
        uint32_t m_RendererID = 0;
        std::string m_Name, m_AssetPath;
		bool m_Loaded = false;
		bool m_IsCompute = false;
        
		std::unordered_map<GLenum, std::string> m_ShaderSource;

        std::vector<ShaderReloadedCallback> m_ShaderReloadedCallbacks;
        inline static std::unordered_map<uint32_t, ShaderUniformBuffer> s_UniformBuffers;

		std::unordered_map<std::string, ShaderBuffer> m_Buffers;
		std::unordered_map<std::string, ShaderResourceDeclaration> m_Resources;
		std::unordered_map<std::string, int> m_UniformLocations;
    };
}