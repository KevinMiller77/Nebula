#pragma once

#include <Math/math.h>

#include "Camera.h"
#include "Shader.h"
#include "VertexArray.h"
#include "RendererConfig.h"

namespace Nebula
{
    class Renderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void OnWindowResize(unsigned int w, unsigned int h);

        static void BeginScene(Camera camera);
        static void EndScene();

        static void Submit(Shader* shader, VertexArray* vertexArray, const Mat4f transform = Mat4f(1.0f));
    
        static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

    private:
        struct SceneData_t
        {
            Mat4f ViewProjMatrix;
        };

        static SceneData_t* SceneData;
    };
}