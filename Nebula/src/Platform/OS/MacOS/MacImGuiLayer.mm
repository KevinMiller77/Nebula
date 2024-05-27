#include <backends/imgui_impl_osx.h>


bool ImGui_ImplOSX_Init_CPP(void* view) {
    return ImGui_ImplOSX_Init((NSView*)view);
}
void ImGui_ImplOSX_Shutdown_CPP(void) {
    ImGui_ImplOSX_Shutdown();
}
void ImGui_ImplOSX_NewFrame_CPP(void* view) {
    ImGui_ImplOSX_NewFrame((NSView*)view);
}