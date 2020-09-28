#pragma once

#include <Graphics/Shader.h>
// #include <Core/NebulaCommon.h>

typedef uint32_t GLenum;

namespace Nebula
{
    class GLShader : public Shader
    {
    public:
        GLShader(const std::string filepath);
        GLShader(const std::string name, const std::string& vertexSrc, const std::string& fragmentSrc);
        virtual ~GLShader() override;

        virtual void Bind();
        virtual void Unbind();
        
        virtual void SetInt(const std::string name, int value);
        virtual void SetIntArray(const std::string name, int* values, int count);
        virtual void SetFloat(const std::string name, float value);
        virtual void SetFloat2(const std::string name, Vec2f value);
        virtual void SetFloat3(const std::string name, Vec3f value);
        virtual void SetFloat4(const std::string name, Vec4f value);
        virtual void SetMat3(const std::string name, Mat3f value);
        virtual void SetMat4(const std::string name, Mat4f value);

        virtual const std::string& GetName() const;

        void UploadUniformInt(const std::string& name, int value);
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const Vec2f& value);
		void UploadUniformFloat3(const std::string& name, const Vec3f& value);
		void UploadUniformFloat4(const std::string& name, const Vec4f& value);

		void UploadUniformMat3(const std::string& name, const Mat3f& matrix);
		void UploadUniformMat4(const std::string& name, const Mat4f& matrix);
    private:

        std::string ReadFile(const std::string& filepath);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& src);
        void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

        uint32_t ID;
        std::string name;

    };
}