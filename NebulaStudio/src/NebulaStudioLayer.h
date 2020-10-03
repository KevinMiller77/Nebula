#pragma once
#include <Nebula.h>

struct QuadInfo
{
    QuadInfo(std::string_view name, Nebula::Texture2D* tex = nullptr) : Name(name), Texture(tex) {}

    std::string_view Name;
    Vec3f Pos = {0.0f, 0.0f, 0.0f};
    Vec2f Size = {1.0f, 1.0f};
    float Rotation = 0;
    Vec4f Color = {1.0f, 1.0f, 1.0f, 1.0f}; 
    Nebula::Texture2D* Texture;
};

class NebulaStudioLayer : public Nebula::Layer
{
public:
    vector<QuadInfo*> quads;

    static Vec4f clearColor;
    
    Nebula::Framebuffer* FrameBuffer;
    bool m_ViewportFocused = false, m_ViewportHovered = false;
    Vec2f ViewportSize = {0.0f, 0.0f};

    Nebula::Shader* shader = nullptr;
    Nebula::OrthographicCameraController* Camera = nullptr;
    Nebula::Texture2D* texture = nullptr;

    NebulaStudioLayer() {};
    ~NebulaStudioLayer() = default;

    virtual void OnAttach() override;
    virtual void OnUpdate(float ts) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Nebula::Event& e) override;
};