#include "MeshImporter.h"

namespace Nebula {
    namespace Importer {

        void TokenizeLine(std::vector<std::string>& tokenDst, const std::string& line) {
            std::string currentToken;
            for (char i : line) {
                switch (i) {
                    case ('\n'):
                    case(' '): {
                        if (currentToken.size() != 0) {
                            tokenDst.push_back(currentToken);
                            currentToken = "";
                        }
                        break;
                    }
                    default: {
                        currentToken += i;
                    }
                }
            }
            if (currentToken.size()) {
                tokenDst.push_back(currentToken);
            }
        }

        // Returns is the special case is true where v1//vn1
        void TokenizeFace(std::vector<std::vector<int>>& tokenDst, const std::vector<std::string>& tokensRepresentingFace, uint32_t firstTokenOffset) {
            std::vector<int> currentOutputTokenSet;

            for (int unformattedTokenIDX = firstTokenOffset; unformattedTokenIDX < tokensRepresentingFace.size(); unformattedTokenIDX++) {
                // This is one group of face defs ie. 1/1/1
                std::string currentInputTokenSet = tokensRepresentingFace[unformattedTokenIDX]; 
                std::string currentOutputNumberAsStr;
                for (char i : currentInputTokenSet) {
                    switch (i) {
                        case('/'): {
                            if (currentOutputNumberAsStr.size() != 0) {
                                currentOutputTokenSet.push_back(std::stoi(currentOutputNumberAsStr));
                                currentOutputNumberAsStr = "";
                            }
                            break;
                        }
                        default: {
                            currentOutputNumberAsStr.push_back(i);
                        }
                    }
                }
                // Wrap up the last subtoken if exists
                if (currentOutputNumberAsStr.size()) {
                    currentOutputTokenSet.push_back(std::stoi(currentOutputNumberAsStr));
                }

                tokenDst.push_back(currentOutputTokenSet);
                currentOutputTokenSet = std::vector<int>();
            }
        }

        // FBX
        Ref<iMeshScene> ImportMesh_FBX(const std::string& filepath) {
            LOG_ERR("[Mesh Importer] FBX IS NOT CURRENTLY SUPPORTED\n");
            return nullptr;
        }

        /* Tokens
            mtllib - load lib
            usemtl - use material for following obj
            o - start object
            s - smooth shading (bool - 1/off)
        */
        // OBJ

        bool LoadMaterialMap(Ref<iMeshMaterial> material, iMeshMaterialFlags flag, const std::string& filepath) {
            if(!VFS::Exists(filepath, true)) {
                LOG_ERR("[Mesh Importer] Could not load texture for material: %s\n", filepath.c_str());
                return false;
            }

            material->AddFlag(flag);

            iMeshTexture tex;
            tex.mPath = filepath;
            tex.mType = GetTextureTypeFromFlag(flag);

            material->mNumTextures++;
            material->mTextures.push_back(tex);

            return true;
        }

