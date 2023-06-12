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
// AppKit/NSMenuItem.hpp
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#include <Foundation/NSPrivate.hpp>
#include "AppKitPrivate.hpp"
#include <string>
namespace NS::Private::Class {
	_APPKIT_PRIVATE_DEF_CLS( NSMenuItem );
}

namespace NS::Private::Selector {
	_APPKIT_PRIVATE_DEF_SEL( 
		ns_menuItem_init_,
		"init" 
	);

	_APPKIT_PRIVATE_DEF_SEL( 
		ns_menuItem_keyEquivalentModifierMask_,
		"keyEquivalentModifierMask" 
	);

	_APPKIT_PRIVATE_DEF_SEL( 
		ns_menuItem_setKeyEquivalentModifierMask_,
		"setKeyEquivalentModifierMask:" 
	);

	_APPKIT_PRIVATE_DEF_SEL( 
		ns_menuItem__setSubmenu_,
		"setSubmenu:" 
	);
}

namespace NS
{

	typedef void (*MenuItemCallback)( void* unused, SEL name, const NS::Object* pSender );

	class MenuItem : public NS::Referencing< MenuItem >
	{
		public:
			static SEL						registerActionCallback( const char* name, MenuItemCallback callback );
			static MenuItem*				alloc();
			MenuItem*						init();

			void							setKeyEquivalentModifierMask( NS::EventModifierFlags modifierMask );
			NS::EventModifierFlags			keyEquivalentModifierMask() const;
			void							setSubmenu( const class Menu* pSubmenu );
	};
}


_NS_INLINE SEL NS::MenuItem::registerActionCallback( const char* name, NS::MenuItemCallback callback )
{
	auto siz = strlen( name );
	SEL sel;
	if ( ( siz > 0 ) && ( name[ siz - 1 ] != ':' ) )
	{
		char* colName = (char *)alloca( siz + 2 );
		memcpy( colName, name, siz );
		colName[ siz ] = ':';
		colName[ siz + 1 ] = '\0';
		sel = sel_registerName( colName );
	}
	else
	{
		sel = sel_registerName( name );
	}

	if ( callback )
	{
		class_addMethod( (Class)_APPKIT_PRIVATE_CLS( NSObject ), sel, (IMP)callback, "v@:@" );
	}
	return sel;
}

_NS_INLINE NS::MenuItem* NS::MenuItem::alloc()
{
	return _OBJ_C_ALLOC_NAME( NS::MenuItem, NSMenuItem );
}

_NS_INLINE NS::MenuItem* NS::MenuItem::init()
{
	return _OBJ_C_SEND( NS::MenuItem*, this, ns_menuItem_init_ );
}

_NS_INLINE void NS::MenuItem::setKeyEquivalentModifierMask( NS::EventModifierFlags modifierMask )
{
	_OBJ_C_SEND_V( void, this, ns_menuItem_setKeyEquivalentModifierMask_, modifierMask );
}

_NS_INLINE NS::EventModifierFlags NS::MenuItem::keyEquivalentModifierMask() const
{
	return _OBJ_C_SEND( NS::EventModifierFlags, this, ns_menuItem_keyEquivalentModifierMask_ );
}

_NS_INLINE void NS::MenuItem::setSubmenu( const class NS::Menu* pSubmenu )
{
	_OBJ_C_SEND_V( void, this, ns_menuItem__setSubmenu_, pSubmenu );
}
