#include <backends/imgui_impl_metal.h>

namespace MTL {
    class Device;
    class RenderPassDescriptor;
    class CommandBuffer;
    class RenderCommandEncoder;
}

bool ImGui_ImplMetal_Init_CPP(MTL::Device* device) {
    return ImGui_ImplMetal_Init((id)device);
}
void ImGui_ImplMetal_Shutdown_CPP() {
    ImGui_ImplMetal_Shutdown();
}
void ImGui_ImplMetal_NewFrame_CPP(MTL::RenderPassDescriptor* renderPassDescriptor) {
    ImGui_ImplMetal_NewFrame((id)renderPassDescriptor);
}
void ImGui_ImplMetal_RenderDrawData_CPP(ImDrawData* drawData, MTL::CommandBuffer* commandBuffer, MTL::RenderCommandEncoder* commandEncoder) {
    ImGui_ImplMetal_RenderDrawData(drawData, (id)commandBuffer, (id)commandEncoder);
}