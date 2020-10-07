#pragma once

#include <Core/Application.h>
#include <Debugging/Instrumentor.h>
// #include <Nebula_pch.h>

extern Nebula::Application* CreateApplication();

int main(int argv, char** argc)
{
    NEB_PROFILE_BEGIN_SESSION("Start App", "Nebula-Startup.json");
    auto app = CreateApplication();
    NEB_PROFILE_END_SESSION();

    LOG_INF("Running app\n");

    NEB_PROFILE_BEGIN_SESSION("Runtime", "Nebula-Runtime.json");
    app->Run();
    NEB_PROFILE_END_SESSION();

    NEB_PROFILE_BEGIN_SESSION("Shutdown", "Nebula-Shutdown.json");
    delete app;
    NEB_PROFILE_END_SESSION();

    return 0;
}