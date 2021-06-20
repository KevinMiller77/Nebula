#pragma once

#include <vector>
#include <string>
#include <Math/math.h>
#include <Core/VFS.h>
#include <Core/Files.h>
#include <Core/Ref.h>

#define MAX_NUMBER_OF_TEXTURE_COORDS 8

namespace Nebula {
    struct iMeshFace {
        uint32_t mNumIndices = 0;
        
        std::vector<uint32_t> mPositionIndices;
        
        std::vector<uint32_t> mTexCoordIndices;
        
        std::vector<uint32_t> mVertexNormalIndices;
        std::vector<uint32_t> mVertexTangentIndices;
        std::vector<uint32_t> mVertexBitangentIndices;

        std::vector<uint32_t> mFreeGeometryIndices;

        bool smoothingEnabled = false;
        bool generatedNormals = false;
    };

    struct iMeshBoneWeights {
        uint32_t mVertexID = 0;    
        float mWeight;
    };

    struct iMeshBone {
        std::string mName;
        Mat4f mOffsetMatrix = Mat4f(1.0f);

        uint32_t mNumWeights = 0;
        std::vector<iMeshBoneWeights> mWeights;
    };

    struct iKeyFrameKeys {
        float mKeyTime = 1.0f;
    };

    struct iPositionKey : iKeyFrameKeys { Vec3f mPosition = {0.0f, 0.0f, 0.0f};};
    struct iRotationKey : iKeyFrameKeys { Vec3f mRotation = {0.0f, 0.0f, 0.0f};};
    struct iScaleKey : iKeyFrameKeys    { Vec3f mScale = {0.0f, 0.0f, 0.0f};   };

    struct iNodeAnimation {
        std::string mName;

        float mTicksPerSecond = 1.0f;
        float mDuration = 1.0f;

        std::vector<iPositionKey> mPositionKeys;
        std::vector<iRotationKey> mRotationKeys;
        std::vector<iScaleKey> mScaleKeys;
    };

    struct iMeshRoot {
        std::string mName;

        uint32_t mNumPositions = 0;
        std::vector<Vec3f> mPositions;

        uint32_t mNumNormals = 0;
        std::vector<Vec3f> mNormals;

        uint32_t mNumFreeforms = 0;
        std::vector<Vec3f> mFreeforms;

        std::vector<Vec2f> mTextureCoords[MAX_NUMBER_OF_TEXTURE_COORDS];

        uint32_t mNumLineVertexSets = 0;
        std::vector<Vec2i> mLineVertices;

        uint32_t mNumBones = 0;
        std::vector<iMeshBone> mBones;

        std::vector<Vec3f> mTangents;
        std::vector<Vec3f> mBitangents;


        bool HasNormals() { return mNumNormals; }
        bool HasPositions() { return mNumPositions; }
        bool HasTangentsAndBitangents() { return mTangents.size() && mBitangents.size(); }
        bool HasTextureCoords(int i) { return mTextureCoords[i].size(); }
    };

    // Mesh Requires a root node for the bundled vectex data
    struct iMeshNode {

        std::string mName;
        
        uint32_t mMaterialIndex = 0;
        uint32_t mNumVertices = 0;
        
        uint32_t mNumFaces = 0;
        std::vector<Ref<iMeshFace>> mFaces;

        Mat4f mTransform = Mat4f(1.0f);

        std::vector<Ref<iMeshNode>> mChildren;
    };

    enum class iMeshTextureType {
        NONE = 0,
        AMBIENT,
        DIFFUSE,
        NORMAL,
        ROUGHNESS,
        METALLIC,
        SHEEN,
        TRANSMISSION,
        SPECULARC,
        SPECULARH,
        OCCLUSION,
        ALPHA,
        EMISSIVE,
        BUMP,
        OPACITY,
        DISPLACEMENT
    };


