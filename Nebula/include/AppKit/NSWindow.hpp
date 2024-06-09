/*
 *
 * Copyright 2020-2021 Apple Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// AppKit/NSWindow.hpp
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "AppKitPrivate.hpp"
#include "NSView.hpp"
#include <AppKit/NSEvent.hpp>
#include <Foundation/NSObject.hpp>
#include <Foundation/NSTypes.hpp>

#include <objc/runtime.h>
#include <iostream>

namespace NS::Private::Class {
	_APPKIT_PRIVATE_DEF_CLS( NSWindow );
}

namespace NS::Private::Selector {
	_APPKIT_PRIVATE_DEF_SEL( 
		ns_window_init_,
		"initWithContentRect:styleMask:backing:defer:" 
	);
	_APPKIT_PRIVATE_DEF_SEL( 
		ns_window_setContentView_,
		"setContentView:" 
	);
	_APPKIT_PRIVATE_DEF_SEL( 
		ns_window_contentView,
		"contentView" 
	);
	_APPKIT_PRIVATE_DEF_SEL( 
		ns_window_makeKeyAndOrderFront_,
		"makeKeyAndOrderFront:" 
	);
	_APPKIT_PRIVATE_DEF_SEL(
		ns_window_orderFrontRegardless_,
		"orderFrontRegardless"
	);
	_APPKIT_PRIVATE_DEF_SEL( 
		ns_window_setTitle_,
		"setTitle:" 
	);
	_APPKIT_PRIVATE_DEF_SEL( 
		ns_window_makeKeyWindow,
		"makeKeyWindow" 
	);
	_APPKIT_PRIVATE_DEF_SEL( 
		ns_window_close_,
		"close" 
	);
	_APPKIT_PRIVATE_DEF_SEL(
		ns_window_alloc_,
		"alloc"
	);
	_APPKIT_PRIVATE_DEF_SEL(
		ns_window_acceptsFirstResponder_,
		"acceptsFirstResponder"
	)
	_APPKIT_PRIVATE_DEF_SEL(
		ns_window_canBecomeKeyWindow_,
		"canBecomeKeyWindow"
	)
	_APPKIT_PRIVATE_DEF_SEL(
		ns_window_canBecomeMainWindow_,
		"canBecomeMainWindow"
	)
	_APPKIT_PRIVATE_DEF_SEL(
		ns_window_keyDown_,
		"keyDown:"
	)	
	_APPKIT_PRIVATE_DEF_SEL(
		ns_window_keyUp_,
		"keyUp:"
	)		

	_APPKIT_PRIVATE_DEF_SEL(
		ns_window_frame, 
		"frame"
	)

	_APPKIT_PRIVATE_DEF_SEL(
		ns_window_setFrame_,
		"setFrame:display:"
	)
}
namespace NS
{
	_NS_OPTIONS( NS::UInteger, WindowStyleMask )
	{
		WindowStyleMaskBorderless	   = 0,
		WindowStyleMaskTitled		   = ( 1 << 0 ),
		WindowStyleMaskClosable		 = ( 1 << 1 ),
		WindowStyleMaskMiniaturizable   = ( 1 << 2 ),
		WindowStyleMaskResizable		= ( 1 << 3 ),
		WindowStyleMaskTexturedBackground = ( 1 << 8 ),
		WindowStyleMaskUnifiedTitleAndToolbar = ( 1 << 12 ),
		WindowStyleMaskFullScreen	   = ( 1 << 14 ),
		WindowStyleMaskFullSizeContentView = ( 1 << 15 ),
		WindowStyleMaskUtilityWindow	= ( 1 << 4 ),
		WindowStyleMaskDocModalWindow   = ( 1 << 6 ),
		WindowStyleMaskNonactivatingPanel   = ( 1 << 7 ),
		WindowStyleMaskHUDWindow		= ( 1 << 13 )
	};

	_NS_ENUM( NS::UInteger, BackingStoreType )
	{
		BackingStoreRetained = 0,
		BackingStoreNonretained = 1,
		BackingStoreBuffered = 2
	};

	class Window : public Referencing< Window >
	{
		public:
			static Window*		alloc();
			Window*				init( CGRect contentRect, WindowStyleMask styleMask, BackingStoreType backing, bool defer );

			void				setContentView( const View* pContentView );
			NS::View*			contentView() const;
			void				makeKeyAndOrderFront( const Object* pSender );
			void 				makeKey();
			void 				orderFrontRegardless();
			void				setTitle( const String* pTitle );

			NS::Rect			frame();
			void 				setFrame( NS::Rect frame, bool display = true);

			void				close();
	};

}

// Custom getter functions that always return YES
inline BOOL getCanBecomeKeyWindow(id self, SEL _cmd) {
    return YES;
}

inline BOOL getCanBecomeMainWindow(id self, SEL _cmd) {
    return YES;
}

inline BOOL acceptsFirstResponder(id self, SEL _cmd) {
	return YES;
}

inline void customKeyDown(id self, SEL _cmd, id theEvent) {
    std::cout << "Key down event received." << std::endl;
}

inline void customKeyUp(id self, SEL _cmd, id theEvent) {
    std::cout << "Key up event received." << std::endl;
}

_NS_INLINE NS::Window* NS::Window::alloc()
{
    Class nsWindowClass = objc_getClass("NSWindow");

    // Define the attributes for the new properties
    objc_property_attribute_t type = { "T", "B" }; // BOOL type
    objc_property_attribute_t ownership = { "N", "" }; // nonatomic
    objc_property_attribute_t attrs[] = { type, ownership };

    IMP pre = class_replaceMethod(nsWindowClass, _APPKIT_PRIVATE_SEL(ns_window_canBecomeKeyWindow_), (IMP)getCanBecomeKeyWindow, "B@:");
    pre = class_replaceMethod(nsWindowClass, _APPKIT_PRIVATE_SEL(ns_window_canBecomeMainWindow_), (IMP)getCanBecomeMainWindow, "B@:");
    pre = class_replaceMethod(nsWindowClass, _APPKIT_PRIVATE_SEL(ns_window_acceptsFirstResponder_), (IMP)acceptsFirstResponder, "B@:");
	pre = class_replaceMethod(nsWindowClass, _APPKIT_PRIVATE_SEL(ns_window_keyDown_), (IMP)customKeyDown, "v@:@");
	pre = class_replaceMethod(nsWindowClass, _APPKIT_PRIVATE_SEL(ns_window_keyUp_), (IMP)customKeyUp, "v@:@");

	return _OBJ_C_SEND_S( NS::Window*, NSWindow, ns_window_alloc_);
}

_NS_INLINE NS::Window* NS::Window::init( CGRect contentRect, WindowStyleMask styleMask, BackingStoreType backing, bool defer )
{
	return _OBJ_C_SEND_V( NS::Window*, this, ns_window_init_, contentRect, styleMask, backing, defer );
}

_NS_INLINE void NS::Window::setContentView( const NS::View* pContentView )
{
	_OBJ_C_SEND_V( void, this, ns_window_setContentView_, pContentView );
}

_NS_INLINE NS::View* NS::Window::contentView() const
{
	return _OBJ_C_SEND( NS::View*, this, ns_window_contentView );
}

_NS_INLINE void NS::Window::makeKeyAndOrderFront( const Object* pSender )
{
	_OBJ_C_SEND_V( void, this, ns_window_makeKeyAndOrderFront_, pSender );
}

_NS_INLINE void NS::Window::makeKey()
{
	_OBJ_C_SEND( void, this, ns_window_makeKeyWindow );
}

_NS_INLINE void NS::Window::orderFrontRegardless() 
{
	_OBJ_C_SEND( void, this, ns_window_orderFrontRegardless_ );
}

_NS_INLINE void NS::Window::setTitle( const String* pTitle )
{
	_OBJ_C_SEND_V( void, this, ns_window_setTitle_, pTitle );
}

_NS_INLINE void NS::Window::close()
{
	_OBJ_C_SEND( void, this, ns_window_close_ );
}

_NS_INLINE NS::Rect NS::Window::frame()
{
	return _OBJ_C_SEND( NS::Rect, this, ns_window_frame );
}

_NS_INLINE void NS::Window::setFrame( NS::Rect frame, bool display)
{
	_OBJ_C_SEND_V( void, this, ns_window_setFrame_, frame, display ? YES : NO );
}