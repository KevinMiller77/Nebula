#pragma once
#include <Nebula.h>

class NebulaStudioLayer : public Nebula::Layer
{
public:

    static Vec4f clearColor;
    
    Nebula::Framebuffer* FrameBuffer;
    bool m_ViewportFocused = false, m_ViewportHovered = false;
    Vec2f ViewportSize = {0.0f, 0.0f};

    Nebula::Shader* shader = nullptr;
    Nebula::OrthographicCameraController* Camera = nullptr;
    Nebula::Texture2D* texture = nullptr;

    NebulaStudioLayer() {};

    virtual void OnAttach() override;
    virtual void OnUpdate(float ts) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Nebula::Event& e) override;
};