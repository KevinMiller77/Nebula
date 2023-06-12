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
// AppKit/AppKitPrivate.hpp
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#include <objc/runtime.h>

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#define _APPKIT_PRIVATE_CLS( symbol )				   ( Private::Class::s_k ## symbol )
#define _APPKIT_PRIVATE_SEL( accessor )				 ( Private::Selector::s_k ## accessor )

#define _OBJ_C_ALLOC_NAME( type, symbol )	Object::alloc<type>( _APPKIT_PRIVATE_CLS( symbol ))
// Referencing self, with or without variables
#define _OBJ_C_SEND( ret, symbol, accessor) Object::sendMessage<ret>(symbol, _APPKIT_PRIVATE_SEL(accessor) )
#define _OBJ_C_SEND_V( ret, symbol, accessor, ...) Object::sendMessage<ret>(symbol, _APPKIT_PRIVATE_SEL(accessor), __VA_ARGS__ )

// Referencing a static class, with or without variables
#define _OBJ_C_SEND_S( ret, symbol, accessor) Object::sendMessage<ret>(_APPKIT_PRIVATE_CLS(symbol), _APPKIT_PRIVATE_SEL(accessor) )
#define _OBJ_C_SEND_S_V( ret, symbol, accessor, ...) Object::sendMessage<ret>(_APPKIT_PRIVATE_CLS(symbol), _APPKIT_PRIVATE_SEL(accessor), __VA_ARGS__ )

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#if defined( NS_PRIVATE_IMPLEMENTATION )

#define _APPKIT_PRIVATE_VISIBILITY						__attribute__( ( visibility( "default" ) ) )
#define _APPKIT_PRIVATE_IMPORT						  __attribute__( ( weak_import ) )

#if __OBJC__
#define  _APPKIT_PRIVATE_OBJC_LOOKUP_CLASS( symbol  )   ( ( __bridge void* ) objc_lookUpClass( # symbol ) )
#else
#define  _APPKIT_PRIVATE_OBJC_LOOKUP_CLASS( symbol  )   objc_lookUpClass( # symbol ) 
#endif // __OBJC__

#define _APPKIT_PRIVATE_DEF_CLS( symbol )				void*				   s_k ## symbol 	_NS_PRIVATE_VISIBILITY = _NS_PRIVATE_OBJC_LOOKUP_CLASS( symbol );
#define _APPKIT_PRIVATE_DEF_SEL( accessor, symbol )	 SEL					 s_k ## accessor	_NS_PRIVATE_VISIBILITY = sel_registerName( symbol );
#define _APPKIT_PRIVATE_DEF_CONST( type, symbol )	   _NS_EXTERN type const   NS ## symbol   _NS_PRIVATE_IMPORT; \
													type const			  NS::symbol	 = ( nullptr != &NS ## symbol ) ? NS ## symbol : nullptr;


#else

#define _APPKIT_PRIVATE_DEF_CLS( symbol )				extern void*			s_k ## symbol;
#define _APPKIT_PRIVATE_DEF_SEL( accessor, symbol )	 extern SEL			  s_k ## accessor;
#define _APPKIT_PRIVATE_DEF_CONST( type, symbol )

#endif // NS_PRIVATE_IMPLEMENTATION

//-------------------------------------------------------------------------------------------------------------------------------------------------------------