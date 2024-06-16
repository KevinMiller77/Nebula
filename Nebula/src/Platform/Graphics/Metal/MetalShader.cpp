#include "MetalShader.h"

#include <Core/Ref.h>
#include <Foundation/NSEnumerator.hpp>
#include <Graphics/Shader.h>
#include <Graphics/ShaderUniform.h>
#include <Graphics/Renderer.h>
#include <Metal/MTLArgument.hpp>
#include <Platform/Graphics/Metal/MetalVertexArray.h>
#include <Platform/Graphics/Metal/MetalRendererAPI.h>
#include <Platform/Graphics/Metal/MetalContext.h>

#include <Utils/Logging.h>
#include <cstddef>
#include <cstring>
#include <fstream>

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

namespace Nebula{

	ShaderUniformType MetalToNebula(MTL::DataType type) {
		switch (type) {
			case MTL::DataTypeFloat: return ShaderUniformType::Float;
			case MTL::DataTypeFloat2: return ShaderUniformType::Float;
			case MTL::DataTypeFloat3: return ShaderUniformType::Float;
			case MTL::DataTypeFloat4: return ShaderUniformType::Float;
			case MTL::DataTypeFloat2x2: return ShaderUniformType::Float;
			case MTL::DataTypeFloat2x3: return ShaderUniformType::Float;
			case MTL::DataTypeFloat2x4: return ShaderUniformType::Float;
			case MTL::DataTypeFloat3x2: return ShaderUniformType::Float;
			case MTL::DataTypeFloat3x3: return ShaderUniformType::Float;
			case MTL::DataTypeFloat3x4: return ShaderUniformType::Float;
			case MTL::DataTypeFloat4x2: return ShaderUniformType::Float;
			case MTL::DataTypeFloat4x3: return ShaderUniformType::Float;
			case MTL::DataTypeFloat4x4: return ShaderUniformType::Float;

			// case MTL::DataTypeHalf: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeHalf2: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeHalf3: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeHalf4: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeHalf2x2: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeHalf2x3: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeHalf2x4: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeHalf3x2: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeHalf3x3: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeHalf3x4: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeHalf4x2: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeHalf4x3: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeHalf4x4: 	return ShaderUniformType::Int ;

			case MTL::DataTypeInt: 		return ShaderUniformType::Int;
			case MTL::DataTypeInt2: 	return ShaderUniformType::Int;
			case MTL::DataTypeInt3: 	return ShaderUniformType::Int;
			case MTL::DataTypeInt4: 	return ShaderUniformType::Int;
			// case MTL::DataTypeUInt: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeUInt2: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeUInt3: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeUInt4: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeShort: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeShort2: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeShort3: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeShort4: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeUShort: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeUShort2: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeUShort3: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeUShort4: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeChar: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeChar2: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeChar3: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeChar4: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeUChar: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeUChar2: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeUChar3: 	return ShaderUniformType::Int ;
			// case MTL::DataTypeUChar4: 	return ShaderUniformType::Int ;
			case MTL::DataTypeBool: 	return ShaderUniformType::Bool ;
			case MTL::DataTypeBool2: 	return ShaderUniformType::Bool ;
			case MTL::DataTypeBool3: 	return ShaderUniformType::Bool ;
			case MTL::DataTypeBool4: 	return ShaderUniformType::Bool ;

			// case MTL::DataTypeMat3: return ShaderUniformType::Mat3;
			// case MTL::DataTypeMat4: return ShaderUniformType::Mat4;
			default: break;
		}

		LOG_ERR("[Metal][Shader] Unknown data type\n"); 
		abort();
	}

