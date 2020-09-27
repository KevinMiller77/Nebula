#include "Shader.h"
#include <Graphics/RendererConfig.h>

#include <Platform/Graphics/GL/GLShader.h>

namespace Nebula
{
    Shader* Shader::Create(const std::string& filepath)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    LOG_ERR("RendererAPI::None is currently not supported!\n"); return nullptr;
			case RendererAPI::API::OpenGL:  return new GLShader(filepath);
		}

		LOG_ERR("Unknown RendererAPI!\n");
		return nullptr;
	}

    Shader* Shader::Create(const std::string name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    LOG_ERR("RendererAPI::None is currently not supported!\n"); return nullptr;
			case RendererAPI::API::OpenGL:  return new GLShader(name, vertexSrc, fragmentSrc);
		}

		LOG_ERR("Unknown RendererAPI!\n");
		return nullptr;
	}
    
    void ShaderLibrary::Add(const std::string& name, Shader* shader)
    {
        if (Exists(name)) LOG_WRN("Shader already existed!\n");
        shaders[name] = shader;
    }
    void ShaderLibrary::Add(Shader* shader)
    {
        const std::string name = shader->GetName();
        Add(name, shader);
    }

    Shader* ShaderLibrary::Load(const std::string& filepath)
    {
        auto shader = Shader::Create(filepath);
        Add(shader);
        return shader;
    }
    Shader* ShaderLibrary::Load(const std::string name, const std::string& filepath)
    {
        auto shader = Shader::Create(filepath);
        Add(name, shader);
        return shader;
    }

    Shader* ShaderLibrary::Get(std::string& name)
    {
        if (!Exists(name)) 
        {
            LOG_ERR("Shader does not exist!\n");
            return nullptr;
        }
        return shaders[name];
    }

    bool ShaderLibrary::Exists(const std::string name)
    {
        return shaders.find(name) != shaders.end();
    }
}