    enum class iMeshMaterialFlags : uint8_t {
        AmbientColor = 0,
        AmbientMap = 22,
        DiffuseColor = 1,
        DiffuseMap = 23,
        SpecularColor = 2,
        SpecularColorMap = 24,
        SpecularHighlights = 3,
        SpecularHighlightMap = 25,
        RefractionIndex = 4,
        Dissolve = 5,
        Illumination = 6,
        Roughness = 7,
        RoughnessMap = 8,
        Metallic = 9,
        MetallicMap = 10,
        Sheen = 11,
        SheenMap = 12,
        ClearcoatThickness = 13,
        ClearcoatRoughness = 14,
        EmissiveColor = 15,
        EmissiveMap = 16,
        Anistropy = 17,
        AnistropyRotation = 18,
        NormalMap = 19,
        TransmissionColor = 20,
        TransmissionMap = 21,
        AlphaMap = 26,
        BumpMap = 27,
        DisplacementMap = 28
    };

    enum class iMeshMaterialAttribType {
        NONE = 0,
        COLOR_DIFFUSE,
        SHININESS,
        REFLECTIVITY
    };

    inline iMeshTextureType GetTextureTypeFromFlag(iMeshMaterialFlags flag) 
    {   
        switch(flag) {

            case(iMeshMaterialFlags::AmbientMap) :      {return iMeshTextureType::AMBIENT           ;}               
            case(iMeshMaterialFlags::DiffuseMap) :      {return iMeshTextureType::DIFFUSE           ;}
            case(iMeshMaterialFlags::SpecularColor) :   {return iMeshTextureType::SPECULARC         ;}
            case(iMeshMaterialFlags::SpecularHighlightMap) : {return iMeshTextureType::SPECULARH    ;}
            case(iMeshMaterialFlags::RoughnessMap) :    {return iMeshTextureType::ROUGHNESS         ;}
            case(iMeshMaterialFlags::MetallicMap) :     {return iMeshTextureType::METALLIC          ;}
            case(iMeshMaterialFlags::SheenMap) :        {return iMeshTextureType::SHEEN             ;}
            case(iMeshMaterialFlags::TransmissionMap) : {return iMeshTextureType::TRANSMISSION      ;}
            case(iMeshMaterialFlags::AlphaMap) :        {return iMeshTextureType::ALPHA             ;}
            case(iMeshMaterialFlags::BumpMap) :         {return iMeshTextureType::BUMP              ;}
            case(iMeshMaterialFlags::DisplacementMap) : {return iMeshTextureType::DISPLACEMENT      ;}
        }
    }

    inline bool iMeshHasFlag(uint32_t meshFlags, iMeshMaterialFlags neededFlag) {
        return 1 & (meshFlags >> (uint32_t)neededFlag);
    }


    // MIGHT BE SERIOUSLY WRONG HERE -- NOT SURE WHAT TF IS GOING ON DADDYO
    inline bool iMeshHasAttribute(uint32_t meshFlags, iMeshMaterialAttribType attrib) {
        switch(attrib) {
            case(iMeshMaterialAttribType::COLOR_DIFFUSE): {
                return iMeshHasFlag(meshFlags, iMeshMaterialFlags::DiffuseColor);
                break;
            }
            case(iMeshMaterialAttribType::REFLECTIVITY): {
                return iMeshHasFlag(meshFlags, iMeshMaterialFlags::SpecularHighlights);
                break;
            }
            case(iMeshMaterialAttribType::SHININESS): {
                return iMeshHasFlag(meshFlags, iMeshMaterialFlags::Roughness);
                break;
            }

            case(iMeshMaterialAttribType::NONE):
            default: {
                return false;
            }
        }
    }

    struct iMeshTexture {
        iMeshTextureType mType;

        // Ensure when this is set, it is the absolute path!
        std::string mPath;
    };

    struct iMeshMaterial {
        std::string mName;

        uint32_t mMatFlags = 0; // 