	MetalShader::MetalShader(const std::string& shaderSrc, Ref<VertexArray> vertexArray)
	{
		m_AssetPath = shaderSrc;
		m_VertexArray = vertexArray;

		if (m_VertexArray == nullptr) {
			LOG_ERR("[Metal][Shader] Attempted to create a shader with null vertex array!\n");
			abort();
		}

        size_t found = shaderSrc.find_last_of("/\\");
		m_Name = found != std::string::npos ? shaderSrc.substr(found + 1) : shaderSrc;
		found = m_Name.find_last_of(".");
		m_Name = found != std::string::npos ? m_Name.substr(0, found) : m_Name;

		Reload();
	}

    
    MetalShader::MetalShader(const Builtin::BuiltInShader& shaderSrc, Ref<VertexArray> vertexArray)
		: m_VertexArray(vertexArray) 
	{
		if (m_VertexArray == nullptr) {
			LOG_ERR("[Metal][Shader] Attempted to create a shader with null vertex array!\n");
			abort();
		}

        std::string hiddenSrcDir = "./.builtinshadersrc/";
        m_Name = shaderSrc.name;

        if(!std::filesystem::exists(hiddenSrcDir)) {
            std::filesystem::create_directory(hiddenSrcDir);
        }


        std::ofstream outFile(hiddenSrcDir + shaderSrc.name + ".metal");
        if (outFile) {
            outFile.write(shaderSrc.src.c_str(), shaderSrc.src.size());
            outFile.close();

            m_AssetPath = std::filesystem::absolute(hiddenSrcDir + shaderSrc.name + ".metal").string(); 

            Reload();
        }
    }

    MetalShader::~MetalShader() {}

    void MetalShader::Reload(bool forceRecompile)
	{
		std::string source = ReadShaderFromFile(m_AssetPath);
		Load(source);
	}

    void MetalShader::Bind()
	{ 
		if (m_RenderPipelineState == nullptr) {
			LOG_ERR("[Metal][Shader] Attempted to bind a shader with no configured pipeline state!\n");
			abort();
		}
		MetalRendererAPI::BindShader(m_RenderPipelineState);
	}

    void MetalShader::Unbind()
	{ 

	}

    void MetalShader::Load(const std::string& source)
	{
		if (m_Library) {
			LOG_INF("[Metal][Shader] Reloading shader: %s\n", m_Name.c_str());
			m_Library = nullptr;
		}

		NS::String* src = NS::String::string(source.c_str(), NS::UTF8StringEncoding);
		
		MTL::CompileOptions* options = MTL::CompileOptions::alloc()->init();
		
		NS::Error** errorCb = (NS::Error**)malloc(sizeof(NS::Error**));
		*errorCb = (NS::Error*)malloc(sizeof(NS::Error*));

		m_Library = MetalRendererAPI::GetDevice()->newLibrary(
			src, 
			options,
			errorCb
		);

		if (errorCb != nullptr && *errorCb != nullptr) {
			LOG_ERR("[Metal][Shader] Failed to compile shader\n");
			LOG_ERR("%s\n", (*errorCb)->localizedDescription()->utf8String());
			abort();
		}

		Reflect();
	}

	void MetalShader::AddShaderReloadedCallback(const ShaderReloadedCallback& callback)
	{
		m_ShaderReloadedCallbacks.push_back(callback);
	}

