#pragma once

#include <Core/Application.h>

extern Nebula::Application* CreateApplication();

int main(int argv, char** argc)
{
    auto app = CreateApplication();
    
    LOG_INF("Running app\n"); //TODO: Make log
    
    app->Run();

    delete app;
    
    return 0;
}