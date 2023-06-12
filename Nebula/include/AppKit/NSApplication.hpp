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
// AppKit/NSApplication.hpp
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#include <Foundation/Foundation.hpp>
#include "AppKitPrivate.hpp"

// Objective-C Binding classes and selectors
namespace NS::Private::Class {
	_APPKIT_PRIVATE_DEF_CLS( NSApplication );
}

namespace NS::Private::Selector {
	_APPKIT_PRIVATE_DEF_SEL( 
		ns_application_applicationDidFinishLaunching_,
		"applicationDidFinishLaunching:" 
	);

	_APPKIT_PRIVATE_DEF_SEL( 
		ns_application_applicationShouldTerminateAfterLastWindowClosed_,
		"applicationShouldTerminateAfterLastWindowClosed:" 
	);

	_APPKIT_PRIVATE_DEF_SEL( 
		ns_application_applicationWillFinishLaunching_,
		"applicationWillFinishLaunching:" 
	);

	_APPKIT_PRIVATE_DEF_SEL( 
		ns_application_sharedApplication_,
		"sharedApplication" 
	);

	_APPKIT_PRIVATE_DEF_SEL( 
		ns_application_setDelegate_,
		"setDelegate:" 
	);

	_APPKIT_PRIVATE_DEF_SEL( 
		ns_application_setActivationPolicy_,
		"setActivationPolicy:" 
	);

	_APPKIT_PRIVATE_DEF_SEL( 
		ns_application_activateIgnoringOtherApps_,
		"activateIgnoringOtherApps:" 
	);

	_APPKIT_PRIVATE_DEF_SEL( 
		ns_application_setMainMenu_,
		"setMainMenu:" 
	);

	_APPKIT_PRIVATE_DEF_SEL( 
		ns_application_windows_,
		"windows" 
	);

	_APPKIT_PRIVATE_DEF_SEL( 
		ns_application_run_,
		"run" 
	);

	_APPKIT_PRIVATE_DEF_SEL( 
		ns_application_terminate_,
		"terminate:" 
	);

	_APPKIT_PRIVATE_DEF_SEL( 
		ns_application_stop_,
		"stop:" 
	);

	_APPKIT_PRIVATE_DEF_SEL( 
		ns_application_nextEventMatchingMask_,
		"nextEventMatchingMask:untilDate:inMode:dequeue:"
	);

	_APPKIT_PRIVATE_DEF_SEL( 
		ns_application_sendEvent_,
		"sendEvent:" 
	);
}

namespace NS
{
	/* Enums and Objects */
	_NS_ENUM( NS::UInteger, ActivationPolicy )
	{
		ActivationPolicyRegular,
		ActivationPolicyAccessory,
		ActivationPolicyProhibited
	};

	/* Classes */
	class ApplicationDelegate
	{
		public:
			virtual					~ApplicationDelegate() { }
			virtual void			applicationWillFinishLaunching( Notification* pNotification ) { }
			virtual void			applicationDidFinishLaunching( Notification* pNotification ) { }
			virtual bool			applicationShouldTerminateAfterLastWindowClosed( class Application* pSender ) { return false; }
	};

	class Application : public NS::Referencing< Application >
	{
		public:
			static Application*		sharedApplication();

			void 		setDelegate( const ApplicationDelegate* pDelegate );

			bool		setActivationPolicy( ActivationPolicy activationPolicy );
			
			void		activateIgnoringOtherApps( bool ignoreOtherApps );
			
			void		setMainMenu( const class Menu* pMenu );
			
			NS::Array*	windows() const;
			
			void		run();
			void		stop( const Object* pSender );
			void		terminate( const Object* pSender );
			
			NS::Event*	nextEventMatchingMask( NS::UInteger mask, NS::Date* pUntilDate, const NS::String* pInMode, bool deqFlag );
			void 		sendEvent( const NS::Event* pEvent );
	};

}

_NS_INLINE NS::Application* NS::Application::sharedApplication()
{
	return _OBJ_C_SEND_S(Application*, NSApplication, ns_application_sharedApplication_);
}

