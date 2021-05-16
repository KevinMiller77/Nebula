#include "GameLayer.h"

GameLayer::GameLayer() : Layer("Game Layer")
{
    m_Scene = CreateRef<Nebula::Scene>();

    e_Camera = m_Scene->CreateEntity("Main Cam");
    auto& camC = e_Camera.AddComponent<Nebula::CameraComponent>();
    camC.Camera = Nebula::SceneCamera(1600.0f / 900.0f);
    camC.FixedAspectRatio = true;
    
    m_Scene->SetPrimaryCamera(e_Camera);
    
    // e_Square = m_Scene->CreateEntity("Square");
    // e_Square.AddComponent<Nebula::RootEntityComponent>();
    // e_Square.AddComponent<Nebula::SpriteRendererComponent>(Nebula::Vec4f(0.0f, 0.5f, 0.5f, 0.8f));

    InitGrid();
}

void GameLayer::OnAttach() 
{
    Nebula::FramebufferSpecification fbSpec;
    fbSpec.Width = 1600;
    fbSpec.Height = 900;
    fbSpec.Samples = 1;
    m_FrameBuffer = Nebula::Framebuffer::Create(fbSpec);

    m_Scene->OnPlay();

    for (int i = 0; i < 1; i++) {
        Nebula::Entity newE = m_Scene->CreateEntity(std::to_string(i).c_str());
        newE.AddComponent<Nebula::RootEntityComponent>();
        newE.AddComponent<Nebula::SpriteRendererComponent>(Nebula::Vec4f(0.0f, 0.5f, 0.5f, 0.8f));
    }
}

void GameLayer::OnDetach() 
{
    m_Scene->OnStop();
}

void GameLayer::OnUpdate(float ts) 
{
    Nebula::RendererConfig::Clear();

    m_Scene->OnUpdateRuntime(ts);
}

void GameLayer::OnImGuiRender() 
{
    ImGui::Begin("Stats");
        
    Nebula::Renderer2DStatistics stats = Nebula::Renderer2D::GetStats();
    
    if (e_Camera.IsValid())
    {
        ImGui::Text("Camera E ID: %s", e_Camera.GetComponent<Nebula::TagComponent>().Tag.c_str());
    }
    else
    {
        ImGui::Text("No cam");
    }
    ImGui::Text("FPS            : %f", Nebula::Application::Get()->GetFPS());
    ImGui::Text("UPS            : %f", Nebula::Application::Get()->GetUPS());
    ImGui::Separator();
    ImGui::Text("Draw Calls     : %d", stats.DrawCalls);    
    ImGui::Text("Quad Count     : %d", stats.QuadCount);    
    ImGui::Text("Line Count     : %d", stats.LineCount);
    ImGui::Text("Vertex Count   : %d ", stats.GetTotalVertexCount());    
    ImGui::Text("Index Count    : %d", stats.GetTotalIndexCount());

    ImGui::End();
}

void GameLayer::OnEvent(Nebula::Event& event) 
{
    switch(event.GetEventType()) {
        case(Nebula::EventType::MouseButtonPressed): 
        {
            break;
        }
        default:
            break;
    }
}

void GameLayer::InitGrid()
{

}