        bool LoadMaterialsFromFile(Ref<iMeshScene> scene, const std::string& filepath) {
            std::string currentDirectory = std::filesystem::path(filepath).parent_path().string() + "/";

            std::ifstream matFile;
            matFile.open(filepath, std::ios::in);

            if (!matFile.is_open()) {
                LOG_ERR("[Mesh Importer] Error opening obj mat file: %s", filepath.c_str());
                return false;
            }

            Ref<iMeshMaterial> currentMaterialRef = CreateRef<iMeshMaterial>();

            bool MaterialOpen = false;
            std::string currentLine;
            while( std::getline(matFile, currentLine) ) {
                // Tokenize the current line
                std::vector<std::string> tokensOnLine;
                TokenizeLine(tokensOnLine, currentLine);

                if (tokensOnLine.size() == 0) {
                    continue;
                }

                if (tokensOnLine[0][0] == '#') {
                    continue;
                }

                // Push the last material to the scene and start a new one
                if (tokensOnLine[0] == "newmtl") {
                    if (MaterialOpen) {
                        scene->mNumMaterials++;
                        scene->mMaterials.push_back(currentMaterialRef);

                    }

                    currentMaterialRef = CreateRef<iMeshMaterial>();
                    currentMaterialRef->mName = currentLine.substr(7);
                    MaterialOpen = true;
                }
                else if (tokensOnLine[0] == "Ka") {
                    currentMaterialRef->AddFlag(iMeshMaterialFlags::AmbientColor);
                    currentMaterialRef->mAmbientColor = { std::stof(tokensOnLine[1]), std::stof(tokensOnLine[2]), std::stof(tokensOnLine[3]) } ;                   
                }
                else if (tokensOnLine[0] == "Kd") {
                    currentMaterialRef->AddFlag(iMeshMaterialFlags::DiffuseColor);
                    currentMaterialRef->mDiffuseColor = { std::stof(tokensOnLine[1]), std::stof(tokensOnLine[2]), std::stof(tokensOnLine[3]) } ; 
                }
                else if (tokensOnLine[0] == "Ks") {
                    currentMaterialRef->AddFlag(iMeshMaterialFlags::SpecularColor);
                    currentMaterialRef->mDiffuseColor = { std::stof(tokensOnLine[1]), std::stof(tokensOnLine[2]), std::stof(tokensOnLine[3]) } ; 
                }
                else if (tokensOnLine[0] == "Ke") {
                    currentMaterialRef->AddFlag(iMeshMaterialFlags::EmissiveColor);
                    currentMaterialRef->mEmissiveColor = { std::stof(tokensOnLine[1]), std::stof(tokensOnLine[2]), std::stof(tokensOnLine[3]) } ; 
                }
                else if (tokensOnLine[0] == "Ni") {
                    currentMaterialRef->AddFlag(iMeshMaterialFlags::RefractionIndex);
                    currentMaterialRef->mSpecular = std::stof(tokensOnLine[1]);
                }
                else if (tokensOnLine[0] == "Ns") {
                    currentMaterialRef->AddFlag(iMeshMaterialFlags::SpecularHighlights);
                    currentMaterialRef->mSpecular = std::stof(tokensOnLine[1]);
                }
                else if (tokensOnLine[0] == "d") {
                    currentMaterialRef->AddFlag(iMeshMaterialFlags::RefractionIndex);
                    currentMaterialRef->mSpecular = std::stof(tokensOnLine[1]);
                }
                else if (tokensOnLine[0] == "Tr") {
                    currentMaterialRef->AddFlag(iMeshMaterialFlags::RefractionIndex);
                    currentMaterialRef->mSpecular = 1.0f - std::stof(tokensOnLine[1]);
                    
                }
                else if (tokensOnLine[0] == "Tf") {
                    currentMaterialRef->AddFlag(iMeshMaterialFlags::TransmissionColor);
                    // Fuck this
                    if (tokensOnLine[1] == "xyz") {
                        if (tokensOnLine.size() == 5) {
                            currentMaterialRef->mDiffuseColor = { std::stof(tokensOnLine[2]), std::stof(tokensOnLine[3]), std::stof(tokensOnLine[4]) } ; 
                        } else {
                            currentMaterialRef->mDiffuseColor = { std::stof(tokensOnLine[1]), std::stof(tokensOnLine[1]), std::stof(tokensOnLine[1]) } ; 
                        }
                    } else {
                        currentMaterialRef->mDiffuseColor = { std::stof(tokensOnLine[1]), std::stof(tokensOnLine[2]), std::stof(tokensOnLine[3]) } ; 
                    }
                }
                else if (tokensOnLine[0] == "illum") {
                    currentMaterialRef->AddFlag(iMeshMaterialFlags::Illumination);
                    currentMaterialRef->mIlluminationModel = std::stof(tokensOnLine[1]);
                }
                else if (tokensOnLine[0] == "map_Ka") {
                    LoadMaterialMap(currentMaterialRef, iMeshMaterialFlags::AmbientMap, currentDirectory + tokensOnLine[1]);
                }
                else if (tokensOnLine[0] == "map_Ke") {
                    LoadMaterialMap(currentMaterialRef, iMeshMaterialFlags::EmissiveMap, currentDirectory + tokensOnLine[1]);
                }
                else if (tokensOnLine[0] == "map_Kd") {
                    LoadMaterialMap(currentMaterialRef, iMeshMaterialFlags::DiffuseMap, currentDirectory + tokensOnLine[1]);
                }
                else if (tokensOnLine[0] == "map_Ks") {
                    LoadMaterialMap(currentMaterialRef, iMeshMaterialFlags::SpecularColorMap, currentDirectory + tokensOnLine[1]);
                }
                else if (tokensOnLine[0] == "map_Ns") {
                    LoadMaterialMap(currentMaterialRef, iMeshMaterialFlags::SpecularHighlightMap, currentDirectory + tokensOnLine[1]);
                }
                else if (tokensOnLine[0] == "map_d") {
                    LoadMaterialMap(currentMaterialRef, iMeshMaterialFlags::AlphaMap, currentDirectory + tokensOnLine[1]);
                }
                else if (tokensOnLine[0] == "map_bump" || tokensOnLine[0] == "bump") {
                    LoadMaterialMap(currentMaterialRef, iMeshMaterialFlags::BumpMap, currentDirectory + tokensOnLine[1]);
                } 
            }

            if (MaterialOpen) {
                scene->mNumMaterials++;
                scene->mMaterials.push_back(currentMaterialRef);
            }

            return true;
        }

