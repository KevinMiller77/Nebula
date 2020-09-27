#pragma once

#include "Event.h"

namespace Nebula
{
    class WindowResizeEvent : public Event
    {
    protected:
        Vec2u screenSize;

    public:
        WindowResizeEvent(Vec2u newSize)
            : screenSize(newSize)   {}

        inline Vec2u GetScreenSize() const { return screenSize; } 

        EVENT_CLASS_TYPE(EventType::WindowResize)
        EVENT_CLASS_CATEGORY(EventCategoryWindow)
    };

    class WindowFocusEvent : public Event
    {
    public:
        WindowFocusEvent() {}

        EVENT_CLASS_TYPE(EventType::WindowFocus)
        EVENT_CLASS_CATEGORY(EventCategoryWindow)
    };

    class WindowLostFocusEvent : public Event
    {
    public:
        WindowLostFocusEvent() {}

        
        EVENT_CLASS_TYPE(EventType::WindowFocus)
        EVENT_CLASS_CATEGORY(EventCategoryWindow)
    };

    class WindowCloseEvent : public Event
    {

    public:
        WindowCloseEvent() = default;

        EVENT_CLASS_TYPE(EventType::WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryWindow)
    };
}
