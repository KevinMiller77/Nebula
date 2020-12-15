#pragma once

#include <Nebula.h>

namespace Nebula
{
    class BootLayer : public Layer
    {
        enum BootState
        {
            GREET, NEW_PROJ, PROJ_READY_NEW, PROJ_READY_OPEN, DONE, CANCEL
        };

    public:
        BootLayer()
        {
        }
        ~BootLayer() = default;

        void DrawProjSelection();
        void DrawNewProjScreen();

        virtual void OnAttach() override;
        virtual void OnUpdate(float ts) override;
        virtual void OnImGuiRender() override;

        virtual void OnEvent(Event& e) override;
        
        inline std::string Done() { return State == DONE ? std::string(projLocation) : std::string(); }
        inline void Done(bool done) 
        { 
            if(State == DONE)
            {
                State = done ? DONE : GREET;
            } 
            else
            {
                State = done ? DONE : State;
            }
        }

    private:
        bool hasRecents = false;

        BootState State = GREET;
        char projLocation[256];

        //Project Selection sizes
        float greetingTextWidth = 50.0f;
        float newProjButtonWidth =  50.0f;
        float openProjButtonWidth = 50.0f;
        float submitButtonWidth= 50.0f;
        float cancelButtonWidth = 50.0f;

        //Project Creation sizes
    };
}