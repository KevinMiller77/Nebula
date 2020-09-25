#pragma once

extern Nebula::Application* CreateApplication();

int main(int argv, char** argc)
{
    auto app = CreateApplication();
    
    printf("Running app\n"); //TODO: Make log
    
    app->Run();

    delete app;
    
    return 0;
}
 