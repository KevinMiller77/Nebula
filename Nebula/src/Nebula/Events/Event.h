#pragma once

//Pass all things into the event from the platform. 
//Reference all things according to the generic key names 
//Ex. KEY_D0

#include <Core/PlatformInfo.h>
#include <Math/math.h>
#include <Utils/Logging.h>
#include "InputCodes.h"

#include <functional>

namespace Nebula
{
    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        KeyPressed, KeyReleased, KeyHeld, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    enum EventCategory
    {
        None = 0,
        EventCategoryWindow         = BIT(0),
        EventCategoryInput          = BIT(1),
        EventCategoryKeyboard       = BIT(2),
        EventCategoryMouse          = BIT(3),
        EventCategoryMouseButton    = BIT(4)
    };


    //Check the type and name of any event
    #define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return (EventType) type; } \
                                                        virtual EventType GetEventType() const override { return GetStaticType(); } \
                                                        virtual const char* GetName() const override { return #type; }

    #define EVENT_CLASS_CATEGORY(cat) virtual int GetCategoryFlags() const override { return cat; }

    class Event
    {
        friend class EventDispatcher;
    protected:
        bool eventHandled = false;

    public:
        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual const char* ToString() const { return GetName(); }

        inline bool IsInCategory(EventCategory cat)
        {
            return GetCategoryFlags() & cat;
        }

        inline bool IsHandled() const { return eventHandled; }
        inline void Handle() { eventHandled = true; }
    };


    class EventDispatcher
    {
        Event& curEvent;

        template<typename T>
        using EventFunc = std::function<bool(T&)>;

    public:
        EventDispatcher(Event& event)
            : curEvent(event)
        {
        }

        template<typename T, typename F>
        bool Dispatch(const F& func)
        {
            if (curEvent.GetEventType() == T::GetStaticType())
            {
                curEvent.eventHandled = func(static_cast<T&>(curEvent));
                return true;
            }
            return false;
        }

    };
}
