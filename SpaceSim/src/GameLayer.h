#pragma once

#include <Nebula.h>

class GameLayer : public Nebula::Layer
{
public:
    GameLayer();
    ~GameLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(float ts) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Nebula::Event& event) override;

    void SetupCamera();
    void InitGrid();


private:
    Ref<Nebula::Framebuffer> m_FrameBuffer;

    Ref<Nebula::Scene> m_Scene;
    Ref<Nebula::SceneCamera> m_SceneCamera;
    
    Nebula::Entity e_Camera;
    Nebula::Entity e_Square;

    bool m_IsViewportFocused = false;
    bool m_IsViewportHovered = false;
    
};