/*
    Author: Kevin Miller
    Date Created: 10/09/2021
*/


//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// AppKit/NSEvent.hpp
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#include <Foundation/Foundation.hpp>
#include "AppKitPrivate.hpp"

#include "NSWindow.hpp"


namespace NS::Private::Class {
    _APPKIT_PRIVATE_DEF_CLS( NSEvent );
}

namespace NS::Private::Selector {
    _APPKIT_PRIVATE_DEF_SEL( ns_event_type_, "type" );
    _APPKIT_PRIVATE_DEF_SEL( ns_event_subtype_, "subtype" );
    // _APPKIT_PRIVATE_DEF_SEL( modifierFlags_, "modifierFlags" );
    // _APPKIT_PRIVATE_DEF_SEL( timestamp_, "timestamp" );
    // _APPKIT_PRIVATE_DEF_SEL( window_, "window" );
    // _APPKIT_PRIVATE_DEF_SEL( windowNumber_, "windowNumber" );
}

namespace NS {
    _NS_ENUM ( NS::UInteger, EventType) {
        EventTypeLeftMouseDown             = 1,
        EventTypeLeftMouseUp               = 2,
        EventTypeRightMouseDown            = 3,
        EventTypeRightMouseUp              = 4,
        EventTypeMouseMoved                = 5,
        EventTypeRightMouseDragged         = 7,
        EventTypeLeftMouseDragged          = 6,
        EventTypeMouseExited               = 9,
        EventTypeMouseEntered              = 8,
        EventTypeKeyUp                     = 11,
        EventTypeKeyDown                   = 10,
        EventTypeAppKitDefined             = 13,
        EventTypeFlagsChanged              = 12,
        EventTypeApplicationDefined        = 15,
        EventTypeSystemDefined             = 14,
        EventTypeCursorUpdate              = 17,
        EventTypePeriodic                  = 16,
        EventTypeTabletPoint               = 23,
        EventTypeScrollWheel               = 22,
        EventTypeOtherMouseDown            = 25,
        EventTypeTabletProximity           = 24,
        EventTypeOtherMouseDragged         = 27,
        EventTypeOtherMouseUp              = 26,

        /* The following event types are available on some hardware on 10.5.2 and later */
        EventTypeGesture                  = 29,
        EventTypeMagnify                  = 30,
        EventTypeSwipe                    = 31,
        EventTypeRotate                   = 18,
        EventTypeBeginGesture             = 19,
        EventTypeEndGesture               = 20,
        EventTypeSmartMagnify             = 32,
        EventTypeQuickLook                = 33,
        EventTypePressure                 = 34,
        EventTypeDirectTouch              = 37,
        EventTypeChangeMode               = 38,
    };

    // For APIs introduced in Mac OS X 10.6 and later, this type is used with NS*Mask constants to indicate the events of interest.
    _NS_OPTIONS( unsigned long long, EventMask ) {
        EventMaskLeftMouseDown         = 1ULL << EventTypeLeftMouseDown,
        EventMaskLeftMouseUp           = 1ULL << EventTypeLeftMouseUp,
        EventMaskRightMouseDown        = 1ULL << EventTypeRightMouseDown,
        EventMaskRightMouseUp          = 1ULL << EventTypeRightMouseUp,
        EventMaskMouseMoved            = 1ULL << EventTypeMouseMoved,
        EventMaskLeftMouseDragged      = 1ULL << EventTypeLeftMouseDragged,
        EventMaskRightMouseDragged     = 1ULL << EventTypeRightMouseDragged,
        EventMaskMouseEntered          = 1ULL << EventTypeMouseEntered,
        EventMaskMouseExited           = 1ULL << EventTypeMouseExited,
        EventMaskKeyDown               = 1ULL << EventTypeKeyDown,
        EventMaskKeyUp                 = 1ULL << EventTypeKeyUp,
        EventMaskFlagsChanged          = 1ULL << EventTypeFlagsChanged,
        EventMaskAppKitDefined         = 1ULL << EventTypeAppKitDefined,
        EventMaskSystemDefined         = 1ULL << EventTypeSystemDefined,
        EventMaskApplicationDefined    = 1ULL << EventTypeApplicationDefined,
        EventMaskPeriodic              = 1ULL << EventTypePeriodic,
        EventMaskCursorUpdate          = 1ULL << EventTypeCursorUpdate,
        EventMaskScrollWheel           = 1ULL << EventTypeScrollWheel,
        EventMaskTabletPoint           = 1ULL << EventTypeTabletPoint,
        EventMaskTabletProximity       = 1ULL << EventTypeTabletProximity,
        EventMaskOtherMouseDown        = 1ULL << EventTypeOtherMouseDown,
        EventMaskOtherMouseUp          = 1ULL << EventTypeOtherMouseUp,
        EventMaskOtherMouseDragged     = 1ULL << EventTypeOtherMouseDragged,
        /* The following event masks are available on some hardware on 10.5.2 and later */
        EventMaskGesture               = 1ULL << EventTypeGesture,
        EventMaskMagnify               = 1ULL << EventTypeMagnify,
        EventMaskSwipe                 = 1ULL << EventTypeSwipe,
        EventMaskRotate                = 1ULL << EventTypeRotate,
        EventMaskBeginGesture          = 1ULL << EventTypeBeginGesture,
        EventMaskEndGesture            = 1ULL << EventTypeEndGesture,
        
        /* Note: You can only use these event masks on 64 bit. In other words, you cannot setup a local, nor global, event monitor for these event types on 32 bit. Also, you cannot search the event queue for them (nextEventMatchingMask:...) on 32 bit.
        */
        EventMaskSmartMagnify     = 1ULL << EventTypeSmartMagnify,
        EventMaskPressure         = 1ULL << EventTypePressure,
        EventMaskDirectTouch      = 1ULL << EventTypeDirectTouch,

        EventMaskChangeMode       = 1ULL << EventTypeChangeMode,
        
        EventMaskAny              = UIntegerMax,
    };

