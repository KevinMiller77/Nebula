#pragma once

#include <Core/NebulaCommon.h>
#include <Core/Ref.h>
#include <Math/math.h>

#include "Camera.h"
#include "Shader.h"
#include "VertexArray.h"
#include "RendererConfig.h"
#include "Renderer2D.h"

namespace Nebula
{
    class Renderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void OnWindowResize(uint32 w, uint32 h);
        static void ReloadShaders();

        static void BeginScene(Camera camera);
        static void EndScene();

        static void Submit(Ref<Shader> shader, Ref<VertexArray> vertexArray, const Mat4f transform = Mat4f(1.0f));
    
        static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

    private:
        struct SceneData_t
        {
            Mat4f ViewProjMatrix;
        };

        static SceneData_t* SceneData;
    };
}