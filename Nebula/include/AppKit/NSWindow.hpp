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
#include <Foundation/NSObject.hpp>

#include <CoreGraphics/CGGeometry.h>

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
		ns_window_makeKeyAndOrderFront_,
		"makeKeyAndOrderFront:" 
	);
	_APPKIT_PRIVATE_DEF_SEL( 
		ns_window_setTitle_,
		"setTitle:" 
	);
	_APPKIT_PRIVATE_DEF_SEL( 
		ns_window_close_,
		"close" 
	);
	_APPKIT_PRIVATE_DEF_SEL(
		ns_window_alloc_,
		"alloc"
	);
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
			void				makeKeyAndOrderFront( const Object* pSender );
			void				setTitle( const String* pTitle );

			void				close();
	};

}


_NS_INLINE NS::Window* NS::Window::alloc()
{
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

_NS_INLINE void NS::Window::makeKeyAndOrderFront( const Object* pSender )
{
	_OBJ_C_SEND_V( void, this, ns_window_makeKeyAndOrderFront_, pSender );
}

_NS_INLINE void NS::Window::setTitle( const String* pTitle )
{
	_OBJ_C_SEND_V( void, this, ns_window_setTitle_, pTitle );
}

_NS_INLINE void NS::Window::close()
{
	_OBJ_C_SEND( void, this, ns_window_close_ );
}
