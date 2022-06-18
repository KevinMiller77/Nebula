#include "Material.h"

namespace Nebula {

	//////////////////////////////////////////////////////////////////////////////////
	// Material
	//////////////////////////////////////////////////////////////////////////////////

	Ref<Material> Material::Create(const Ref<Shader>& shader)
	{
		return CreateRef<Material>(shader);
	}

	Material::Material(const Ref<Shader>& shader)
		: m_Shader(shader)
	{
		AllocateStorage();

		m_MaterialFlags |= (uint32_t)MaterialFlag::DepthTest;
		m_MaterialFlags |= (uint32_t)MaterialFlag::Blend;
	}

	Material::~Material()
	{
	}

	void Material::AllocateStorage()
	{

		const auto& shaderBuffers = m_Shader->GetShaderBuffers();

        if (shaderBuffers.size() > 1) {
            LOG_ERR("We currently only support ONE material buffer!\n");
            return;
        }

		if (shaderBuffers.size() > 0)
		{
			const ShaderBuffer& buffer = (*shaderBuffers.begin()).second;
			m_UniformStorageBuffer.Allocate(buffer.Size);
			m_UniformStorageBuffer.ZeroInitialize();
		}
	}

	void Material::OnShaderReloaded()
	{
		AllocateStorage();
		
		for (auto mi : m_MaterialInstances)
			mi->OnShaderReloaded();
	}

	const ShaderUniform* Material::FindUniformDeclaration(const std::string& name)
	{
		const auto& shaderBuffers = m_Shader->GetShaderBuffers();

		if (shaderBuffers.size() > 1) {
            LOG_ERR("We currently only support ONE material buffer!\n");
            return nullptr;
        }
		if (shaderBuffers.size() > 0)
		{
			const ShaderBuffer& buffer = (*shaderBuffers.begin()).second;
			if (buffer.Uniforms.find(name) == buffer.Uniforms.end())
				return nullptr;

			return &buffer.Uniforms.at(name);
		}
		return nullptr;
	}

	const ShaderResourceDeclaration* Material::FindResourceDeclaration(const std::string& name)
	{
		auto& resources = m_Shader->GetResources();
		for (const auto& [n, resource] : resources)
		{
			if (resource.GetName() == name)
				return &resource;
		}
		return nullptr;
	}

	void Material::Bind()
	{
		auto& shader = m_Shader;
		shader->Bind();
		const auto& shaderBuffers = GetShader()->GetShaderBuffers();
		if (shaderBuffers.size() > 1) {
            LOG_ERR("We currently only support ONE material buffer!\n");
            return;
        }

		if (shaderBuffers.size() > 0)
		{
			const ShaderBuffer& buffer = (*shaderBuffers.begin()).second;

			for (auto& [name, uniform] : buffer.Uniforms)
			{
				switch (uniform.GetType())
				{
					//	None = 0, Bool, Int, Float, Vec2, Vec3, Vec4, Mat3, Mat4
				case ShaderUniformType::Bool:
				{
					bool value = m_UniformStorageBuffer.Read<bool>(uniform.GetOffset());
					shader->SetUniform(name, value);
					break;
				}
				case ShaderUniformType::Int:
				{
					int value = m_UniformStorageBuffer.Read<int>(uniform.GetOffset());
					shader->SetUniform(name, value);
					break;
				}
				case ShaderUniformType::Float:
				{
					float value = m_UniformStorageBuffer.Read<float>(uniform.GetOffset());
					shader->SetUniform(name, value);
					break;
				}
				case ShaderUniformType::Vec2:
				{
					const Vec2f& value = m_UniformStorageBuffer.Read<Vec2f>(uniform.GetOffset());
					shader->SetUniform(name, value);
					break;
				}
				case ShaderUniformType::Vec3:
				{
					const Vec3f& value = m_UniformStorageBuffer.Read<Vec3f>(uniform.GetOffset());
					shader->SetUniform(name, value);
					break;
				}
				case ShaderUniformType::Vec4:
				{
					const Vec4f& value = m_UniformStorageBuffer.Read<Vec4f>(uniform.GetOffset());
					shader->SetUniform(name, value);
					break;
				}
				case ShaderUniformType::Mat3:
				{
					const Mat3f& value = m_UniformStorageBuffer.Read<Mat3f>(uniform.GetOffset());
					shader->SetUniform(name, value);
					break;
				}
				case ShaderUniformType::Mat4:
				{
					const Mat4f& value = m_UniformStorageBuffer.Read<Mat4f>(uniform.GetOffset());
					shader->SetUniform(name, value);
					break;
				}
				}
			}
		}
		BindTextures();
	}