        std::vector<Ref<iMeshFace>> ParseFaces(const std::vector<std::string>& tokensOnLine, bool smoothingEnabled, Ref<iMeshNode> node) {
            std::vector<Ref<iMeshFace>> outFaces;

            int numberOfIndicesInFace = tokensOnLine.size() - 1;
            bool texCoordsSkipped = tokensOnLine[1].find("//") != std::string::npos;

            std::vector<std::vector<int>> formattedFaceIndices;
            TokenizeFace(formattedFaceIndices, tokensOnLine, 1);

            // v1//vn1 given
            if (!texCoordsSkipped) {

                enum IndexType {
                    VERTEX_POSITION = 1,
                    VERTEX_NORMAL = 2,
                    TEX_COORD = 3
                };

                // Check how many params there are if not special case (free form geo)
                int numVertices = formattedFaceIndices.size();
                node->mNumVertices += numVertices;
                
                // Do the faces in batches of 3 all the time

                bool hasTexCoord = texCoordsSkipped ? false : formattedFaceIndices[0].size() > 1;
                bool hasNormal = texCoordsSkipped || formattedFaceIndices[0].size() == 3;

                uint32_t posIdx = 0;
                uint32_t texIdx = 1;
                uint32_t normalIdx = hasTexCoord ? 2 : 1;

                for (int wave = 0; wave < numVertices / 3; wave++) {
                    Ref<iMeshFace> face = CreateRef<iMeshFace>();

                    bool generateNormals = true;
                    for (int i = 0; i < 3; i++) {
                        int curFaceInd = wave * 3 + i;

                        face->mPositionIndices.push_back(formattedFaceIndices[curFaceInd][posIdx]);
                         
                        if (hasTexCoord) {
                            face->mTexCoordIndices.push_back(formattedFaceIndices[curFaceInd][texIdx]);
                        }
                        if (hasNormal) {
                            face->mVertexNormalIndices.push_back(formattedFaceIndices[curFaceInd][normalIdx]);

                        }
                        face->mNumIndices++;
                    }

                    // Calculate Normal, Tangent, and Bitangent here
                    face->smoothingEnabled = smoothingEnabled;
                    face->generatedNormals = generateNormals;
                    outFaces.push_back(face);
                }
                return outFaces;
            }

            int numVertices = formattedFaceIndices.size();
            node->mNumVertices += numVertices;
            // Do the faces in batches of 3 all the time
            for (int wave = 0; wave < numVertices / 3; wave++) {
                Ref<iMeshFace> face = CreateRef<iMeshFace>();
                for (int i = 0; i < 3; i++) {
                    face->mPositionIndices.push_back(formattedFaceIndices[i][0]);

                    face->mVertexNormalIndices.push_back(formattedFaceIndices[i][1]);

                    face->mNumIndices++;
                }

                face->smoothingEnabled = smoothingEnabled;
                outFaces.push_back(face);
            }

            return outFaces;
        }