        Vec3f mAmbientColor = {1.0f, 1.0f, 1.0f};        //Ka
        Vec3f mDiffuseColor = {1.0f, 1.0f, 1.0f};        //Kd
        Vec3f mSpecularColor = {1.0f, 1.0f, 1.0f};       //Ks
        Vec3f mEmissiveColor = {1.0f, 1.0f, 1.0f};       //Ks
        Vec3f mTransmissiveColor = {1.0f, 1.0f, 1.0f};   //Tf

        float mSpecular = 1.0f;  //Ns
        float mRefractionIndex = 1.0f;     //Ni
        float mDissolve = 1.0f;            //d
        float mIlluminationModel = 1.0f;   //illum
        float mRoughness = 1.0f;

        uint32_t mNumTextures = 0;
        std::vector<iMeshTexture> mTextures; //map_Kd

        bool Get(iMeshMaterialAttribType type, void* value) {
            if (!iMeshHasAttribute(mMatFlags, type)) {
                return false;
            }

            switch (type) {
                case(iMeshMaterialAttribType::COLOR_DIFFUSE): {
                    *(Vec3f*)value = mDiffuseColor;
                    return true;
                    break;
                }
                case(iMeshMaterialAttribType::SHININESS): {
                    *(float*)value = mRoughness;
                    return true;
                    break;
                }
                case(iMeshMaterialAttribType::REFLECTIVITY): {
                    *(float*)value = mSpecular;
                    return true;
                    break;
                }
                default: {
                    return false;
                }
            }
        }

        bool GetTexture(iMeshTextureType type, uint32_t index, std::string* path) {
            uint32_t cntIdx = index;
            for (iMeshTexture i : mTextures) {
                if (type == i.mType) {
                    if (cntIdx == 0) {
                        *path = i.mPath;
                        return true;
                    }
                    cntIdx--;
                }
            }
            return false;
        }

        std::string GetName() { return mName; }
        uint32_t GetTextureCount(iMeshTextureType type) { 
            uint32_t num = 0;
            for (auto& i : mTextures) { if (i.mType == type) num++; }
            return num;
        }

        void AddFlag(iMeshMaterialFlags flag) {
            mMatFlags = mMatFlags | (1 << (uint8_t)flag);
        }
    };


    struct iMeshKey : iKeyFrameKeys { uint32_t mKey; };
    struct iMeshAnimation {
        std::string mName;

        uint32_t mNumKeys = 0;
        std::vector<iMeshKey> mKeys;
    };

    struct iSceneAnimation {
        std::string mName;

        float mDuration = 0.0f;
        float mTicksPerSecond = 0.0f;

        uint32_t mNumChannels = 0;
        std::vector<iNodeAnimation> mChannels;

        uint32_t mNumMeshChannels = 0;
        std::vector<iMeshAnimation> mMeshChannels;
    };

    struct iMeshScene {
        Ref<iMeshRoot> mRootNode;

        uint32_t mNumAnimations = 0;
        std::vector<iSceneAnimation> mAnimations;

        uint32_t mNumMeshes = 0;
        std::vector<Ref<iMeshNode>> mMeshes;

        uint32_t mNumMaterials = 0;
        std::vector<Ref<iMeshMaterial>> mMaterials;

        bool HasMeshes() { return mNumMeshes; }
        bool HasMaterials() { return mNumMaterials; }

        int GetMaterialIndex(const std::string& name) {
            int idx = 0;
            for (auto i : mMaterials) {
                if (i->mName == name) {
                    return idx; 
                }
                idx++;
            }
            return -1;
        }
        int GetMeshIndex(const std::string& name) {
            int idx = 0;
            for (auto i : mMeshes) {
                if (i->mName == name) {
                    return idx; 
                }
                idx++;
            }
            return -1;
        }
        int GetAnimationIndex(const std::string& name) {
            int idx = 0;
            for (auto i : mAnimations) {
                if (i.mName == name) {
                    return idx; 
                }
                idx++;
            }
            return -1;
        }

        void PrintScene() {

        }
    };

    namespace Importer {
       Ref<MeshFile> s_ImportMesh(const std::string& filepath);
    }
}