#pragma once

#include "Event.h"

namespace Nebula{
    class MouseMovedEvent : public Event
    {
    protected:
        Vec2f position;

    public:
        MouseMovedEvent(Vec2f newPos)
            :   position(newPos)  {}

        inline const Vec2f getPos() const { return position; }

        EVENT_CLASS_TYPE(EventType::MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    };

    class MouseScrolledEvent : public Event
    {
    protected:
        float XOffset, YOffset;

    public:
        MouseScrolledEvent(float X, float Y)
            :   XOffset(X), YOffset(Y)  {}

        inline const float GetXOffset() const { return XOffset; }
        inline const float GetYOffset() const { return YOffset;}

        EVENT_CLASS_TYPE(EventType::MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    };

    class MouseButtonEvent : public Event
    {
    protected:
        MouseCode ButtonIQ;

    public:
        MouseButtonEvent(MouseCode buttonInQuestion)
            :   ButtonIQ(buttonInQuestion) {}

        inline MouseCode getButton() { return ButtonIQ; }
        
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    };


    class MouseButtonPressedEvent : public MouseButtonEvent
    {

    public:
        MouseButtonPressedEvent(MouseCode button)
            : MouseButtonEvent(button)  {}
        
        EVENT_CLASS_TYPE(EventType::MouseButtonPressed);
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent
    {

    public:
        MouseButtonReleasedEvent(MouseCode button)
            : MouseButtonEvent(button)  {}
        
        EVENT_CLASS_TYPE(EventType::MouseButtonReleased);
    };
}
