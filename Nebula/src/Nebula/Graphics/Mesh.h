#pragma once

#include <Math/math.h>

#include "Pipeline.h"
#include "Buffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include <Importer/Importer.h>

namespace Nebula {

    struct Vertex {
        Vec3f Position;
        Vec3f Normal;
        Vec3f Tangent;
        Vec3f Binormal;
        Vec2f TexCoord;
    };

    struct AnimatedVertex {
        Vec3f Position;
        Vec3f Normal;
        Vec3f Tangent;
        Vec3f Binormal;
        Vec2f TexCoord;

        uint32_t IDs[4] = {0, 0, 0, 0};
        float Weights[4] = {0.0f, 0.0f, 0.0f, 0.0f};

        void AddBoneData(uint32_t boneID, float weight) {
            for (size_t i = 0; i < 4; i++) {
                IDs[i] = boneID;
                Weights[i] = weight;
                return;
            }

            LOG_ERR("Animated Vertex created with more than 4 bones! This is not supported!\n");
        }
    };

    static const int NumAttributes = 5;

    struct Index {
        uint32_t V1, V2, V3;
    };

    struct BoneInfo {
        Mat4f BoneOffset;
        Mat4f EndTransform;
    };

    struct VertexBoneData {
        uint32_t IDs[4];
        float Weights[4];

        VertexBoneData() {
            memset(IDs, 0, sizeof(IDs));
            memset(Weights, 0, sizeof(Weights));
        }

        void AddBoneData(uint32_t boneID, float weight) {
            for (size_t i = 0; i < 4; i++) {
                if (Weights[i] == 0.0f) {
                    IDs[i] = boneID;
                    Weights[i] = weight;
                    return;
                }
            }

            LOG_ERR("Too many bones inside VertexBoneData!\n");
        }
    };

    struct Triangle {
        Vertex V0, V1, V2;

        Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2) 
            : V0(v0), V1(v1), V2(v2) {}
    };

    struct Submesh {
        uint32_t BaseVertex;
        uint32_t BaseIndex;
        uint32_t MaterialIndex;
        uint32_t IndexCount;

        Mat4f Transform;
        AABB BoundingBox;

        std::string NodeName, MeshName;
    };

    class Mesh {
    public:
        Mesh(const std::string& filepath);
        ~Mesh();

        void OnUpdate(float ts);
        void DumpVertexBuffer();

        std::vector<Submesh>& GetSubmeshes() { return m_Submeshes; }
        const std::vector<Submesh>& GetSubmeshes() const { return m_Submeshes; }

        Ref<Shader> GetMeshShader() { return m_MeshShader; }
        Ref<Material> GetMeshMaterial() { return m_BaseMaterial; }
        std::vector<Ref<MaterialInstance>> GetMaterials() { return m_Materials; }
		const std::vector<Ref<Texture2D>>& GetTextures() const { return m_Textures; }
		const std::string& GetFilePath() const { return m_FilePath; }

		const std::vector<Triangle> GetTriangleCache(uint32_t index) const { return m_TriangleCache.at(index); }

		Ref<VertexBuffer> GetVertexBuffer() { return m_VertexBuffer; }
		Ref<IndexBuffer> GetIndexBuffer() { return m_IndexBuffer; }
		const BufferLayout& GetVertexBufferLayout() const { return m_VertexBufferLayout; }

    private:
        void BoneTransform(float time);
        
        void ReadNodeHierarchy(float animationTime, const Mat4f& parentTransform);
        void ReadNodeHierarchy(float animationTime, Ref<iMeshNode> node, const Mat4f& parentTransform);

        void TraverseNodes();
        void TraverseNodes(Ref<iMeshNode> pNode, const Mat4f& parentTransform = Mat4f(1.0), uint32_t level = 0);

        const iNodeAnimation* FindNodeAnim(const iSceneAnimation animation, const std::string& nodeName);
		uint32_t FindPosition(float AnimationTime, const iNodeAnimation& pNodeAnim);
		uint32_t FindRotation(float AnimationTime, const iNodeAnimation& pNodeAnim);
		uint32_t FindScaling(float AnimationTime, const iNodeAnimation&  pNodeAnim);
		Vec3f InterpolateTranslation(float animationTime, const iNodeAnimation& nodeAnim);
		Vec3f InterpolateRotation(float animationTime, const iNodeAnimation& nodeAnim);
		Vec3f InterpolateScale(float animationTime, const iNodeAnimation& nodeAnim);
        
    private:
        Ref<iMeshScene> m_Scene;

        std::vector<Submesh> m_Submeshes;
        
        Mat4f m_InverseTransform = Mat4f(1.0f);

        uint32_t m_BoneCount = 0;
        std::vector<BoneInfo> m_BoneInfo;

        Ref<Pipeline> m_Pipeline;
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;
        BufferLayout m_VertexBufferLayout;

        std::vector<Vertex> m_StaticVertices;
        std::vector<AnimatedVertex> m_AnimatedVertices;
        std::vector<Index> m_Indices;
        std::unordered_map<std::string, uint32_t> m_BoneMapping;
        std::vector<Mat4f> m_BoneTransforms;

        
		Ref<Shader> m_MeshShader;
		Ref<Material> m_BaseMaterial;
		std::vector<Ref<Texture2D>> m_Textures;
		std::vector<Ref<Texture2D>> m_NormalMaps;
		std::vector<Ref<MaterialInstance>> m_Materials;


        std::unordered_map<uint32_t, std::vector<Triangle>> m_TriangleCache;

        bool m_IsAnimated = false;
        float m_AnimationTime = 0.0f;
        float m_WorldTime = 0.0f;
        float m_TimeMultiplier = 1.0f;
        bool m_AnimationPlaying = true;

        std::string m_FilePath;

        friend class Renderer;
        friend class SceneHierarchyPanel;
    };
}