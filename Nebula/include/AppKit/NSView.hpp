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
// AppKit/NSView.hpp
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "AppKitPrivate.hpp"
#include <Foundation/Foundation.hpp>
#include <CoreGraphics/CGGeometry.h>

namespace NS::Private::Class {
	_APPKIT_PRIVATE_DEF_CLS( NSView );
}

namespace NS::Private::Selector {

	_APPKIT_PRIVATE_DEF_SEL( 
		ns_view_initWithFrame_,
		"initWithFrame:" 
	);
}
namespace NS
{

	class View : public NS::Referencing< View >
	{
		public:
			View* 	initWithFrame( CGRect frame );
	};
}


_NS_INLINE NS::View* NS::View::initWithFrame( CGRect frame )
{
	return _OBJ_C_SEND_S_V( View*, NSView, ns_view_initWithFrame_, frame );
}
