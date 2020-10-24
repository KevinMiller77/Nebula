#pragma once
#include <Nebula.h>
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/LogPanel.h"

namespace Nebula
{
    class NebulaStudioLayer : public Layer
    {
    public:
        Scene Scene;
        Entity CameraEntity;

        static Vec4f clearColor;
        
        Framebuffer* FrameBuffer;
        bool ViewportFocused = false, ViewportHovered = false;
        Vec2f ViewportSize = {0.0f, 0.0f};

        Shader* shader = nullptr;
        TextureLibrary textures;

        NebulaStudioLayer() {};
        ~NebulaStudioLayer() = default;

        virtual void OnAttach() override;
        virtual void OnUpdate(float ts) override;
        virtual void OnImGuiRender() override;
        virtual void OnEvent(Event& e) override;

        SceneHierarchyPanel SceneHierarchyPanel;
        LogPanel Log;
    };

}