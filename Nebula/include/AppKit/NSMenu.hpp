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
// AppKit/NSMenu.hpp
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#include <Foundation/NSPrivate.hpp>
#include "AppKitPrivate.hpp"

namespace NS::Private::Class {
	_APPKIT_PRIVATE_DEF_CLS( NSMenu );
}

namespace NS::Private::Selector {
	_APPKIT_PRIVATE_DEF_SEL( 
		ns_menu_init_,
		"init" 
	);

	_APPKIT_PRIVATE_DEF_SEL( 
		ns_menu_initWithTitle_,
		"initWithTitle:" 
	);

	_APPKIT_PRIVATE_DEF_SEL( 
		ns_menu_addItemWithTitle_,
		"addItemWithTitle:action:keyEquivalent:" 
	);

	_APPKIT_PRIVATE_DEF_SEL( 
		ns_menu_addItem_,
		"addItem:" 
	);

	_APPKIT_PRIVATE_DEF_SEL( 
		ns_menu_setAutoenablesItems_,
		"setAutoenablesItems:" 
	);


}

namespace NS
{
	class MenuItem;

	class Menu : public Referencing< Menu >
	{
		public:
			static Menu*	alloc();
			Menu*			init();
			Menu*			initWithTitle( const String* pTitle );

			void			addItem( const MenuItem* pItem );
			MenuItem*		addItemWithTitle( const String* pTitle, SEL pSelector, const String* pKeyEquivalent );
	};
}


_NS_INLINE NS::Menu* NS::Menu::alloc()
{
	return _OBJ_C_ALLOC_NAME( NS::Menu, NSMenu );
}

_NS_INLINE NS::Menu* NS::Menu::init()
{
	return _OBJ_C_SEND( NS::Menu*, this, ns_menu_init_ );
}

_NS_INLINE NS::Menu* NS::Menu::initWithTitle( const String* pTitle )
{
	return _OBJ_C_SEND_V( NS::Menu*, this, ns_menu_initWithTitle_, pTitle );
}

_NS_INLINE void NS::Menu::addItem( const MenuItem* pItem )
{
	_OBJ_C_SEND_V( void, this, ns_menu_addItem_, pItem );
}

_NS_INLINE NS::MenuItem* NS::Menu::addItemWithTitle( const String* pTitle, SEL pSelector, const String* pKeyEquivalent )
{
	return _OBJ_C_SEND_V( NS::MenuItem*, this, ns_menu_addItemWithTitle_, pTitle, pSelector, pKeyEquivalent );
}

