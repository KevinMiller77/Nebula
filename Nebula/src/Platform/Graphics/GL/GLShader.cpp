#include "GLShader.h"

#include <fstream>
#include <glad/glad.h>
#include <shaderc/shaderc.hpp>
#include <Graphics/Renderer.h>

namespace Nebula{
    static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		LOG_ERR("Unknown shader type!\n");
		return 0;
	}

	GLShader::GLShader(const std::string& shaderSrc)
        : m_AssetPath(shaderSrc)
	{
        size_t found = shaderSrc.find_last_of("/\\");
		m_Name = found != std::string::npos ? shaderSrc.substr(found + 1) : shaderSrc;
		found = m_Name.find_last_of(".");
		m_Name = found != std::string::npos ? m_Name.substr(0, found) : m_Name;

		Reload();
	}

    
    GLShader::GLShader(const Builtin::BuiltInShader& shaderSrc) {
        std::string hiddenSrcDir = "./.builtinshadersrc/";
        m_Name = shaderSrc.name;

        if(!std::filesystem::exists(hiddenSrcDir)) {
            std::filesystem::create_directory(hiddenSrcDir);
        }


        std::ofstream outFile(hiddenSrcDir + shaderSrc.name + ".glsl");
        if (outFile) {
            outFile.write(shaderSrc.src.c_str(), shaderSrc.src.size());
            outFile.close();

            m_AssetPath = std::filesystem::absolute(hiddenSrcDir + shaderSrc.name + ".glsl").string(); 

            Reload();
        }
    }

    // GLShader::GLShader(const std::string name, const std::string& vertexSrc, const std::string& fragmentSrc)
    //     :   m_Name(name)
    // {
    //     std::unordered_map<GLenum, std::string> sources;
	// 	sources[GL_VERTEX_SHADER] = vertexSrc;
	// 	sources[GL_FRAGMENT_SHADER] = fragmentSrc;
	// 	Compile(sources);
	// }

    GLShader::~GLShader()
	{
        glDeleteProgram(m_RendererID);
	}

    void GLShader::Reload(bool forceRecompile)
	{
		std::string source = ReadShaderFromFile(m_AssetPath);
		Load(source, forceRecompile);
	}

    void GLShader::Bind()
	{
        glUseProgram(m_RendererID);
	}
    void GLShader::Unbind()
	{
        glUseProgram(0);
	}
	
	
    // void GLShader::Reload(const std::string& input, bool inputIsFilePath)
	// {
	// 	glUseProgram(0);
	// 	glDeleteProgram(m_RendererID);

	// 	std::unordered_map<GLenum, std::string> srcs;
	// 	if( inputIsFilePath)
	// 	{
	// 		srcs = PreProcess(ReadFile(input));
	// 	}
	// 	else
	// 	{
	// 		srcs = PreProcess(input);
	// 	}
		

    //     Compile(srcs);
	// }

    // void GLShader::Reload(const std::string& vertexShader, const std::string& fragmentShader)
	// {
    //     std::unordered_map<GLenum, std::string> sources;
	// 	sources[GL_VERTEX_SHADER] = vertexShader;
	// 	sources[GL_FRAGMENT_SHADER] = fragmentShader;
	// 	Compile(sources);
	// }


    // std::string GLShader::ReadFile(const std::string& filepath)
    // {
    //     std::string result;
	// 	std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
	// 	if (in)
	// 	{
	// 		in.seekg(0, std::ios::end);
	// 		size_t size = (size_t)in.tellg();
	// 		if (size != -1)
	// 		{
	// 			result.resize(size);
	// 			in.seekg(0, std::ios::beg);
	// 			in.read(&result[0], size);
	// 		}
	// 		else
	// 		{
	// 			LOG_ERR("Could not read from file '%s'", filepath.c_str());
	// 		}
	// 	}
	// 	else
	// 	{
	// 		LOG_ERR("Could not open file '%s'", filepath.c_str());
	// 	}

	// 	return result;
    // }

    // std::unordered_map<GLenum, std::string> GLShader::PreProcess(const std::string& src)
    // {
    //     std::unordered_map<GLenum, std::string> shaderSources;

	// 	const char* typeToken = "#type";
	// 	size_t typeTokenLength = strlen(typeToken);
	// 	size_t pos = src.find(typeToken, 0); //Start of shader type declaration line
	// 	while (pos != std::string::npos)
	// 	{
	// 		size_t eol = src.find_first_of("\r\n", pos); //End of shader type declaration line
	// 		if(eol == std::string::npos) LOG_ERR("Syntax error\n");
	// 		size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
	// 		std::string type = src.substr(begin, eol - begin);
	// 		if(!ShaderTypeFromString(type)) LOG_ERR("Invalid shader type specified\n");

	// 		size_t nextLinePos = src.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
	// 		if(nextLinePos == std::string::npos) LOG_ERR("Syntax error\n");
	// 		pos = src.find(typeToken, nextLinePos); //Start of next shader type declaration line

	// 		shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? src.substr(nextLinePos) : src.substr(nextLinePos, pos - nextLinePos);
	// 	}

	// 	return shaderSources;
	//}

    static ShaderUniformType SPIRTypeToShaderUniformType(spirv_cross::SPIRType type)
	{
		switch (type.basetype)
		{
		case spirv_cross::SPIRType::Boolean:  return ShaderUniformType::Bool;
		case spirv_cross::SPIRType::Int:      return ShaderUniformType::Int;
		case spirv_cross::SPIRType::Float:
			if (type.vecsize == 1)            return ShaderUniformType::Float;
			if (type.vecsize == 2)            return ShaderUniformType::Vec2;
			if (type.vecsize == 3)            return ShaderUniformType::Vec3;
			if (type.vecsize == 4)            return ShaderUniformType::Vec4;

			if (type.columns == 3)            return ShaderUniformType::Mat3;
			if (type.columns == 4)            return ShaderUniformType::Mat4;
			break;
		}
		assert(false, "Unknown type!");
		return ShaderUniformType::None;
	}

    void GLShader::ParseConstantBuffers(const spirv_cross::CompilerGLSL& compiler)
	{
		spirv_cross::ShaderResources res = compiler.get_shader_resources();
		for (const spirv_cross::Resource& resource : res.push_constant_buffers)
		{
			auto& bufferType = compiler.get_type(resource.base_type_id);
			auto location = compiler.get_decoration(resource.id, spv::DecorationLocation);
			const auto& bufferName = resource.name;
			auto bufferSize = compiler.get_declared_struct_size(bufferType);
			int memberCount = bufferType.member_types.size();
			ShaderBuffer& buffer = m_Buffers[bufferName];
			buffer.Name = bufferName;
			buffer.Size = bufferSize;
			for (int i = 0; i < memberCount; i++)
			{
				auto type = compiler.get_type(bufferType.member_types[i]);
				const auto& memberName = compiler.get_member_name(bufferType.self, i);
				auto size = compiler.get_declared_struct_member_size(bufferType, i);
				auto offset = compiler.type_struct_member_offset(bufferType, i);

				std::string uniformName = bufferName + "." + memberName;
				buffer.Uniforms[uniformName] = ShaderUniform(uniformName, SPIRTypeToShaderUniformType(type), size, offset);
			}

		}
	}

	void GLShader::Reflect(std::vector<uint32_t>& data)
	{
		spirv_cross::Compiler comp(data);
		spirv_cross::ShaderResources res = comp.get_shader_resources();

		LOG_DBG("GLShader::Reflect - %s\n", m_AssetPath.c_str());
		LOG_DBG("   %d Uniform Buffers\n", res.uniform_buffers.size());
		LOG_DBG("   %d Resources\n", res.sampled_images.size());

		glUseProgram(m_RendererID);

		uint32_t bufferIndex = 0;
		for (const spirv_cross::Resource& resource : res.uniform_buffers)
		{
			auto& bufferType = comp.get_type(resource.base_type_id);
			int memberCount = bufferType.member_types.size();
			uint32_t bindingPoint = comp.get_decoration(resource.id, spv::DecorationBinding);

			if (s_UniformBuffers.find(bindingPoint) == s_UniformBuffers.end())
			{
				ShaderUniformBuffer& buffer = s_UniformBuffers[bindingPoint];
				buffer.Name = resource.name;
				buffer.BindingPoint = bindingPoint;
				buffer.Size = comp.get_declared_struct_size(bufferType);
			 
#if 0
				buffer.Uniforms.reserve(memberCount);
				for (int i = 0; i < memberCount; i++)
				{
					auto type = comp.get_type(bufferType.member_types[i]);
					const auto& name = comp.get_member_name(bufferType.self, i);
					auto size = comp.get_declared_struct_member_size(bufferType, i);
					auto offset = comp.type_struct_member_offset(bufferType, i);

					//ShaderUniformType uniformType = SPIRTypeToShaderUniformType(type);
					//buffer.Uniforms.emplace_back(name, uniformType, size, offset);
				}
#endif
				glCreateBuffers(1, &buffer.RendererID);
				glBindBuffer(GL_UNIFORM_BUFFER, buffer.RendererID);
				glBufferData(GL_UNIFORM_BUFFER, buffer.Size, nullptr, GL_DYNAMIC_DRAW);
				glBindBufferBase(GL_UNIFORM_BUFFER, buffer.BindingPoint, buffer.RendererID);

				LOG_DBG("Created Uniform Buffer at binding point %d with name '%s', size is %d bytes\n", buffer.BindingPoint, buffer.Name.c_str(), buffer.Size);

				glBindBuffer(GL_UNIFORM_BUFFER, 0);
			}
		}

		int32_t sampler = 0;
		for (const spirv_cross::Resource& resource : res.sampled_images)
		{
			auto& type = comp.get_type(resource.base_type_id);
			auto binding = comp.get_decoration(resource.id, spv::DecorationBinding);
			const auto& name = resource.name;
			uint32_t dimension = type.image.dim;

			GLint location = glGetUniformLocation(m_RendererID, name.c_str());
			if (location == -1) {
				//assert(false);
			}
			m_Resources[name] = ShaderResourceDeclaration(name, binding, 1);
			glUniform1i(location, binding);
		}
	}

    void GLShader::Load(const std::string& source, bool forceRecompile)
	{
		m_ShaderSource = PreProcess(source);
		if (!m_IsCompute)
		{
			//Parse();

			std::array<std::vector<uint32_t>, 2> vulkanBinaries;
			CompileOrGetVulkanBinary(vulkanBinaries, forceRecompile);
			CompileOrGetOpenGLBinary(vulkanBinaries, forceRecompile);
		}
		else
		{
			Renderer::Submit([=]()
			{
				if (m_RendererID)
					glDeleteProgram(m_RendererID);

				CompileAndUploadShader();
				if (!m_IsCompute)
				{
					// ResolveUniforms();
					// ValidateUniforms();
				}

				if (m_Loaded)
				{
					for (auto& callback : m_ShaderReloadedCallbacks)
						callback();
				}

				m_Loaded = true;
			});
		}
	}

	void GLShader::CompileOrGetVulkanBinary(std::array<std::vector<uint32_t>, 2>& outputBinary, bool forceCompile)
	{
		// Vertex Shader
		if (!forceCompile)
		{
			std::filesystem::path p = m_AssetPath;
			auto path = p.parent_path() / "cached" / (p.filename().string() + ".cached_vulkan.vert");
			std::string cachedFilePath = path.string();

			FILE* f = fopen(cachedFilePath.c_str(), "rb");
			if (f)
			{
				fseek(f, 0, SEEK_END);
				uint64_t size = ftell(f);
				fseek(f, 0, SEEK_SET);
				outputBinary[0] = std::vector<uint32_t>(size / sizeof(uint32_t));
				fread(outputBinary[0].data(), sizeof(uint32_t), outputBinary[0].size(), f);
				std::fclose(f);
			}
		}

		// Fragment Shader
		if (!forceCompile)
		{
			std::filesystem::path p = m_AssetPath;
			auto path = p.parent_path() / "cached" / (p.filename().string() + ".cached_vulkan.frag");
			std::string cachedFilePath = path.string();

			FILE* f = fopen(cachedFilePath.c_str(), "rb");
			if (f)
			{
				fseek(f, 0, SEEK_END);
				uint64_t size = ftell(f);
				fseek(f, 0, SEEK_SET);
				outputBinary[1] = std::vector<uint32_t>(size / sizeof(uint32_t));
				fread(outputBinary[1].data(), sizeof(uint32_t), outputBinary[1].size(), f);
				std::fclose(f);
			}
		}

		if (outputBinary[0].size() == 0)
		{
			shaderc::Compiler compiler;
			shaderc::CompileOptions options;
			options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);

			const bool optimize = false;
			if (optimize)
				options.SetOptimizationLevel(shaderc_optimization_level_performance);

			// Vertex Shader
			{
				auto& shaderSource = m_ShaderSource.at(GL_VERTEX_SHADER);
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(shaderSource, shaderc_vertex_shader, m_AssetPath.c_str(), options);

				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					std::string err = module.GetErrorMessage();
					LOG_ERR("%s\n", err.c_str());
					assert(false);
				}

				const uint8_t* begin = (const uint8_t*)module.cbegin();
				const uint8_t* end = (const uint8_t*)module.cend();
				const ptrdiff_t size = end - begin;

				outputBinary[0] = std::vector<uint32_t>(module.cbegin(), module.cend());
			}

			{
				std::filesystem::path p = m_AssetPath;

				auto dir = p.parent_path() / "cached";
				if (!std::filesystem::exists(dir)) {
					std::filesystem::create_directory(dir);
				}

				auto path = dir / (p.filename().string() + ".cached_vulkan.vert");
				std::string cachedFilePath = path.string();
				if (std::filesystem::exists(cachedFilePath)) {
					std::filesystem::remove(cachedFilePath);
				}

				FILE* f = fopen(cachedFilePath.c_str(), "wb");
				fwrite(outputBinary[0].data(), sizeof(uint32_t), outputBinary[0].size(), f);
				fclose(f);
			}
		}

		if (outputBinary[1].size() == 0)
		{
			shaderc::Compiler compiler;
			shaderc::CompileOptions options;
			options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);

			const bool optimize = false;
			if (optimize)
				options.SetOptimizationLevel(shaderc_optimization_level_performance);

			// Fragment Shader
			{
				auto& shaderSource = m_ShaderSource.at(GL_FRAGMENT_SHADER);
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(shaderSource, shaderc_fragment_shader, m_AssetPath.c_str(), options);

				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					std::vector<std::string> errs;
					uint32_t numErr = module.GetNumErrors();
					for (uint32_t i = 0; i < numErr; i++) {
						std::string err = module.GetErrorMessage();
						errs.push_back(err);
					}
					LOG_ERR("%s\n", errs[0].c_str());
					assert(false);
				}

				const uint8_t* begin = (const uint8_t*)module.cbegin();
				const uint8_t* end = (const uint8_t*)module.cend();
				const ptrdiff_t size = end - begin;

				outputBinary[1] = std::vector<uint32_t>(module.cbegin(), module.cend());
			}

			{
				std::filesystem::path p = m_AssetPath;

				auto dir = p.parent_path() / "cached";
				if (!std::filesystem::exists(dir)) {
					std::filesystem::create_directory(dir);
				}

				auto path = p.parent_path() / "cached" / (p.filename().string() + ".cached_vulkan.frag");
				std::string cachedFilePath = path.string();
				
				if (std::filesystem::exists(cachedFilePath)) {
					std::filesystem::remove(cachedFilePath);
				}

				FILE* f = fopen(cachedFilePath.c_str(), "wb");
				fwrite(outputBinary[1].data(), sizeof(uint32_t), outputBinary[1].size(), f);
				fclose(f);
			}
		}
	}

    void GLShader::CompileOrGetOpenGLBinary(const std::array<std::vector<uint32_t>, 2>& vulkanBinaries, bool forceCompile)
	{
		if (m_RendererID) {
			glDeleteProgram(m_RendererID);
        }

		const auto& vertexBinary = vulkanBinaries[0];
		const auto& fragmentBinary = vulkanBinaries[1];

		GLuint program = glCreateProgram();
		m_RendererID = program;

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);

		std::array<GLuint, 2> shaderRendererIDs = { 0, 0 };
		std::vector<uint32_t> shaderData[2]; // vertex + fragment for now

		{
			spirv_cross::CompilerGLSL glsl(vertexBinary);
			ParseConstantBuffers(glsl);

			std::filesystem::path p = m_AssetPath;
			auto path = p.parent_path() / "cached" / (p.filename().string() + ".cached_opengl.vert");
			std::string cachedFilePath = path.string();

			if (forceCompile) {
				if (std::filesystem::exists(cachedFilePath)) {
					std::filesystem::remove(cachedFilePath);
				}
			}

			FILE* f = fopen(cachedFilePath.c_str(), "rb");
			if (f)
			{
				fseek(f, 0, SEEK_END);
				uint64_t size = ftell(f);
				fseek(f, 0, SEEK_SET);
				shaderData[0] = std::vector<uint32_t>(size / sizeof(uint32_t));
				fread(shaderData[0].data(), sizeof(uint32_t), shaderData[0].size(), f);
				fclose(f);
			}
			else
			{
				std::string source = glsl.compile();
#if 0
				printf("=========================================\n");
				printf("Vertex Shader:\n%s\n", source.c_str());
				printf("=========================================\n");
#endif
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, shaderc_vertex_shader, m_AssetPath.c_str(), options);

				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					LOG_ERR("%s\n", module.GetErrorMessage().c_str());
					assert(false);
				}

				shaderData[0] = std::vector<uint32_t>(module.cbegin(), module.cend());

				{
					std::filesystem::path p = m_AssetPath;
					auto path = p.parent_path() / "cached" / (p.filename().string() + ".cached_opengl.vert");
					std::string cachedFilePath = path.string();
					FILE* f = fopen(cachedFilePath.c_str(), "wb");
					fwrite(shaderData[0].data(), sizeof(uint32_t), shaderData[0].size(), f);
					fclose(f);
				}
			}

			GLuint shaderID = glCreateShader(GL_VERTEX_SHADER);
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, shaderData[0].data(), shaderData[0].size() * sizeof(uint32_t));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderID);

			shaderRendererIDs[0] = shaderID;
		}

		{
			spirv_cross::CompilerGLSL glsl(fragmentBinary);
			ParseConstantBuffers(glsl);

			std::filesystem::path p = m_AssetPath;
			auto path = p.parent_path() / "cached" / (p.filename().string() + ".cached_opengl.frag");
			std::string cachedFilePath = path.string();

			if (forceCompile) {
				if (std::filesystem::exists(cachedFilePath)) {
					std::filesystem::remove(cachedFilePath);
				}
			}

			FILE* f = fopen(cachedFilePath.c_str(), "rb");
			if (f)
			{
				fseek(f, 0, SEEK_END);
				uint64_t size = ftell(f);
				fseek(f, 0, SEEK_SET);
				shaderData[1] = std::vector<uint32_t>(size / sizeof(uint32_t));
				fread(shaderData[1].data(), sizeof(uint32_t), shaderData[1].size(), f);
				fclose(f);
			}
			else
			{
				std::string source = glsl.compile();
#if PRINT_SHADERS
				printf("=========================================\n");
				printf("Fragment Shader:\n%s\n", source.c_str());
				printf("=========================================\n");
#endif
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, shaderc_fragment_shader, m_AssetPath.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					LOG_ERR("%s\n", module.GetErrorMessage().c_str());
					assert(false);
				}

				shaderData[1] = std::vector<uint32_t>(module.cbegin(), module.cend());

				{
					std::filesystem::path p = m_AssetPath;
					auto path = p.parent_path() / "cached" / (p.filename().string() + ".cached_opengl.frag");
					std::string cachedFilePath = path.string();

					FILE* f = fopen(cachedFilePath.c_str(), "wb");
					fwrite(shaderData[1].data(), sizeof(uint32_t), shaderData[1].size(), f);
					fclose(f);
				}
			}

			GLuint shaderID = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, shaderData[1].data(), shaderData[1].size() * sizeof(uint32_t));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderID);

			shaderRendererIDs[1] = shaderID;
		}

		// Link shader program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
			LOG_ERR("Shader compilation failed (%s):\n%s", m_AssetPath.c_str(), &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto id : shaderRendererIDs)
				glDeleteShader(id);
		}

		// Always detach shaders after a successful link.
        
		// for (auto id : shaderRendererIDs)
		// 	glDetachShader(program, id);

		// Get uniform locations
		glUseProgram(m_RendererID);
		for (auto& [name, buffer] : m_Buffers)
		{
			for (auto& [nameU, uniform] : buffer.Uniforms)
			{
				const auto& name = uniform.GetName();
				GLint location = glGetUniformLocation(m_RendererID, nameU.c_str());
				if (location == -1)
					LOG_WRN("%s: could not find uniform location", nameU.c_str());

				m_UniformLocations[nameU] = location;
			}
		}

		Reflect(shaderData[0]);
		Reflect(shaderData[1]);
	}

    void GLShader::AddShaderReloadedCallback(const ShaderReloadedCallback& callback)
	{
		m_ShaderReloadedCallbacks.push_back(callback);
	}

    std::string GLShader::ReadShaderFromFile(const std::string& filepath) const
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
			assert(false, "Could not load shader!");
		}
		in.close();
		return result;
	}

    std::unordered_map<GLenum, std::string> GLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			assert(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			assert(type == "vertex" || type == "fragment" || type == "pixel" || type == "compute", "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			auto shaderType = ShaderTypeFromString(type);
			shaderSources[shaderType] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));

			// Compute shaders cannot contain other types
			if (shaderType == GL_COMPUTE_SHADER)
			{
				m_IsCompute = true;
				break;
			}
		}

		return shaderSources;
	}

	// Parsing helper functions
	const char* FindToken(const char* str, const std::string& token)
	{
		const char* t = str;
		while (t = strstr(t, token.c_str()))
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

	static bool IsTypeStringResource(const std::string& type)
	{
		if (type == "sampler2D")		return true;
		if (type == "sampler2DMS")		return true;
		if (type == "samplerCube")		return true;
		if (type == "sampler2DShadow")	return true;
		return false;
	}

	int32_t GLShader::GetUniformLocation(const std::string& name) const
	{
		int32_t result = glGetUniformLocation(m_RendererID, name.c_str());
		if (result == -1)
			LOG_WRN("Could not find uniform '%s' in shader", name.c_str());

		return result;
	}

	GLenum GLShader::ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;
		if (type == "compute")
			return GL_COMPUTE_SHADER;

		return GL_NONE;
	}

	void GLShader::CompileAndUploadShader()
	{
		std::vector<GLuint> shaderRendererIDs;

		GLuint program = glCreateProgram();
		for (auto& kv : m_ShaderSource)
		{
			GLenum type = kv.first;
			std::string& source = kv.second;

			GLuint shaderRendererID = glCreateShader(type);
			const GLchar *sourceCstr = (const GLchar *)source.c_str();
			glShaderSource(shaderRendererID, 1, &sourceCstr, 0);

			glCompileShader(shaderRendererID);

			GLint isCompiled = 0;
			glGetShaderiv(shaderRendererID, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shaderRendererID, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shaderRendererID, maxLength, &maxLength, &infoLog[0]);

				LOG_ERR("Shader compilation failed:\n%s\n", &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shaderRendererID);

				assert(false, "Failed");
			}

			shaderRendererIDs.push_back(shaderRendererID);
			glAttachShader(program, shaderRendererID);
		}

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
			LOG_ERR("Shader compilation failed:\n%s\n", &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto id : shaderRendererIDs)
				glDeleteShader(id);
		}

		// Always detach shaders after a successful link.
		// for (auto id : shaderRendererIDs)
		// 	glDetachShader(program, id);

		m_RendererID = program;
	}

	void GLShader::SetUniformBuffer(const std::string& name, const void* data, uint32_t size)
	{
		uint8_t* buffer = new uint8_t[size];
		memcpy(buffer, data, size);

		// Renderer::Submit([=]()
		// {
			ShaderUniformBuffer* uniformBuffer = nullptr;
			for (auto& [bindingPoint, ub] : s_UniformBuffers)
			{
				if (ub.Name == name)
				{
					uniformBuffer = &ub;
					break;
				}
			}

			assert(uniformBuffer);
			assert(uniformBuffer->Size >= size);
			glNamedBufferSubData(uniformBuffer->RendererID, 0, size, buffer);

			delete[] buffer;
		// });
	}

    
    void GLShader::SetUniform(const std::string name, const int& value)
	{
        UploadUniformInt(name, value);
	}
    void GLShader::SetUniform(const std::string name, int* values, int count)
	{
        UploadUniformIntArray(name, values, count);
	}
    void GLShader::SetUniform(const std::string name, const float& value)
	{
        UploadUniformFloat(name, value);
	}
    void GLShader::SetUniform(const std::string name, const Vec2f& value)
	{
        UploadUniformFloat2(name, value);
	}
    void GLShader::SetUniform(const std::string name, const Vec3f& value)
	{
        UploadUniformFloat3(name, value);
	}
    void GLShader::SetUniform(const std::string name, const Vec4f& value)
	{
        UploadUniformFloat4(name, value);
	}
    void GLShader::SetUniform(const std::string name, const Mat3f& value)
	{
        UploadUniformMat3(name, value);
	}
    void GLShader::SetUniform(const std::string name, const Mat4f& value)
	{
        UploadUniformMat4(name, value);
	}

    void GLShader::UploadUniformInt(const std::string& name, int value)
	{
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}
    void GLShader::UploadUniformIntArray(const std::string& name, int* values, unsigned int count)
	{
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

    void GLShader::UploadUniformFloat(const std::string& name, float value)
	{
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}
    void GLShader::UploadUniformFloat2(const std::string& name, const Vec2f& value)
	{
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.X, value.Y);
	}
    void GLShader::UploadUniformFloat3(const std::string& name, const Vec3f& value)
	{
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.X, value.Y, value.Z);

	}
    void GLShader::UploadUniformFloat4(const std::string& name, const Vec4f& value)
	{
        
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.X, value.Y, value.Z, value.W);
	}

    void GLShader::UploadUniformMat3(const std::string& name, const Mat3f& matrix)
	{
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, matrix.elements);
	}
    void GLShader::UploadUniformMat4(const std::string& name, const Mat4f& matrix)
	{
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, matrix.elements);
	}

    const std::string& GLShader::GetName() const
	{
        return m_Name;
	}
}