    std::string MetalShader::ReadShaderFromFile(const std::string& filepath) const
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
		}
		else
		{
			LOG_ERR("[Metal][Shader] Could not open shader file '%s'", filepath.c_str());
			abort();
		}
		in.close();
		return result;
	}

	void MetalShader::Reflect() {
		if (!m_Library) {
			LOG_ERR("[Metal][Shader] Attmepted to perform reflection on an unloaded shader\n");
			abort();
		}

		std::vector<MTL::Function*> functions = {};

		size_t functionCount = m_Library->functionNames()->count();
		for (size_t i = 0; i < functionCount; i++) {
			NS::String* functionName = (NS::String*)m_Library->functionNames()->object(i);
			MTL::Function* function = m_Library->newFunction(functionName);
			if (!function) {
				LOG_ERR("[Metal][Shader] Failed to reflect function: %s\n", functionName->utf8String());
				continue;
			}

			functions.push_back(function);
		}

		MTL::VertexDescriptor* vertexDescriptor = MTL::VertexDescriptor::alloc()->init();
		MTL::VertexAttributeDescriptorArray* descriptorAttributeArray =  vertexDescriptor->attributes();
		MTL::VertexBufferLayoutDescriptorArray* descriptorLayoutArray = vertexDescriptor->layouts();

		MTL::Function* vertexFunction = nullptr;
		MTL::Function* fragmentFunction = nullptr;

		for (MTL::Function* function : functions) {
			MTL::FunctionType type = function->functionType();
			switch (type) {
			case MTL::FunctionTypeVertex: {
				if (vertexFunction) {
					LOG_ERR("[Metal][Shader] Multiple vertex functions found in shader during reflection: orig: %s, new: %s\n", vertexFunction->name()->utf8String(), function->name()->utf8String());
				}
				vertexFunction = function;
			} break;
			case MTL::FunctionTypeFragment:
				if (fragmentFunction) {
					LOG_ERR("[Metal][Shader] Multiple fragment functions found in shader during reflection: orig: %s, new: %s\n", fragmentFunction->name()->utf8String(), function->name()->utf8String());
				}
				fragmentFunction = function;
				break;
			case MTL::FunctionTypeKernel:
				break;
			default:
				break;
			}
		}


		MTL::RenderPipelineDescriptor* pipelineDesc = MTL::RenderPipelineDescriptor::alloc()->init();
		if (vertexFunction) {
			pipelineDesc->setVertexFunction(vertexFunction);
		}
		if (fragmentFunction) {
			pipelineDesc->setFragmentFunction(fragmentFunction);
		}
		// Reflect on the fragment outputs and set colorAttatchments accordingly
		pipelineDesc->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
		pipelineDesc->colorAttachments()->object(1)->setPixelFormat(MTL::PixelFormatR32Sint);

		pipelineDesc->setVertexDescriptor(CastRef<MetalVertexArray>(m_VertexArray)->m_VertexDescriptor);

		NS::Error** errorCb = (NS::Error**)malloc(sizeof(NS::Error**));
		*errorCb = (NS::Error*)malloc(sizeof(NS::Error*));

		MTL::AutoreleasedRenderPipelineReflection* reflectCb = (MTL::AutoreleasedRenderPipelineReflection*)malloc(sizeof(MTL::AutoreleasedRenderPipelineReflection*));
		*reflectCb = (MTL::AutoreleasedRenderPipelineReflection)malloc(sizeof(MTL::AutoreleasedRenderPipelineReflection));

		MTL::RenderPipelineState* pipelineState = MetalRendererAPI::GetDevice()->newRenderPipelineState(
			pipelineDesc, 
			MTL::PipelineOptionBufferTypeInfo, 
			reflectCb, 
			errorCb
		);

		if (errorCb != nullptr && *errorCb != nullptr) {
			LOG_ERR("[Metal][Shader] Failed to create pipeline state for reflection: %s\n", (*errorCb)->localizedDescription()->utf8String());
			abort();
		}

		if (pipelineState == nullptr) {
			LOG_ERR("[Metal][Shader] Failed to create pipeline state for reflection\n");
			abort();
		}

		if (reflectCb == nullptr) {
			LOG_ERR("[Metal][Shader] Failed to reflect pipeline state\n");
			abort();
		}

		MTL::RenderPipelineReflection* reflection = (MTL::RenderPipelineReflection*)*reflectCb;
		if (reflection == nullptr) {
			LOG_ERR("[Metal][Shader] Failed to reflect pipeline state\n");
			abort();
		}

		NS::Array* vertexArguments = reflection->vertexArguments();
		NS::Array* fragmentArguments = reflection->fragmentArguments();

		if (vertexArguments) {
			for (size_t i = 0; i < vertexArguments->count(); i++) {
				MTL::Argument* arg = (MTL::Argument*)vertexArguments->object(i);
				
				LOG_INF("[Metal][Shader] Vertex Argument: %s\n", arg->name()->utf8String());
				if (std::string(arg->name()->utf8String()).find("vertexBuffer") != std::string::npos) {
					continue;
				}

				LOG_INF("\t\t Type: %lu\n", arg->type());
				if (arg->type() == MTL::ArgumentTypeBuffer) {
					LOG_INF("\t\t Buffer Type: %lu\n", arg->bufferDataType());
					LOG_INF("\t\t Buffer Data Size: %lu\n", arg->bufferDataSize());
					LOG_INF("\t\t Buffer alignment: %lu\n", arg->bufferAlignment());

					m_VertexUniforms[std::string(arg->name()->utf8String())] = MetalUniformData(
						(uint32_t)arg->index(),
						MetalUniformLocationEnum::VERTEX,
						nullptr
					);

					ShaderUniform uniform = ShaderUniform{
						std::string(arg->name()->utf8String()),
						MetalToNebula(arg->bufferDataType()),
						(uint32_t)arg->bufferDataSize(),
						0
					};

					m_Buffers[std::string(arg->name()->utf8String())] = ShaderBuffer{
						std::string(arg->name()->utf8String()),
						(uint32_t)arg->bufferDataSize(),
						{ 
							{ std::string(arg->name()->utf8String()), uniform } 
						}
					};
				}
			}
		
		}

		if (fragmentArguments) {
			for (size_t i = 0; i < fragmentArguments->count(); i++) {
				MTL::Argument* arg = (MTL::Argument*)fragmentArguments->object(i);
				
				if (std::string(arg->name()->utf8String()).find("fragmentBuffer") != std::string::npos) {
					continue;
				}

				LOG_INF("[Metal][Shader] Fragment Argument: %s\n", arg->name()->utf8String());
				if (arg->type() == MTL::ArgumentTypeTexture) {
					LOG_INF("\t\t Texture Data: %lu\n", arg->textureDataType());
					LOG_INF("\t\t Texture Type: %lu\n", arg->textureType());
					LOG_INF("\t\t Texture Index: %lu\n", arg->index());

					// Add this to the resources
					// m_Resources[std::string(arg->name()->utf8String())] = ShaderResourceDeclaration{
					// 	std::string(arg->name()->utf8String()),
					// 	(uint32_t)arg->index(),
					// 	1
					// };

					m_FragmentUniforms[std::string(arg->name()->utf8String())] = MetalUniformData(
						(uint32_t)arg->index(),
						MetalUniformLocationEnum::FRAGMENT,
						nullptr
					);

					m_Resources[std::string(arg->name()->utf8String())] = ShaderResourceDeclaration{
						std::string(arg->name()->utf8String()),
						(uint32_t)arg->index(),
						1
					};
				}
			}
		}

		m_RenderPipelineState = pipelineState;
	}

	void MetalShader::SetUniformBuffer(const std::string& name, const void* data, uint32_t size)
	{
		if (m_VertexUniforms.find(name) != m_VertexUniforms.end()) {
			MetalUniformData& uniform = m_VertexUniforms[name];
			
			// If uniform buffer exists but size is incorrect, release it
			if (uniform.Buffer != nullptr && uniform.Buffer->length() < size) {
				uniform.Buffer->release();
				uniform.Buffer = nullptr;
			}
			
			// If uniform buffer does not exist, create it
			if (uniform.Buffer == nullptr) {
				uniform.Buffer = MetalRendererAPI::GetDevice()->newBuffer(data, size, MTL::ResourceOptionCPUCacheModeDefault);
				uniform.Buffer->retain();
			} else {
				memcpy(uniform.Buffer->contents(), data, size);
			}

			MetalRendererAPI::BindShaderVertexBuffer(uniform.Buffer, 0, uniform.Index);
			return;
		} 
		
		if (m_FragmentUniforms.find(name) != m_FragmentUniforms.end()) {
			MetalUniformData& uniform = m_FragmentUniforms[name];
			
			// If uniform buffer exists but size is incorrect, release it
			if (uniform.Buffer != nullptr && uniform.Buffer->length() < size) {
				uniform.Buffer->release();
				uniform.Buffer = nullptr;
			}
			
			// If uniform buffer does not exist, create it
			if (uniform.Buffer == nullptr) {
				uniform.Buffer = MetalRendererAPI::GetDevice()->newBuffer(data, size, MTL::ResourceOptionCPUCacheModeDefault);
				uniform.Buffer->retain();
			} else {
				memcpy(uniform.Buffer->contents(), data, size);
			}

			MetalRendererAPI::BindShaderFragmentBuffer(uniform.Buffer, 0, uniform.Index);
			return;
		}

		LOG_ERR("[Metal][Shader] Uniform buffer %s not found in shader\n", name.c_str());
		abort();
	}

    void MetalShader::SetUniform(const std::string name, const int& value)
	{
        UploadUniformInt(name, value);
	}
    void MetalShader::SetUniform(const std::string name, int* values, int count)
	{
        UploadUniformIntArray(name, values, count);
	}
    void MetalShader::SetUniform(const std::string name, const float& value)
	{
        UploadUniformFloat(name, value);
	}
    void MetalShader::SetUniform(const std::string name, const Vec2f& value)
	{
        UploadUniformFloat2(name, value);
	}
    void MetalShader::SetUniform(const std::string name, const Vec3f& value)
	{
        UploadUniformFloat3(name, value);
	}
    void MetalShader::SetUniform(const std::string name, const Vec4f& value)
	{
        UploadUniformFloat4(name, value);
	}
    void MetalShader::SetUniform(const std::string name, const Mat3f& value)
	{
        UploadUniformMat3(name, value);
	}
    void MetalShader::SetUniform(const std::string name, const Mat4f& value)
	{
        UploadUniformMat4(name, value);
	}
	
    void MetalShader::UploadUniformInt(const std::string& name, int value)
	{
		SetUniformBuffer(name, &value, sizeof(int));
	}
    void MetalShader::UploadUniformIntArray(const std::string& name, int* values, unsigned int count)
	{
		SetUniformBuffer(name, values, count * sizeof(int));
	}
    void MetalShader::UploadUniformFloat(const std::string& name, float value)
	{
		SetUniformBuffer(name, &value, sizeof(float));
	}
    void MetalShader::UploadUniformFloat2(const std::string& name, const Vec2f& value)
	{
		SetUniformBuffer(name, value.elements, sizeof(value.elements));
	}
    void MetalShader::UploadUniformFloat3(const std::string& name, const Vec3f& value)
	{
		SetUniformBuffer(name, value.elements, sizeof(value.elements));
	}
	void MetalShader::UploadUniformFloat4(const std::string& name, const Vec4f& value)
	{
		SetUniformBuffer(name, value.elements, sizeof(value.elements));
	}

	void MetalShader::UploadUniformMat3(const std::string& name, const Mat3f& matrix)
	{
		SetUniformBuffer(name, matrix.elements, sizeof(matrix.elements));
	}

	void MetalShader::UploadUniformMat4(const std::string& name, const Mat4f& matrix)
	{
		SetUniformBuffer(name, matrix.elements, sizeof(matrix.elements));
	}

    const std::string& MetalShader::GetName() const
	{
        return m_Name;
	}

	/*
	====================================================================================================
										End MetalShader
	====================================================================================================
	*/
	namespace MetalShaderStringUtils {

		// Parsing helper functions
		const char* FindToken(const char* str, const std::string& token)
		{
			const char* t = str;
			while ((t = strstr(t, token.c_str())))
			{
				bool left = str == t || isspace(t[-1]);
				bool right = !t[token.size()] || isspace(t[token.size()]);
				if (left && right)
					return t;

				t += token.size();
			}
			return nullptr;
		}

		const char* FindToken(const std::string& string, const std::string& token)
		{
			return FindToken(string.c_str(), token);
		}

		std::vector<std::string> SplitString(const std::string& string, const std::string& delimiters)
		{
			size_t start = 0;
			size_t end = string.find_first_of(delimiters);

			std::vector<std::string> result;

			while (end <= std::string::npos)
			{
				std::string token = string.substr(start, end - start);
				if (!token.empty())
					result.push_back(token);

				if (end == std::string::npos)
					break;

				start = end + 1;
				end = string.find_first_of(delimiters, start);
			}

			return result;
		}

		std::vector<std::string> SplitString(const std::string& string, const char delimiter)
		{
			return SplitString(string, std::string(1, delimiter));
		}

		std::vector<std::string> Tokenize(const std::string& string)
		{
			return SplitString(string, " \t\n");
		}

		std::vector<std::string> GetLines(const std::string& string)
		{
			return SplitString(string, "\n");
		}

		std::string GetBlock(const char* str, const char** outPosition)
		{
			const char* end = strstr(str, "}");
			if (!end)
				return str;

			if (outPosition)
				*outPosition = end;
			uint32_t length = end - str + 1;
			return std::string(str, length);
		}

		std::string GetStatement(const char* str, const char** outPosition)
		{
			const char* end = strstr(str, ";");
			if (!end)
				return str;

			if (outPosition)
				*outPosition = end;
			uint32_t length = end - str + 1;
			return std::string(str, length);
		}

		bool StartsWith(const std::string& string, const std::string& start)
		{
			return string.find(start) == 0;
		}
	}
}
