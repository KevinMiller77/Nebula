#pragma once

#include <filesystem>

#include <Core/NebulaCommon.h>
#include <Core/Ref.h>
#include <Math/math.h>

#include "Camera.h"
#include "Shader.h"
#include "VertexArray.h"
#include "RendererConfig.h"
#include "Pipeline.h"
#include "Renderer2D.h"
#include "RenderPass.h"
#include "RenderCommandQueue.h"
#include "Material.h"
#include "Mesh.h"

namespace Nebula{
    class Renderer
    {
    public:
        static void Init(Ref<GraphicsContext>);
        static void Shutdown();

        static void OnWindowResize(uint32 w, uint32 h);
        static void ReloadShaders();

        // Takes a function and submits it to the renderer before renderpass
        template <typename FunctionT>
        static void Submit(FunctionT&& func) {
            auto renderCMD = [](void* ptr) {
                auto funcPtr = (FunctionT*)ptr;
                (*funcPtr)();

                funcPtr->~FunctionT();
            };
            auto storageBuffer = Renderer::GetRenderCommandQueue().Allocate(renderCMD, sizeof(func));
            new (storageBuffer) FunctionT(std::forward<FunctionT>(func));
        }
        static void WaitAndRender();

        static void BeginRenderPass(Ref<RenderPass> renderPass, bool clear);
        static void EndRenderPass();

        static void SubmitQuad(Ref<MaterialInstance> material, const Mat4f& transform = Mat4f(1.0f));
		static void SubmitFullscreenQuad(Ref<MaterialInstance> material);
		static void SubmitMesh(Ref<Mesh> mesh, const Mat4f& transform, Ref<MaterialInstance> overrideMaterial = nullptr);
    
		static void DrawAABB(const AABB& aabb, const Mat4f& transform, const Vec4f& color = Vec4f(1.0f));
		static void DrawAABB(Ref<Mesh> mesh, const Mat4f& transform, const Vec4f& color = Vec4f(1.0f));

        static Ref<ShaderLibrary> GetShaderLibrary() { return s_ShaderLibrary; }

        static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

    private:
        static RenderCommandQueue& GetRenderCommandQueue();
        static Ref<ShaderLibrary> s_ShaderLibrary;
        struct SceneData_t
        {
            Mat4f ViewProjMatrix;
        };

        static SceneData_t* SceneData;
    };
}