	void Material::BindTextures()
	{
		for (size_t i = 0; i < m_Textures.size(); i++)
		{
			auto& texture = m_Textures[i];
			if (texture)
				texture->Bind(i);
		}
	}

	//////////////////////////////////////////////////////////////////////////////////
	// MaterialInstance
	//////////////////////////////////////////////////////////////////////////////////

	Ref<MaterialInstance> MaterialInstance::Create(const Ref<Material>& material)
	{
		return CreateRef<MaterialInstance>(material);
	}

	MaterialInstance::MaterialInstance(const Ref<Material>& material, const std::string& name)
		: m_Material(material), m_Name(name)
	{
		m_Material->m_MaterialInstances.insert(this);
		AllocateStorage();
	}

	MaterialInstance::~MaterialInstance()
	{
		m_Material->m_MaterialInstances.erase(this);
	}

	void MaterialInstance::OnShaderReloaded()
	{
		AllocateStorage();
		m_OverriddenValues.clear();
	}

	void MaterialInstance::AllocateStorage()
	{
		const auto& shaderBuffers = GetShader()->GetShaderBuffers();

        if (shaderBuffers.size() < 1) {
		    LOG_ERR("We currently only support ONE material buffer!\n");
            return;
        }

		if (shaderBuffers.size() > 0)
		{
			const ShaderBuffer& buffer = (*shaderBuffers.begin()).second;
			m_UniformStorageBuffer.Allocate(buffer.Size);
			m_UniformStorageBuffer.ZeroInitialize();
		}
	}

	void MaterialInstance::SetFlag(MaterialFlag flag, bool value)
	{
		if (value)
		{
			m_Material->m_MaterialFlags |= (uint32_t)flag;
		}
		else
		{
			m_Material->m_MaterialFlags &= ~(uint32_t)flag;
		}
	}

	void MaterialInstance::OnMaterialValueUpdated(const ShaderUniform& decl)
	{
		if (m_OverriddenValues.find(decl.GetName()) == m_OverriddenValues.end())
		{
			auto& buffer = m_UniformStorageBuffer;
			auto& materialBuffer = m_Material->m_UniformStorageBuffer;
			buffer.Write(materialBuffer.Data + decl.GetOffset(), decl.GetSize(), decl.GetOffset());
		}
	}

	void MaterialInstance::Bind()
	{
		auto& shader = m_Material->m_Shader;
		shader->Bind();

		const auto& shaderBuffers = GetShader()->GetShaderBuffers();
		if (shaderBuffers.size() < 1) {
		    LOG_ERR("We currently only support ONE material buffer!\n");
            return;
        }

		if (shaderBuffers.size() > 0)
		{
			const ShaderBuffer& buffer = (*shaderBuffers.begin()).second;

			for (auto& [name, uniform] : buffer.Uniforms)
			{
				switch (uniform.GetType())
				{
					//	None = 0, Bool, Int, Float, Vec2, Vec3, Vec4, Mat3, Mat4
				case ShaderUniformType::Bool:
				{
					bool value = m_UniformStorageBuffer.Read<bool>(uniform.GetOffset());
					shader->SetUniform(name, value);
					break;
				}
				case ShaderUniformType::Int:
				{
					int value = m_UniformStorageBuffer.Read<int>(uniform.GetOffset());
					shader->SetUniform(name, value);
					break;
				}
				case ShaderUniformType::Float:
				{
					float value = m_UniformStorageBuffer.Read<float>(uniform.GetOffset());
					shader->SetUniform(name, value);
					break;
				}
				case ShaderUniformType::Vec2:
				{
					const Vec2f& value = m_UniformStorageBuffer.Read<Vec2f>(uniform.GetOffset());
					shader->SetUniform(name, value);
					break;
				}
				case ShaderUniformType::Vec3:
				{
					const Vec3f& value = m_UniformStorageBuffer.Read<Vec3f>(uniform.GetOffset());
					shader->SetUniform(name, value);
					break;
				}
				case ShaderUniformType::Vec4:
				{
					const Vec4f& value = m_UniformStorageBuffer.Read<Vec4f>(uniform.GetOffset());
					shader->SetUniform(name, value);
					break;
				}
				case ShaderUniformType::Mat3:
				{
					const Mat3f& value = m_UniformStorageBuffer.Read<Mat3f>(uniform.GetOffset());
					shader->SetUniform(name, value);
					break;
				}
				case ShaderUniformType::Mat4:
				{
					const Mat4f& value = m_UniformStorageBuffer.Read<Mat4f>(uniform.GetOffset());
					shader->SetUniform(name, value);
					break;
				}
				}
			}
		}

		m_Material->BindTextures();
		for (size_t i = 0; i < m_Textures.size(); i++)
		{
			auto& texture = m_Textures[i];
			if (texture)
				texture->Bind(i);
		}
	}

}