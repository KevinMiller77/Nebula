#pragma once

#include <string>
#include <unordered_map>
#include <Math/math.h>
#include <Utils/Logging.h>
#include <Core/Ref.h>
#include <Graphics/ShaderUniform.h>
#include <filesystem>
#include "BuiltinShaders.h"

namespace Nebula{
    enum class UniformType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Matrix3x3, Matrix4x4,
		Int32, Uint32
	};

	struct UniformDecl
	{
		UniformType Type;
		std::ptrdiff_t Offset;
		std::string Name;
	};

	struct UniformBuffer
	{
		// TODO: This currently represents a uint8_t buffer that has been
		// packed with uniforms. This was primarily created for OpenGL,
		// and needs to be revisted for other rendering APIs. Furthermore,
		// this currently does not assume any alignment. This also has
		// nothing to do with GL uniform buffers, this is simply a CPU-side
		// buffer abstraction.
		uint8_t* Buffer;
		std::vector<UniformDecl> Uniforms;
	};

    struct UniformBufferBase
	{
		virtual const uint8_t* GetBuffer() const = 0;
		virtual const UniformDecl* GetUniforms() const = 0;
		virtual unsigned int GetUniformCount() const = 0;
	};

	template<unsigned int N, unsigned int U>
	struct UniformBufferDeclaration : public UniformBufferBase
	{
		uint8_t Buffer[N];
		UniformDecl Uniforms[U];
		std::ptrdiff_t Cursor = 0;
		int Index = 0;

		virtual const uint8_t* GetBuffer() const override { return Buffer; }
		virtual const UniformDecl* GetUniforms() const override { return Uniforms; }
		virtual unsigned int GetUniformCount() const { return U; }

		template<typename T>
		void Push(const std::string& name, const T& data) {}

		template<>
		void Push(const std::string& name, const float& data)
		{
			Uniforms[Index++] = { UniformType::Float, Cursor, name };
			memcpy(Buffer + Cursor, &data, sizeof(float));
			Cursor += sizeof(float);
		}

		template<>
		void Push(const std::string& name, const Vec3f& data)
		{
			Uniforms[Index++] = { UniformType::Float3, Cursor, name };
			memcpy(Buffer + Cursor, glm::value_ptr(data), sizeof(Vec3f));
			Cursor += sizeof(Vec3f);
		}

		template<>
		void Push(const std::string& name, const Vec4f& data)
		{
			Uniforms[Index++] = { UniformType::Float4, Cursor, name };
			memcpy(Buffer + Cursor, glm::value_ptr(data), sizeof(Vec4f));
			Cursor += sizeof(Vec4f);
		}

		template<>
		void Push(const std::string& name, const Mat4f& data)
		{
			Uniforms[Index++] = { UniformType::Matrix4x4, Cursor, name };
			memcpy(Buffer + Cursor, data.elements, sizeof(Vec4f) * 4);
			Cursor += sizeof(Vec4f) * 4;
		}

	};

    enum class ShaderUniformType
	{
		None = 0, Bool, Int, Float, Vec2, Vec3, Vec4, Mat3, Mat4
	};

	class ShaderUniform
	{
	public:
		ShaderUniform() = default;
		ShaderUniform(const std::string& name, ShaderUniformType type, uint32_t size, uint32_t offset);

		const std::string& GetName() const { return m_Name; }
		ShaderUniformType GetType() const { return m_Type; }
		uint32_t GetSize() const { return m_Size; }
		uint32_t GetOffset() const { return m_Offset; }

		static const std::string& UniformTypeToString(ShaderUniformType type);
	private:
		std::string m_Name;
		ShaderUniformType m_Type = ShaderUniformType::None;
		uint32_t m_Size = 0;
		uint32_t m_Offset = 0;
	};

	struct ShaderUniformBuffer
	{
		std::string Name;
		uint32_t Index;
		uint32_t BindingPoint;
		uint32_t Size;
		uint32_t RendererID;
		std::vector<ShaderUniform> Uniforms;
	};

	struct ShaderBuffer
	{
		std::string Name;
		uint32_t Size = 0;
		std::unordered_map<std::string, ShaderUniform> Uniforms;
	};
    
    class Shader
    {
    public:
        virtual ~Shader() {};

        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        virtual void Reload(bool forceReload) = 0;
        
		using ShaderReloadedCallback = std::function<void()>;
		
		virtual void AddShaderReloadedCallback(const ShaderReloadedCallback& callback) = 0;
        virtual void SetUniformBuffer(const std::string& name, const void* data, uint32_t size) = 0;
        
        virtual void SetUniform(const std::string name, const int& value) = 0;
        virtual void SetUniform(const std::string name, int* values, int count) = 0;
        virtual void SetUniform(const std::string name, const float& value) = 0;
        virtual void SetUniform(const std::string name, const Vec2f& value) = 0;
        virtual void SetUniform(const std::string name, const Vec3f& value) = 0;
        virtual void SetUniform(const std::string name, const Vec4f& value) = 0;
        virtual void SetUniform(const std::string name, const Mat3f& value) = 0;
        virtual void SetUniform(const std::string name, const Mat4f& value) = 0;

        virtual const std::string& GetName() const = 0;

        // static Ref<Shader> Create(const std::string name, const std::string& vertexSrc, const std::string& fragmentSrc);
        static Ref<Shader> Create(const std::string& shaderSrc);
        static Ref<Shader> Create(const Builtin::BuiltInShader& shaderSrc);
        
		virtual const std::unordered_map<std::string, ShaderBuffer>& GetShaderBuffers() const = 0;
		virtual const std::unordered_map<std::string, ShaderResourceDeclaration>& GetResources() const = 0;
    };

    class ShaderLibrary
    {
    public:

        void Add(Ref<Shader> shader);
        Ref<Shader> Load(const std::string& filepath);
        // Ref<Shader> Load(const std::string name, const std::string& filepath);

        Ref<Shader> Get(const std::string& name);
		std::unordered_map<std::string, Ref<Shader>> GetAll() { return shaders; }

        bool Exists(const std::string& name);
    private:
        std::unordered_map<std::string, Ref<Shader>> shaders;
    
    };
}