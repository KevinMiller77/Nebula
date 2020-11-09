#pragma once

#include <string>
#include <unordered_map>
#include <Math/math.h>
#include <Utils/Logging.h>
#include <Core/Ref.h>

namespace Nebula
{
    class Shader
    {
    public:
        virtual ~Shader() {};

        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        
        virtual void SetInt(const std::string name, int value) = 0;
        virtual void SetIntArray(const std::string name, int* values, int count) = 0;
        virtual void SetFloat(const std::string name, float value) = 0;
        virtual void SetFloat3(const std::string name, Vec3f value) = 0;
        virtual void SetFloat4(const std::string name, Vec4f value) = 0;
        virtual void SetMat4(const std::string name, Mat4f value) = 0;

        virtual const std::string& GetName() const = 0;

        static Ref<Shader> Create(const std::string& filepath);
        static Ref<Shader> Create(const std::string name, const std::string& vertexSrc, const std::string& fragmentSrc);
        static Ref<Shader> Create(const std::string name, const std::string& shaderSrc);

    };

    class ShaderLibrary
    {
    public:
        void Add(const std::string& name, Ref<Shader> shader);
        void Add(Ref<Shader> shader);

        Ref<Shader> Load(const std::string& filepath);
        Ref<Shader> Load(const std::string name, const std::string& filepath);

        Ref<Shader> Get(std::string& name);

        bool Exists(const std::string name);
    private:
        std::unordered_map<std::string, Ref<Shader>> shaders;
    
    };
}