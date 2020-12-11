#include "GLShader.h"

#include <fstream>
#include <glad/glad.h>

namespace Nebula
{
    static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		LOG_ERR("Unknown shader type!\n");
		return 0;
	}

    // GLShader::GLShader(const std::string filepath)
	// {
    //     std::string source = ReadFile(filepath);
    //     auto shaderSrcs = PreProcess(source);
    //     Compile(shaderSrcs);

    //     // Extract name from filepath
	// 	auto lastSlash = filepath.find_last_of("/\\");
	// 	lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
	// 	auto lastDot = filepath.rfind('.');
	// 	auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
	// 	name = filepath.substr(lastSlash, count);
	// }
	
	GLShader::GLShader(const std::string name, const std::string& shaderSrc)
	{
		auto shaderSrcs = PreProcess(shaderSrc);
        Compile(shaderSrcs);
		this->name = name;
	}

    GLShader::GLShader(const std::string name, const std::string& vertexSrc, const std::string& fragmentSrc)
        :   name(name)
    {
        std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

    GLShader::~GLShader()
	{
        glDeleteProgram(ID);
	}

    void GLShader::Bind()
	{
        glUseProgram(ID);
	}
    void GLShader::Unbind()
	{
        glUseProgram(0);
	}
	
	
    void GLShader::Reload(const std::string& input, bool inputIsFilePath)
	{
		glUseProgram(0);
		glDeleteProgram(ID);

		std::unordered_map<GLenum, std::string> srcs;
		if( inputIsFilePath)
		{
			srcs = PreProcess(ReadFile(input));
		}
		else
		{
			srcs = PreProcess(input);
		}
		

        Compile(srcs);
		this->name = name;
	}

    void GLShader::Reload(const std::string& vertexShader, const std::string& fragmentShader)
	{
        std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexShader;
		sources[GL_FRAGMENT_SHADER] = fragmentShader;
		Compile(sources);
	}


    std::string GLShader::ReadFile(const std::string& filepath)
    {
        std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = (size_t)in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			}
			else
			{
				LOG_ERR("Could not read from file '%s'", filepath.c_str());
			}
		}
		else
		{
			LOG_ERR("Could not open file '%s'", filepath.c_str());
		}

		return result;
    }

    std::unordered_map<GLenum, std::string> GLShader::PreProcess(const std::string& src)
    {
        std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = src.find(typeToken, 0); //Start of shader type declaration line
		while (pos != std::string::npos)
		{
			size_t eol = src.find_first_of("\r\n", pos); //End of shader type declaration line
			if(eol == std::string::npos) LOG_ERR("Syntax error\n");
			size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			std::string type = src.substr(begin, eol - begin);
			if(!ShaderTypeFromString(type)) LOG_ERR("Invalid shader type specified\n");

			size_t nextLinePos = src.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			if(nextLinePos == std::string::npos) LOG_ERR("Syntax error\n");
			pos = src.find(typeToken, nextLinePos); //Start of next shader type declaration line

			shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? src.substr(nextLinePos) : src.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
    }

    void GLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
    {
        GLuint program = glCreateProgram();
		if(shaderSources.size() > 2) LOG_ERR("We only support 2 shaders for now\n");
        GLenum glShaderIDs[2];
		int glShaderIDIndex = 0;
		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog = std::vector<GLchar>(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				LOG_ERR("%s", infoLog.data());
				LOG_ERR("Shader compilation failure!\n");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}
		
		ID = program;

		// Link our program
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

			// We don't need the program anymore.
			glDeleteProgram(program);
			
			for (auto id : glShaderIDs)
				glDeleteShader(id);

			LOG_ERR("%s\n", infoLog.data());
			LOG_ERR("Shader link failure!\n");
			return;
		}

		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}
    }

    
    void GLShader::SetInt(const std::string name, int value)
	{
        UploadUniformInt(name, value);
	}
    void GLShader::SetIntArray(const std::string name, int* values, int count)
	{
        UploadUniformIntArray(name, values, count);
	}
    void GLShader::SetFloat(const std::string name, float value)
	{
        UploadUniformFloat(name, value);
	}
    void GLShader::SetFloat2(const std::string name, Vec2f value)
	{
        UploadUniformFloat2(name, value);
	}
    void GLShader::SetFloat3(const std::string name, Vec3f value)
	{
        UploadUniformFloat3(name, value);
	}
    void GLShader::SetFloat4(const std::string name, Vec4f value)
	{
        UploadUniformFloat4(name, value);
	}
    void GLShader::SetMat3(const std::string name, Mat3f value)
	{
        UploadUniformMat3(name, value);
	}
    void GLShader::SetMat4(const std::string name, Mat4f value)
	{
        UploadUniformMat4(name, value);
	}

    void GLShader::UploadUniformInt(const std::string& name, int value)
	{
        GLint location = glGetUniformLocation(ID, name.c_str());
		glUniform1i(location, value);
	}
    void GLShader::UploadUniformIntArray(const std::string& name, int* values, unsigned int count)
	{
        GLint location = glGetUniformLocation(ID, name.c_str());
		glUniform1iv(location, count, values);
	}

    void GLShader::UploadUniformFloat(const std::string& name, float value)
	{
        GLint location = glGetUniformLocation(ID, name.c_str());
		glUniform1f(location, value);
	}
    void GLShader::UploadUniformFloat2(const std::string& name, const Vec2f& value)
	{
        GLint location = glGetUniformLocation(ID, name.c_str());
		glUniform2f(location, value.X, value.Y);
	}
    void GLShader::UploadUniformFloat3(const std::string& name, const Vec3f& value)
	{
        GLint location = glGetUniformLocation(ID, name.c_str());
		glUniform3f(location, value.X, value.Y, value.Z);

	}
    void GLShader::UploadUniformFloat4(const std::string& name, const Vec4f& value)
	{
        
        GLint location = glGetUniformLocation(ID, name.c_str());
		glUniform4f(location, value.X, value.Y, value.Z, value.W);
	}

    void GLShader::UploadUniformMat3(const std::string& name, const Mat3f& matrix)
	{
        GLint location = glGetUniformLocation(ID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, matrix.elements);
	}
    void GLShader::UploadUniformMat4(const std::string& name, const Mat4f& matrix)
	{
        GLint location = glGetUniformLocation(ID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, matrix.elements);
	}

    const std::string& GLShader::GetName() const
	{
        return name;
	}
}