_NS_INLINE void NS::Application::setDelegate( const ApplicationDelegate* pAppDelegate )
{
	// TODO: Use a more suitable Object instead of NS::Value?
	// NOTE: this pWrapper is only held with a weak reference
	NS::Value* pWrapper = NS::Value::value( pAppDelegate );

	typedef void (*DispatchFunction)( NS::Value*, SEL, void* );
	
	DispatchFunction willFinishLaunching = []( Value* pSelf, SEL, void* pNotification ){
		auto pDel = reinterpret_cast< NS::ApplicationDelegate* >( pSelf->pointerValue() );
		pDel->applicationWillFinishLaunching( (NS::Notification *)pNotification );
	};

	DispatchFunction didFinishLaunching = []( Value* pSelf, SEL, void* pNotification ){
		auto pDel = reinterpret_cast< NS::ApplicationDelegate* >( pSelf->pointerValue() );
		pDel->applicationDidFinishLaunching( (NS::Notification *)pNotification );
	};

	DispatchFunction shouldTerminateAfterLastWindowClosed = []( Value* pSelf, SEL, void* pApplication ){
		auto pDel = reinterpret_cast< NS::ApplicationDelegate* >( pSelf->pointerValue() );
		pDel->applicationShouldTerminateAfterLastWindowClosed( (NS::Application *)pApplication );
	};

	class_addMethod( (Class)_APPKIT_PRIVATE_CLS( NSValue ), _APPKIT_PRIVATE_SEL( ns_application_applicationWillFinishLaunching_ ), (IMP)willFinishLaunching, "v@:@" );
	class_addMethod( (Class)_APPKIT_PRIVATE_CLS( NSValue ), _APPKIT_PRIVATE_SEL( ns_application_applicationDidFinishLaunching_ ), (IMP)didFinishLaunching, "v@:@" );
	class_addMethod( (Class)_APPKIT_PRIVATE_CLS( NSValue ), _APPKIT_PRIVATE_SEL( ns_application_applicationShouldTerminateAfterLastWindowClosed_), (IMP)shouldTerminateAfterLastWindowClosed, "B@:@" );

	_OBJ_C_SEND_V( void, this, ns_application_setDelegate_, pWrapper);
}

_NS_INLINE bool NS::Application::setActivationPolicy( ActivationPolicy activationPolicy )
{
	return _OBJ_C_SEND_V( bool, this, ns_application_setActivationPolicy_, activationPolicy );
}

_NS_INLINE void NS::Application::activateIgnoringOtherApps( bool ignoreOtherApps )
{
	_OBJ_C_SEND_V( void, this, ns_application_activateIgnoringOtherApps_, (ignoreOtherApps ? YES : NO) );
}

_NS_INLINE void NS::Application::setMainMenu( const class Menu* pMenu )
{
	_OBJ_C_SEND_V(void, this, ns_application_setMainMenu_, pMenu);
}

_NS_INLINE NS::Array* NS::Application::windows() const
{
	return _OBJ_C_SEND( NS::Array*, this, ns_application_windows_ );
}

_NS_INLINE void NS::Application::run()
{
	_OBJ_C_SEND( void, this, ns_application_run_ );
}


_NS_INLINE void NS::Application::terminate( const Object* pSender )
{
	_OBJ_C_SEND_V( void, this, ns_application_terminate_, pSender );
}

_NS_INLINE void NS::Application::stop(const Object* pSender)
{
	_OBJ_C_SEND_V( void, this, ns_application_stop_, pSender );
}

_NS_INLINE NS::Event* NS::Application::nextEventMatchingMask( NS::UInteger mask, NS::Date* pUntilDate, const NS::String* pInMode, bool deqFlag )
{
	return _OBJ_C_SEND_V( NS::Event*, this, ns_application_nextEventMatchingMask_, mask, pUntilDate, pInMode, (deqFlag ? YES : NO) );
}

_NS_INLINE void NS::Application::sendEvent( const NS::Event* pEvent )
{
	_OBJ_C_SEND_V( void, this, ns_application_sendEvent_, pEvent );
}