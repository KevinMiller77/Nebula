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
};