#include "Shader.h"
#include <Graphics/RendererConfig.h>

#include <Graphics/VertexArray.h>
#include <Platform/Graphics/GL/GLShader.h>
#include <Platform/Graphics/Metal/MetalShader.h>

namespace Nebula{

    Ref<Shader> Shader::Create(const std::string& shaderSrc, Ref<VertexArray> vertexArray)
    {
        switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    LOG_ERR("RendererAPI::None is currently not supported!\n"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<GLShader>(shaderSrc, vertexArray);
			case RendererAPI::API::Metal:   return CreateRef<MetalShader>(shaderSrc, vertexArray);
            default:                        return nullptr;
		}

		LOG_ERR("Unknown RendererAPI!\n");
		return nullptr;
    }   

    Ref<Shader> Shader::Create(const Builtin::BuiltInShader& shaderSrc, Ref<VertexArray> vertexArray)
    {
        switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    LOG_ERR("RendererAPI::None is currently not supported!\n"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<GLShader>(shaderSrc, vertexArray);
			case RendererAPI::API::Metal:   return CreateRef<MetalShader>(shaderSrc, vertexArray);
            default:                        return nullptr;
		}

		LOG_ERR("Unknown RendererAPI!\n");
		return nullptr;
    }   

    ShaderUniform::ShaderUniform(const std::string& name, ShaderUniformType type, uint32_t size, uint32_t offset)
		: m_Name(name), m_Type(type), m_Size(size), m_Offset(offset)
	{
	}
    
    void ShaderLibrary::Add(Ref<Shader> shader)
    {
        const std::string name = shader->GetName();
        if (Exists(name)) LOG_WRN("Shader already exists!\n");
        shaders[name] = shader;
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& filepath, Ref<VertexArray> vertexArray)
    {
        // Get file name without extension
        std::string fileName = std::filesystem::u8path(filepath).stem().string();

        auto shader = Shader::Create(filepath, vertexArray);
        
        Add(shader);
        
        return shader;
    }
    
    // Ref<Shader> ShaderLibrary::Load(const std::string name, const std::string& filepath)
    // {
    //     auto shader = Shader::Create(filepath);
    //     Add(shader);
    //     return shader;
    // }

    Ref<Shader> ShaderLibrary::Get(const std::string& name)
    {
        if (!Exists(name)) 
        {
            LOG_ERR("Shader does not exist: %s!\n", name.c_str());
            return nullptr;
        }
        return shaders[name];
    }
    bool ShaderLibrary::Exists(const std::string& name)
    {
        return shaders.find(name) != shaders.end();
    }
}