        Ref<iMeshScene> ImportMesh_OBJ(const std::string& filepath) {
            std::string currentDirectory = std::filesystem::path(filepath).parent_path().string() + "/";
            std::string fileNameNoExt = std::filesystem::path(filepath).stem().string();

            Ref<iMeshScene> outMeshScene = CreateRef<iMeshScene>();

            std::ifstream objFile;
            objFile.open(filepath, std::ios::in);

            if (!objFile.is_open()) {
                return nullptr;
            }

            // Root takes all base information (this is the submeshes library of vertex info)
            // Mesh only has faces (makes sense, right)

            Ref<iMeshRoot> rootNodeRef = CreateRef<iMeshRoot>();
            rootNodeRef->mName = fileNameNoExt;

            Ref<iMeshMaterial> currentMatRef = CreateRef<iMeshMaterial>();
            Ref<iMeshNode> currentSubNodeRef = CreateRef<iMeshNode>();

            bool verticesGivenBeforeASubMesh = false;
            std::vector<Vec3f> l_Vertices;
            std::vector<Vec2f> l_TexCoords;
            std::vector<Vec3f> l_VertexNormals;

            bool smoothingEnabled = false;

            bool NodeOpen = false;
            auto l_SubmitAndInitNewNode = [&NodeOpen, &outMeshScene, &currentSubNodeRef](const std::string& name) {
                if (NodeOpen) {
                    outMeshScene->mNumMeshes++;
                    outMeshScene->mMeshes.push_back(currentSubNodeRef);
                };

                currentSubNodeRef = CreateRef<iMeshNode>();
                currentSubNodeRef->mName = name;
                
                NodeOpen = true;
            };

            std::string currentLine;
            while( std::getline(objFile, currentLine) ) {
                // Tokenize the current line
                std::vector<std::string> tokensOnLine;
                TokenizeLine(tokensOnLine, currentLine);

                // Throw away empty lines and comments
                if (!tokensOnLine.size()) {
                    continue;
                }
                if (tokensOnLine[0][0] == '#') {
                    continue;
                }

                // Scene and Root Node parameters
                if (tokensOnLine[0] == "mtllib") {
                    std::string matPath = currentDirectory + tokensOnLine[1];
                    if(!VFS::Exists(matPath, true)) {
                        LOG_ERR("[Mesh Importer] Material in obj did not exist: %s", matPath.c_str());
                        continue;
                    }

                    if (!LoadMaterialsFromFile(outMeshScene, matPath)) {
                        LOG_ERR("[Mesh Importer] Error occured adding a material to MeshScene!\n");
                    }
                }
                else if (tokensOnLine[0] == "v") {
                    rootNodeRef->mNumPositions++;
                    rootNodeRef->mPositions.push_back({ std::stof(tokensOnLine[1]), std::stof(tokensOnLine[2]), std::stof(tokensOnLine[3]) });                    
                }
                else if (tokensOnLine[0] == "vt") {
                    rootNodeRef->mTextureCoords[0].push_back({ std::stof(tokensOnLine[1]), std::stof(tokensOnLine[2]) });
                }
                else if (tokensOnLine[0] == "vn") {
                    rootNodeRef->mNumNormals++;
                    rootNodeRef->mNormals.push_back({ std::stof(tokensOnLine[1]), std::stof(tokensOnLine[2]), std::stof(tokensOnLine[3]) });
                }
                else if (tokensOnLine[0] == "vp") {
                    rootNodeRef->mNumFreeforms++;
                    rootNodeRef->mFreeforms.push_back({ std::stof(tokensOnLine[1]), std::stof(tokensOnLine[2]), std::stof(tokensOnLine[3]) });
                }
                else if (tokensOnLine[0] == "l") {
                    //TODO: Implement Lines
                    uint32_t numLineIndices = tokensOnLine.size() - 1;
                    uint32_t setsOfLines = numLineIndices / 2;

                    for (uint32_t set = 0; set < setsOfLines; set++) {
                        rootNodeRef->mNumLineVertexSets++;
                        rootNodeRef->mLineVertices.push_back({ std::stoi(tokensOnLine[(set * 2) + 1]), std::stoi(tokensOnLine[(set * 2) + 2]) });
                    }
                }

                // Sub Mesh parameters
                else if (tokensOnLine[0] == "usemtl") {
                    currentSubNodeRef->mMaterialIndex = outMeshScene->GetMaterialIndex(tokensOnLine[1]);
                }
                // O Means start a new mesh but sometimes doesn't exist!
                else if (tokensOnLine[0] == "o" || tokensOnLine[0] == "g") {
                    l_SubmitAndInitNewNode(tokensOnLine[1]);
                }
                else if (tokensOnLine[0] == "s") {
                    smoothingEnabled = tokensOnLine[1] == "1" ? true : false;
                }
                else if (tokensOnLine[0] == "f") {
                    std::vector<Ref<iMeshFace>> parsedFaces = ParseFaces(tokensOnLine, smoothingEnabled, currentSubNodeRef);

                    for (Ref<iMeshFace> parsedFace : parsedFaces) {
                        if (parsedFace) {
                            currentSubNodeRef->mNumFaces++;
                            currentSubNodeRef->mFaces.push_back(parsedFace);

                            // Generate Normals
                            if (parsedFace->generatedNormals) {
                                // NOT YET IMPLEMENTED AS YOU CAN SEE
                            }
                        }
                    }
                }
            }

            if (NodeOpen) {
                outMeshScene->mNumMeshes++;
                outMeshScene->mMeshes.push_back(currentSubNodeRef);
                NodeOpen = false;
            } 

            outMeshScene->mRootNode = rootNodeRef;
            return outMeshScene;
        }

        Ref<MeshFile> s_ImportMesh(const std::string& filepath) {
            Ref<MeshFile> out = CreateRef<MeshFile>();

            out->type = FileType::Mesh;
            out->filepath = filepath;

            const std::string ext = filepath.substr(filepath.size() - 3);
            if (ext == "fbx") {
                out->meshScene = ImportMesh_FBX(filepath);
                return out;
            }
            if (ext == "obj") {
                out->meshScene = ImportMesh_OBJ(filepath);
                return out;    
            }

            return nullptr;
        }
    } 
}