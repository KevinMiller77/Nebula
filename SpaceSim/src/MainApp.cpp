#include <Nebula.h>

class MainApp : public Nebula::Application
{
    MainApp() : Nebula::Application(this)
    {
        //TODO: Settings loader or auto adjuster
        window->SetVSync(true);


    }


};

Ref<Nebula::Application> CreateApplication()
{
    return CreateRef<MainApp>();
} 