    /* Device-independent bits found in event modifier flags */
    _NS_OPTIONS( NS::UInteger, EventModifierFlags ) {
        EventModifierFlagCapsLock         = 1 << 16,
        EventModifierFlagShift            = 1 << 17,
        EventModifierFlagControl          = 1 << 18,
        EventModifierFlagOption           = 1 << 19,
        EventModifierFlagCommand          = 1 << 20,
        EventModifierFlagNumericPad       = 1 << 21,
        EventModifierFlagHelp             = 1 << 22,
        EventModifierFlagFunction         = 1 << 23,
        EventModifierFlagDeviceIndependentFlagsMask    = 0xffff0000UL,
    };

    _NS_OPTIONS( NS::UInteger, EventPhase ) {
        EventPhaseNone        = 0, // event not associated with a phase.
        EventPhaseBegan       = 0x1 << 0,
        EventPhaseStationary  = 0x1 << 1,
        EventPhaseChanged     = 0x1 << 2,
        EventPhaseEnded       = 0x1 << 3,
        EventPhaseCancelled   = 0x1 << 4,
        EventPhaseMayBegin    = 0x1 << 5,
    };

    _NS_ENUM( NS::Integer, EventGestureAxis ) {
        EventGestureAxisNone = 0,
        EventGestureAxisHorizontal,
        EventGestureAxisVertical
    };

    _NS_OPTIONS( NS::UInteger, EventSwipeTrackingOptions ) {
        EventSwipeTrackingLockDirection = 0x1 << 0, // Clamp gestureAmount to 0 if the user starts to swipe in the opposite direction than they started.
        EventSwipeTrackingClampGestureAmount = 0x1 << 1  // Don't allow gestureAmount to go beyond +/-1.0
    };

    _NS_ENUM( short, EventSubtype ) {
        /* event subtypes for NSEventTypeAppKitDefined events */
        EventSubtypeWindowExposed            = 0,
        EventSubtypeApplicationActivated     = 1,
        EventSubtypeApplicationDeactivated   = 2,
        EventSubtypeWindowMoved              = 4,
        EventSubtypeScreenChanged            = 8,
        
        /* event subtypes for NSEventTypeSystemDefined events */
        EventSubtypePowerOff             = 1,
    };

    _NS_ENUM( NS::Integer, PressureBehavior ) {
        PressureBehaviorUnknown = -1,
        
        // The default primary behavior when otherwise not explicitly configured. In 10.10.3 the default is PressureBehaviorPrimaryDeepClick.
        PressureBehaviorPrimaryDefault = 0,
        
        // Variable pressure with a small dynamic range after mouseDown.
        // Mouse events: LeftMouse
        // # of Stages: 1
        // Allowed stage transitions: 1->0
        // Actuations: mouseDown, mouseUp
        // stageTransition: NO
        PressureBehaviorPrimaryClick = 1,
        
        // Variable pressure with a large dynamic range after mouseDown suitable for drawing and general pressure use.
        // Mouse events: LeftMouse
        // # of Stages: 1
        // Allowed stage transitions: 1->0
        // Actuations: mouseDown, mouseUp
        // stageTransition: NO
        PressureBehaviorPrimaryGeneric = 2,
        
        // Variable pressure after mouseDown suitable for controlling speed. Used by AcceleratorButton
        // Mouse events: LeftMouse
        // # of Stages: 1
        // Allowed stage transitions: 1->0
        // Actuations: mouseDown, mouseUp
        // stageTransition: NO
        PressureBehaviorPrimaryAccelerator = 3,
        
        // Toggle between stages 1 and 2 until drag or mouse up (e.g. Force Click)
        // Mouse events: LeftMouse
        // # of Stages: 2 (stage 2 is disabled once dragging starts)
        // Allowed stage transitions: 1->0, 1->2, 2->1, 2->0
        // Actuations: mouseDown, mouseUp, 1->2, 2->1
        // stageTransition: into and release from stage 2
        PressureBehaviorPrimaryDeepClick = 5,
        
        // Toggle between stages 1 and 2 until mouse up (e.g. Force Click)
        // Mouse events: LeftMouse
        // # of Stages: 2
        // Allowed stage transitions: 1->0, 1->2, 2->1, 2->0
        // Actuations: mouseDown, mouseUp, 1->2, 2->1
        // stageTransition: into and release from stage 2
        PressureBehaviorPrimaryDeepDrag = 6,
    };

    class Event : public NS::Referencing< Event > {
    public:
        NS::EventType type();
        EventSubtype subtype();
        // NS::EventModifierFlags modifierFlags();
        // NS::TimeInterval timestamp();
        // NS::Window* window();
        // NS::Integer windowNumber();

    };
}

_NS_INLINE NS::EventType NS::Event::type() {
    return _OBJ_C_SEND( NS::EventType, this, ns_event_type_ );
}   

_NS_INLINE NS::EventSubtype NS::Event::subtype() {
    return _OBJ_C_SEND( NS::EventSubtype, this, ns_event_subtype_ );
}