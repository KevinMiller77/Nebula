#include "Mesh.h"

#include <Math/Math.h>

#include "imgui/imgui.h"

#include <Graphics/Renderer.h>
#include <Graphics/Buffer.h>

#include <filesystem>

#include <Graphics/Shader.h>
#include <Graphics/Pipeline.h>
#include <Graphics/Texture.h>
#include <Graphics/GraphicsContext.h>

namespace Nebula {

	Mesh::Mesh(const std::string& filename)
	{
		m_FilePath = filename;
		LOG_DBG("Loading meshRef: %s\n", filename.c_str());
	
        // Load file
        // Init Submeshes
        //      Init Verticies (Whether animated or not) (if animated, no AABB needed)
        //
        Ref<MeshFile> sceneMeshFile = Importer::s_ImportMesh(filename);
        Ref<iMeshScene> sceneRef = sceneMeshFile->meshScene;
		if (!sceneRef->HasMeshes()) {
			LOG_ERR("Failed to load meshRef file: %s\n", filename.c_str());
			assert((false));
		}

		m_Scene = sceneRef;

		m_IsAnimated = sceneRef->mNumAnimations != 0;
		m_MeshShader = m_IsAnimated ? Renderer::GetShaderLibrary()->Get("PBR_Anim") : Renderer::GetShaderLibrary()->Get("PBR_Static");
		m_BaseMaterial = CreateRef<Material>(m_MeshShader);

        Ref<iMeshRoot> rootRef = m_Scene->mRootNode;

        assert((rootRef->HasPositions(), "Meshes require positions."));
        //assert((rootRef->HasNormals(), "Meshes require normals."));

        std::vector<Vec3f> l_Positions = rootRef->mPositions;
        std::vector<Vec3f> l_Normals = rootRef->mNormals;
        std::vector<Vec3f> l_Tangents = rootRef->mTangents;
        std::vector<Vec3f> l_Bitangents = rootRef->mBitangents;
        std::vector<Vec2f> l_TexCoords = rootRef->mTextureCoords[0];

		uint32_t vertexCount = 0;
		uint32_t indexCount = 0;

		m_Submeshes.reserve(sceneRef->mNumMeshes);
		for (size_t m = 0; m < sceneRef->mNumMeshes; m++)
		{
			Ref<iMeshNode> meshRef = sceneRef->mMeshes[m];

			Submesh& submesh = m_Submeshes.emplace_back();
			submesh.BaseVertex = vertexCount;
			submesh.BaseIndex = indexCount;
			submesh.MaterialIndex = meshRef->mMaterialIndex;
			submesh.IndexCount = meshRef->mNumFaces * 3;
			submesh.MeshName = meshRef->mName;

			vertexCount += meshRef->mNumFaces * 3;
			indexCount += submesh.IndexCount;

			/* Vertices
			if (m_IsAnimated)
			{
				for (size_t i = 0; i < meshRef->mNumVertices; i++)
				{
					AnimatedVertex vertex;
					vertex.Position = { meshRef->mVertices[i].X, meshRef->mVertices[i].Y, meshRef->mVertices[i].Z };
					vertex.Normal = { meshRef->mNormals[i].X, meshRef->mNormals[i].Y, meshRef->mNormals[i].Z };

					if (meshRef->HasTangentsAndBitangents()) {
						vertex.Tangent = { meshRef->mTangents[i].X, meshRef->mTangents[i].Y, meshRef->mTangents[i].Z };
						vertex.Binormal = { meshRef->mBitangents[i].X, meshRef->mBitangents[i].Y, meshRef->mBitangents[i].Z };
					}

					if (meshRef->HasTextureCoords(0)) {
						vertex.TexCoord = meshRef->mTextureCoords[0][i];
                    }

					m_AnimatedVertices.push_back(vertex);
				}
			}
            */

			// Indices
            auto& aabb = submesh.BoundingBox;
            aabb.Min = { FLT_MAX, FLT_MAX, FLT_MAX };
            aabb.Max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
			for (size_t faceIdx = 0; faceIdx < meshRef->mNumFaces; faceIdx++)
			{
                uint32_t currentIndex = submesh.BaseIndex + faceIdx * 3; 

                Ref<iMeshFace> face = meshRef->mFaces[faceIdx];

                std::vector<uint32_t> i_Positions = face->mPositionIndices;
                std::vector<uint32_t> i_Normals = face->mVertexNormalIndices;
                std::vector<uint32_t> i_Tangents = face->mVertexTangentIndices;
                std::vector<uint32_t> i_Bitangent = face->mVertexBitangentIndices;
                std::vector<uint32_t> i_TexCoords = face->mTexCoordIndices;
                
                for (int vertexIdx = 0; vertexIdx < 3; vertexIdx++) {
                    Vertex vertex;
					vertex.Position = l_Positions[i_Positions[vertexIdx]];
					//vertex.Normal = l_Normals[i_Normals[vertexIdx]];
					aabb.Min.X = min(vertex.Position.X, aabb.Min.X);
					aabb.Min.Y = min(vertex.Position.Y, aabb.Min.Y);
					aabb.Min.Z = min(vertex.Position.Z, aabb.Min.Z);
					aabb.Max.X = max(vertex.Position.X, aabb.Max.X);
					aabb.Max.Y = max(vertex.Position.Y, aabb.Max.Y);
					aabb.Max.Z = max(vertex.Position.Z, aabb.Max.Z);

					if (i_Bitangent.size())
					{
						vertex.Tangent = l_Tangents[i_Bitangent[vertexIdx]];
						vertex.Binormal = l_Bitangents[i_Bitangent[vertexIdx]];
					}

					if (i_TexCoords.size())
						vertex.TexCoord = l_TexCoords[i_TexCoords[vertexIdx]];

					m_StaticVertices.push_back(vertex);
                }

				if (face->mNumIndices != 3) {
                    LOG_ERR("Must have 3 indices. Will be treated as if that is the case\n");
                } 
				Index index = { currentIndex, currentIndex + 1, currentIndex + 2 };
				m_Indices.push_back(index);

				//if (!m_IsAnimated)
				//	m_TriangleCache[m].emplace_back(m_StaticVertices[index.V1 + submesh.BaseVertex], m_StaticVertices[index.V2 + submesh.BaseVertex], m_StaticVertices[index.V3 + submesh.BaseVertex]);
			}
		}

		TraverseNodes();

		PipelineSpecification pipelineSpecification;
		auto shader = m_MeshShader;
		pipelineSpecification.m_Shader = shader;

		// Bones
		// if (m_IsAnimated)
		// {
		// 	for (size_t m = 0; m < sceneRef->mNumMeshes; m++)
		// 	{
		// 		iMeshNode meshRef = sceneRef->mMeshes[m];
		// 		Submesh& submesh = m_Submeshes[m];

		// 		for (size_t i = 0; i < meshRef->mNumBones; i++)
		// 		{
		// 			iMeshBone bone = meshRef->mBones[i];
		// 			std::string boneName(bone.mName);
		// 			int boneIndex = 0;

		// 			if (m_BoneMapping.find(boneName) == m_BoneMapping.end())
		// 			{
		// 				// Allocate an index for a new bone
		// 				boneIndex = m_BoneCount;
		// 				m_BoneCount++;
		// 				BoneInfo bi;
		// 				m_BoneInfo.push_back(bi);
		// 				m_BoneInfo[boneIndex].BoneOffset = bone.mOffsetMatrix;
		// 				m_BoneMapping[boneName] = boneIndex;
		// 			}
		// 			else
		// 			{
		// 				LOG_INF("Found existing bone in map\n");
		// 				boneIndex = m_BoneMapping[boneName];
		// 			}

		// 			for (size_t j = 0; j < bone.mNumWeights; j++)
		// 			{
		// 				int VertexID = submesh.BaseVertex + bone.mWeights[j].mVertexID;
		// 				float Weight = bone.mWeights[j].mWeight;
		// 				m_AnimatedVertices[VertexID].AddBoneData(boneIndex, Weight);
		// 			}
		// 		}
		// 	}
		// }

		// Materials
		if ( sceneRef->HasMaterials())
		{
			LOG_INF("---- Materials - %s ----\n", filename.c_str());

			m_Textures.resize(sceneRef->mNumMaterials);
			m_Materials.resize(sceneRef->mNumMaterials);
			for (uint32_t i = 0; i < sceneRef->mNumMaterials; i++)
			{
				Ref<iMeshMaterial> material = sceneRef->mMaterials[i];
				std::string materialName = material->GetName();

				auto mi = CreateRef<MaterialInstance>(m_BaseMaterial, materialName);
				m_Materials[i] = mi;

				LOG_INF("  %s (Index = %d)\n", materialName.c_str(), i);

				uint32_t textureCount = material->GetTextureCount(iMeshTextureType::DIFFUSE);
				LOG_INF("    TextureCount = %d\n", textureCount);

				Vec3f color(1.0f);
				material->Get(iMeshMaterialAttribType::COLOR_DIFFUSE, &color);

				float shininess = 80.0f;
				material->Get(iMeshMaterialAttribType::SHININESS, &shininess);

                float metalness = 0.0f;
				material->Get(iMeshMaterialAttribType::REFLECTIVITY, &metalness);

				float roughness = 1.0f - sqrt(shininess / 100.0f);
				LOG_INF("    COLOR = %f, %f, %f\n", color.X, color.Y, color.Z);
				LOG_INF("    ROUGHNESS = %f\n", &roughness);
                std::string texturePath;
				bool hasAlbedoMap = material->GetTexture(iMeshTextureType::DIFFUSE, 0, &texturePath);
				if (hasAlbedoMap)
				{
					Ref<Texture2D> texture = Texture2D::Create(texturePath);
					if (texture->IsValid())
					{
						m_Textures[i] = texture;
                        mi->Set("u_AlbedoTexture", m_Textures[i]);
						// TODO!!!!!!!!!!!!!! mi->Set("u_MaterialUniforms.AlbedoTexToggle", 1.0f);
					}
					else
					{
						LOG_ERR("Could not load texture: %s\n", texturePath.c_str());
						// Fallback to albedo color
						//mi->Set("u_MaterialUniforms.AlbedoColor", glm::vec3{ aiColor.r, aiColor.g, aiColor.b });
					}
				}
				else
				{
					//mi->Set("u_MaterialUniforms.AlbedoColor", glm::vec3 { aiColor.r, aiColor.g, aiColor.b });
					LOG_INF("    No albedo map\n");
				}

				// Normal maps
				// mi->Set("u_MaterialUniforms.NormalTexToggle", 0.0f);
				// if (material->GetTexture(iMeshTextureType::NORMAL, 0, &texturePath))
				// {
				// 	LOG_INF("    Normal map path = %s\n", texturePath.c_str());
				// 	auto texture = Texture2D::Create(texturePath);
				// 	if (texture->IsValid())
				// 	{
				// 		m_Textures.push_back(texture);
				// 		mi->Set("u_NormalTexture", texture);
				// 		mi->Set("u_MaterialUniforms.NormalTexToggle", 1.0f);
				// 	}
				// 	else
				// 	{
				// 		LOG_ERR("   Could not load texture: %s\n", texturePath.c_str());
				// 	}
				// }
				// else
				// {
				// 	LOG_ERR("    No normal map\n");
				// }

				// TODO: Roughness map
                // TODO: Metalness map
			}
			LOG_INF("------------------------\n");
		}

		
		if (m_IsAnimated)
		{
			m_VertexBuffer = VertexBuffer::Create((float*)m_AnimatedVertices.data(), m_AnimatedVertices.size() * sizeof(AnimatedVertex));
			m_VertexBufferLayout = {
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float3, "a_Normal" },
				{ ShaderDataType::Float3, "a_Tangent" },
				{ ShaderDataType::Float3, "a_Binormal" },
				{ ShaderDataType::Float2, "a_TexCoord" },
				{ ShaderDataType::Int4, "a_BoneIDs" },
				{ ShaderDataType::Float4, "a_BoneWeights" },
			};
			m_VertexBuffer->SetLayout(m_VertexBufferLayout);
		}
		else
		{
			m_VertexBuffer = VertexBuffer::Create((float*)m_StaticVertices.data(), m_StaticVertices.size() * sizeof(Vertex));
			m_VertexBufferLayout = {
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float3, "a_Normal" },
				{ ShaderDataType::Float3, "a_Tangent" },
				{ ShaderDataType::Float3, "a_Binormal" },
				{ ShaderDataType::Float2, "a_TexCoord" },
			};
			m_VertexBuffer->SetLayout(m_VertexBufferLayout);
		}
		pipelineSpecification.m_Layout = m_VertexBufferLayout;
				
		m_Pipeline = Pipeline::Create(pipelineSpecification);


		m_IndexBuffer = IndexBuffer::Create((uint32_t*)m_Indices.data(), m_Indices.size() * 3);

		m_IsLoaded = true;
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::OnUpdate(float ts)
	{
		if (!IsLoaded()) {
			return;
		}

		if (m_IsAnimated)
		{
			if (m_AnimationPlaying)
			{
				m_WorldTime += ts;

				float ticksPerSecond = (float)(m_Scene->mAnimations[0].mTicksPerSecond != 0 ? m_Scene->mAnimations[0].mTicksPerSecond : 25.0f) * m_TimeMultiplier;
				m_AnimationTime += ts * ticksPerSecond;
				m_AnimationTime = fmod(m_AnimationTime, (float)m_Scene->mAnimations[0].mDuration);
			}

			// TODO: We only need to recalc bones if rendering has been requested at the current animation frame
			BoneTransform(m_AnimationTime);
		}
	}

	static std::string LevelToSpaces(uint32_t level)
	{
		std::string result = "";
		for (uint32_t i = 0; i < level; i++)
			result += "--";
		return result;
	}

    void Mesh::TraverseNodes(Ref<iMeshNode> pNode, const Mat4f& parentTransform, uint32_t level) {

    }

	void Mesh::TraverseNodes()
	{
		for (uint32_t i = 0; i < m_Scene->mMeshes.size(); i++)
		{
            Mat4f transform = m_Scene->mMeshes[i]->mTransform;

			Ref<iMeshNode> meshRef = m_Scene->mMeshes[i];
            uint32_t idx = 0;
            for (int j = 0; j < m_Submeshes.size(); j++) {
                if (m_Submeshes[j].MeshName == meshRef->mName) {
                    idx = j;
                    break;
                }
            }
	
    		Submesh& submesh = m_Submeshes[idx];
			submesh.NodeName = m_Scene->mMeshes[i]->mName;
			submesh.Transform = transform;

		}
        for (uint32_t i = 0; i < m_Scene->mMeshes.size(); i++) {
            TraverseNodes(m_Scene->mMeshes[i], m_Scene->mMeshes[i]->mTransform, 0);
        }

	}

	uint32_t Mesh::FindPosition(float AnimationTime, const iNodeAnimation& pNodeAnim)
	{
		assert((pNodeAnim.mPositionKeys.size() > 0));
		for (uint32_t i = 0; i < pNodeAnim.mPositionKeys.size() - 1; i++)
		{
			if (AnimationTime < (float)pNodeAnim.mPositionKeys[i + 1].mKeyTime)
				return i;
		}

		return 0;
	}


	uint32_t Mesh::FindRotation(float AnimationTime, const iNodeAnimation& pNodeAnim)
	{
		assert((pNodeAnim.mRotationKeys.size() > 0));

		for (uint32_t i = 0; i < pNodeAnim.mRotationKeys.size() - 1; i++)
		{
			if (AnimationTime < (float)pNodeAnim.mRotationKeys[i + 1].mKeyTime)
				return i;
		}

		return 0;
	}


	uint32_t Mesh::FindScaling(float AnimationTime, const iNodeAnimation& pNodeAnim)
	{
		assert((pNodeAnim.mScaleKeys.size() > 0));

		for (uint32_t i = 0; i < pNodeAnim.mScaleKeys.size() - 1; i++)
		{
			if (AnimationTime < (float)pNodeAnim.mScaleKeys[i + 1].mKeyTime)
				return i;
		}

		return 0;
	}


	Vec3f Mesh::InterpolateTranslation(float animationTime, const iNodeAnimation& nodeAnim)
	{
		if (nodeAnim.mPositionKeys.size() == 1)
		{
			// No interpolation necessary for single value
			Vec3f v = nodeAnim.mPositionKeys[0].mPosition;
			return { v.X, v.Y, v.Z };
		}

		uint32_t PositionIndex = FindPosition(animationTime, nodeAnim);
		uint32_t NextPositionIndex = (PositionIndex + 1);
		if(NextPositionIndex >= nodeAnim.mPositionKeys.size());
		
        float DeltaTime = (float)(nodeAnim.mPositionKeys[NextPositionIndex].mKeyTime - nodeAnim.mPositionKeys[PositionIndex].mKeyTime);
		
        float Factor = (animationTime - (float)nodeAnim.mPositionKeys[PositionIndex].mKeyTime) / DeltaTime;
		assert((Factor <= 1.0f, "Factor must be below 1.0f"));
		Factor = clamp(Factor, 0.0f, 1.0f);
		
        const Vec3f& Start = nodeAnim.mPositionKeys[PositionIndex].mPosition;
		const Vec3f& End = nodeAnim.mPositionKeys[NextPositionIndex].mPosition;
		Vec3f Delta = End - Start;
        
        return Start + Factor * Delta;
    }


	Vec3f Mesh::InterpolateRotation(float animationTime, const iNodeAnimation& nodeAnim)
	{
		if (nodeAnim.mPositionKeys.size() == 1)
		{
			// No interpolation necessary for single value
			Vec3f v = nodeAnim.mRotationKeys[0].mRotation;
			return { v.X, v.Y, v.Z };
		}

		uint32_t RotationIndex = FindRotation(animationTime, nodeAnim);
		uint32_t NextRotationIndex = (RotationIndex + 1);
		assert((NextRotationIndex < nodeAnim.mRotationKeys.size()));
		
        float DeltaTime = (float)(nodeAnim.mPositionKeys[NextRotationIndex].mKeyTime - nodeAnim.mRotationKeys[RotationIndex].mKeyTime);
		
        float Factor = (animationTime - (float)nodeAnim.mRotationKeys[RotationIndex].mKeyTime) / DeltaTime;
		assert((Factor <= 1.0f, "Factor must be below 1.0f"));
		Factor = clamp(Factor, 0.0f, 1.0f);
		
        const Vec3f& Start = nodeAnim.mRotationKeys[RotationIndex].mRotation;
		const Vec3f& End = nodeAnim.mRotationKeys[NextRotationIndex].mRotation;
		Vec3f Delta = End - Start;

		return Start + Factor * Delta;
	}    

	Vec3f Mesh::InterpolateScale(float animationTime, const iNodeAnimation& nodeAnim)
	{
		if (nodeAnim.mScaleKeys.size() == 1)
		{
			// No interpolation necessary for single value
			Vec3f v = nodeAnim.mScaleKeys[0].mScale;
			return v;
		}

		uint32_t index = FindScaling(animationTime, nodeAnim);
		uint32_t nextIndex = (index + 1);
		
        assert((nextIndex < nodeAnim.mScaleKeys.size()));
		float deltaTime = (float)(nodeAnim.mScaleKeys[nextIndex].mKeyTime - nodeAnim.mScaleKeys[index].mKeyTime);
		float factor = (animationTime - (float)nodeAnim.mScaleKeys[index].mKeyTime) / deltaTime;
		
        assert((factor <= 1.0f, "Factor must be below 1.0f"));
		
        factor = clamp(factor, 0.0f, 1.0f);
		const auto& start = nodeAnim.mScaleKeys[index].mScale;
		const auto& end = nodeAnim.mScaleKeys[nextIndex].mScale;
		
        auto delta = end - start;
		auto endVec = start + factor * delta;

		return endVec;
	}

	void Mesh::ReadNodeHierarchy(float AnimationTime, Ref<iMeshNode> pNode, const Mat4f& parentTransform)
	{
		std::string name = pNode->mName;
		const iSceneAnimation animation = m_Scene->mAnimations[0];
		Mat4f nodeTransform(pNode->mTransform);
		const iNodeAnimation* nodeAnim = FindNodeAnim(animation, name);

		if (nodeAnim)
		{
			Vec3f translation = InterpolateTranslation(AnimationTime, *nodeAnim);
			Mat4f translationMatrix = Mat4f(1.0f).translation(translation);

			Vec3f rotation = InterpolateRotation(AnimationTime, *nodeAnim);
			Mat4f rotationMatrix = Mat4f::rotation(rotation);

			Vec3f scale = InterpolateScale(AnimationTime, *nodeAnim);
			Mat4f scaleMatrix = Mat4f::scale(scale);

			nodeTransform = translationMatrix * rotationMatrix * scaleMatrix;
		}

		Mat4f transform = parentTransform * nodeTransform;

		if (m_BoneMapping.find(name) != m_BoneMapping.end())
		{
			uint32_t BoneIndex = m_BoneMapping[name];
			m_BoneInfo[BoneIndex].EndTransform = m_InverseTransform * transform * m_BoneInfo[BoneIndex].BoneOffset;
		}

		for (uint32_t i = 0; i < pNode->mChildren.size(); i++)
			ReadNodeHierarchy(AnimationTime, pNode->mChildren[i], transform);
	}

    void Mesh::ReadNodeHierarchy(float AnimationTime, const Mat4f& parentTransform) {
        for (Ref<iMeshNode> node : m_Scene->mMeshes) {
            ReadNodeHierarchy(AnimationTime, node, Mat4f(1.0f));
        }
    }

	const iNodeAnimation* Mesh::FindNodeAnim(const iSceneAnimation animation, const std::string& nodeName)
	{
		for (uint32_t i = 0; i < animation.mNumChannels; i++)
		{
			iNodeAnimation* nodeAnim = new iNodeAnimation();
            *nodeAnim = animation.mChannels[i];
			if (nodeAnim->mName == nodeName) {
				return nodeAnim;
            }
            
            delete nodeAnim;
		}
		return nullptr;
	} 

	void Mesh::BoneTransform(float time)
	{
		ReadNodeHierarchy(time, Mat4f(1.0f));
		m_BoneTransforms.resize(m_BoneCount);
		for (size_t i = 0; i < m_BoneCount; i++)
			m_BoneTransforms[i] = m_BoneInfo[i].EndTransform;
	}

	void Mesh::DumpVertexBuffer()
	{
		if(!IsLoaded()) {
			return;
		}
		// TODO: Convert to ImGui
		LOG_INF("------------------------------------------------------\n");
		LOG_INF("Vertex Buffer Dump\n");
		LOG_INF("Mesh: %s\n", m_FilePath.c_str());
		if (m_IsAnimated)
		{
			for (size_t i = 0; i < m_AnimatedVertices.size(); i++)
			{
				auto& vertex = m_AnimatedVertices[i];
				LOG_INF("Vertex: %f", i);
				LOG_INF("Position: %f, %f, %f\n", vertex.Position.X, vertex.Position.Y, vertex.Position.Z);
				LOG_INF("Normal: %f, %f, %f\n", vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z);
				LOG_INF("Binormal: %f, %f, %f\n", vertex.Binormal.X, vertex.Binormal.Y, vertex.Binormal.Z);
				LOG_INF("Tangent: %f, %f, %f\n", vertex.Tangent.X, vertex.Tangent.Y, vertex.Tangent.Z);
				LOG_INF("TexCoord: %f, %f\n", vertex.TexCoord.X, vertex.TexCoord.Y);
				LOG_INF("--\n");
			}
		}
		else
		{
			for (size_t i = 0; i < m_StaticVertices.size(); i++)
			{
				auto& vertex = m_StaticVertices[i];
				LOG_INF("Vertex: %d\n", i);
				LOG_INF("Position: %f, %f, %f\n", vertex.Position.X, vertex.Position.Y, vertex.Position.Z);
				LOG_INF("Normal: %f, %f, %f\n", vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z);
				LOG_INF("Binormal: %f, %f, %f\n", vertex.Binormal.X, vertex.Binormal.Y, vertex.Binormal.Z);
				LOG_INF("Tangent: %f, %f, %f\n", vertex.Tangent.X, vertex.Tangent.Y, vertex.Tangent.Z);
				LOG_INF("TexCoord: %f, %f\n", vertex.TexCoord.X, vertex.TexCoord.Y);
				LOG_INF("--\n");
			}
		}
		LOG_INF("------------------------------------------------------\n");
	}
}