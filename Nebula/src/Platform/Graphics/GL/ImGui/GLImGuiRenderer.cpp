#include "GLImGuiRenderer.h"


// TODO: Guard this behind the possibility of GL being used
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <examples/imgui_impl_opengl3.cpp>

#include <imgui.h>
#include <examples/imgui_impl_opengl3.h>

namespace Nebula{

    bool GLImGuiRenderer::Init() { 
        return ImGui_ImplOpenGL3_Init("#version 410");
    }

    void GLImGuiRenderer::Shutdown() { 
        ImGui_ImplOpenGL3_Shutdown();
    }

    void GLImGuiRenderer::NewFrame() { 
        ImGui_ImplOpenGL3_NewFrame();
    }

    void GLImGuiRenderer::Render(ImDrawData* draw_data) {
        ImGui_ImplOpenGL3_RenderDrawData(draw_data);
    }

}
