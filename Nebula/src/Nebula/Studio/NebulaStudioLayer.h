#pragma once
#include <Nebula.h>

static Vec4f clearColor = {0.1f, 0.1f, 0.1f, 1.0f};
class NebulaStudioLayer : public Nebula::Layer
{
public:
    float vertexData[7 * 3] = 
    {
        0.5f, -0.5f, 0.0f, 0.41f, 0.0f, 0.18f, 1.0f,
        0.0f, 0.5f, 0.0f, 0.5f, 0.99f, 0.2f, 1.0f,
        -0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f
    };

    unsigned int indicies[3] = {0, 1, 2};

    Nebula::Shader* shader;
    Nebula::VertexArray* va;

    Nebula::BufferLayout layout =  {
        {Nebula::ShaderDataType::Float3, "position"},
        {Nebula::ShaderDataType::Float4, "color"}
    };

    NebulaStudioLayer() = default;
    
    void OnAttach() override
    {
        va = Nebula::VertexArray::Create();
        
        auto vb = Nebula::VertexBuffer::Create(vertexData, sizeof(vertexData));
        vb->SetLayout(layout);
        va->AddVertexBuffer(vb);

        va->SetIndexBuffer(Nebula::IndexBuffer::Create(indicies, sizeof(indicies)));

        
        shader = Nebula::Shader::Create("Nebula/assets/shaders/FlatColor.glsl");
        shader->Bind();

        shader->SetMat4("u_ViewProjection", Mat4f::orthographic(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f));
        shader->SetMat4("u_Transform", Mat4f(1.0f));

    }

    void OnUpdate(float ts) override
    {
        va->Bind();
        Nebula::RendererConfig::DrawIndexed(va);
        va->Unbind();
    }

    void OnImGuiRender() override
    {
        ImGui::Begin("Panel");
        ImGui::ColorPicker4("Background", clearColor.elements);
        ImGui::Text("This probably looks weird as fuck");
        ImGui::End();
    }
};