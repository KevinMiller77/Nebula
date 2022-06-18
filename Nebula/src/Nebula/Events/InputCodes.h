#pragma once

namespace Nebula{
	enum class MouseCode
	{
		Button0                = 0,
		Button1                = 1,
		Button2                = 2,
		Button3                = 3,
		Button4                = 4,
		Button5                = 5,
		Button6               = 6,
		Button7               = 7,
		ButtonNext			   = Button6,
		ButtonLast             = Button7,
		ButtonLeft             = Button0,
		ButtonRight            = Button1,
		ButtonMiddle           = Button2
	};

	enum class KeyCode
	{
		// From glfw3.h
		Space               = 32,
		Apostrophe          = 39, /* ' */
		Comma               = 44, /* , */
		Minus               = 45, /* - */
		Period              = 46, /* . */
		Slash               = 47, /* / */

		D0                  = 48, /* 0 */
		D1                  = 49, /* 1 */
		D2                  = 50, /* 2 */
		D3                  = 51, /* 3 */
		D4                  = 52, /* 4 */
		D5                  = 53, /* 5 */
		D6                  = 54, /* 6 */
		D7                  = 55, /* 7 */
		D8                  = 56, /* 8 */
		D9                  = 57, /* 9 */

		Semicolon           = 59, /* ; */
		Equal               = 61, /* = */

		A                   = 65,
		B                   = 66,
		C                   = 67,
		D                   = 68,
		E                   = 69,
		F                   = 70,
		G                   = 71,
		H                   = 72,
		I                   = 73,
		J                   = 74,
		K                   = 75,
		L                   = 76,
		M                   = 77,
		N                   = 78,
		O                   = 79,
		P                   = 80,
		Q                   = 81,
		R                   = 82,
		S                   = 83,
		T                   = 84,
		U                   = 85,
		V                   = 86,
		W                   = 87,
		X                   = 88,
		Y                   = 89,
		Z                   = 90,

		LeftBracket         = 91,  /* [ */
		Backslash           = 92,  /* \ */
		RightBracket        = 93,  /* ] */
		GraveAccent         = 96,  /* ` */

		World1              = 161, /* non-US #1 */
		World2              = 162, /* non-US #2 */

		/* Function keys */
		Escape              = 256,
		Enter               = 257,
		Tab                 = 258,
		Backspace           = 259,
		Insert              = 260,
		Delete              = 261,
		Right               = 262,
		Left                = 263,
		Down                = 264,
		Up                  = 265,
		PageUp              = 266,
		PageDown            = 267,
		Home                = 268,
		End                 = 269,
		CapsLock            = 280,
		ScrollLock          = 281,
		NumLock             = 282,
		PrintScreen         = 283,
		Pause               = 284,
		F1                  = 290,
		F2                  = 291,
		F3                  = 292,
		F4                  = 293,
		F5                  = 294,
		F6                  = 295,
		F7                  = 296,
		F8                  = 297,
		F9                  = 298,
		F10                 = 299,
		F11                 = 300,
		F12                 = 301,
		F13                 = 302,
		F14                 = 303,
		F15                 = 304,
		F16                 = 305,
		F17                 = 306,
		F18                 = 307,
		F19                 = 308,
		F20                 = 309,
		F21                 = 310,
		F22                 = 311,
		F23                 = 312,
		F24                 = 313,
		F25                 = 314,

		/* Keypad */
		KP0                 = 320,
		KP1                 = 321,
		KP2                 = 322,
		KP3                 = 323,
		KP4                 = 324,
		KP5                 = 325,
		KP6                 = 326,
		KP7                 = 327,
		KP8                 = 328,
		KP9                 = 329,
		KPDecimal           = 330,
		KPDivide            = 331,
		KPMultiply          = 332,
		KPSubtract          = 333,
		KPAdd               = 334,
		KPEnter             = 335,

		LeftShift           = 340,
		LeftControl         = 341,
		LeftSuper           = 343,
		RightShift          = 344,
		RightControl        = 345,
		RightSuper          = 347,
		Alt             = 342
	};

	// From Windows
	// All of these must be defined for each platform
	#define KEY_SPACE           KeyCode::Space
	#define KEY_APOSTROPHE      KeyCode::Apostrophe    /* ' */
	#define KEY_COMMA           KeyCode::Comma         /* , */
	#define KEY_MINUS           KeyCode::Minus         /* - */
	#define KEY_PERIOD          KeyCode::Period        /* . */
	#define KEY_SLASH           KeyCode::Slash         /* / */
	#define KEY_0               KeyCode::D0
	#define KEY_1               KeyCode::D1
	#define KEY_2               KeyCode::D2
	#define KEY_3               KeyCode::D3
	#define KEY_4               KeyCode::D4
	#define KEY_5               KeyCode::D5
	#define KEY_6               KeyCode::D6
	#define KEY_7               KeyCode::D7
	#define KEY_8               KeyCode::D8
	#define KEY_9               KeyCode::D9
	#define KEY_SEMICOLON       KeyCode::Semicolon     /* ; */
	#define KEY_EQUAL           KeyCode::Equal         /* = */
	#define KEY_A               KeyCode::A
	#define KEY_B               KeyCode::B
	#define KEY_C               KeyCode::C
	#define KEY_D               KeyCode::D
	#define KEY_E               KeyCode::E
	#define KEY_F               KeyCode::F
	#define KEY_G               KeyCode::G
	#define KEY_H               KeyCode::H
	#define KEY_I               KeyCode::I
	#define KEY_J               KeyCode::J
	#define KEY_K               KeyCode::K
	#define KEY_L               KeyCode::L
	#define KEY_M               KeyCode::M
	#define KEY_N               KeyCode::N
	#define KEY_O               KeyCode::O
	#define KEY_P               KeyCode::P
	#define KEY_Q               KeyCode::Q
	#define KEY_R               KeyCode::R
	#define KEY_S               KeyCode::S
	#define KEY_T               KeyCode::T
	#define KEY_U               KeyCode::U
	#define KEY_V               KeyCode::V
	#define KEY_W               KeyCode::W
	#define KEY_X               KeyCode::X
	#define KEY_Y               KeyCode::Y
	#define KEY_Z               KeyCode::Z
	#define KEY_LEFT_BRACKET    KeyCode::LeftBracket   /* [ */
	#define KEY_BACKSLASH       KeyCode::Backslash     /* \ */
	#define KEY_RIGHT_BRACKET   KeyCode::RightBracket  /* ] */
	#define KEY_GRAVE_ACCENT    KeyCode::GraveAccent   /* ` */

	/* Function keys */
	#define KEY_ESCAPE          KeyCode::Escape
	#define KEY_ENTER           KeyCode::Enter
	#define KEY_TAB             KeyCode::Tab
	#define KEY_BACKSPACE       KeyCode::Backspace
	#define KEY_INSERT          KeyCode::Insert
	#define KEY_DELETE          KeyCode::Delete
	#define KEY_RIGHT           KeyCode::Right
	#define KEY_LEFT            KeyCode::Left
	#define KEY_DOWN            KeyCode::Down
	#define KEY_UP              KeyCode::Up
	#define KEY_PAGE_UP         KeyCode::PageUp
	#define KEY_PAGE_DOWN       KeyCode::PageDown
	#define KEY_HOME            KeyCode::Home
	#define KEY_END             KeyCode::End
	#define KEY_CAPS_LOCK       KeyCode::CapsLock
	#define KEY_SCROLL_LOCK     KeyCode::ScrollLock
	#define KEY_NUM_LOCK        KeyCode::NumLock
	#define KEY_PRINT_SCREEN    KeyCode::PrintScreen
	#define KEY_PAUSE           KeyCode::Pause
	#define KEY_F1              KeyCode::F1
	#define KEY_F2              KeyCode::F2
	#define KEY_F3              KeyCode::F3
	#define KEY_F4              KeyCode::F4
	#define KEY_F5              KeyCode::F5
	#define KEY_F6              KeyCode::F6
	#define KEY_F7              KeyCode::F7
	#define KEY_F8              KeyCode::F8
	#define KEY_F9              KeyCode::F9
	#define KEY_F10             KeyCode::F10
	#define KEY_F11             KeyCode::F11
	#define KEY_F12             KeyCode::F12
	#define KEY_F13             KeyCode::F13
	#define KEY_F14             KeyCode::F14
	#define KEY_F15             KeyCode::F15
	#define KEY_F16             KeyCode::F16
	#define KEY_F17             KeyCode::F17
	#define KEY_F18             KeyCode::F18
	#define KEY_F19             KeyCode::F19
	#define KEY_F20             KeyCode::F20
	#define KEY_F21             KeyCode::F21
	#define KEY_F22             KeyCode::F22
	#define KEY_F23             KeyCode::F23
	#define KEY_F24             KeyCode::F24
	#define KEY_F25             KeyCode::F25

	/* Keypad */
	#define KEY_KP_0            KeyCode::KP0
	#define KEY_KP_1            KeyCode::KP1
	#define KEY_KP_2            KeyCode::KP2
	#define KEY_KP_3            KeyCode::KP3
	#define KEY_KP_4            KeyCode::KP4
	#define KEY_KP_5            KeyCode::KP5
	#define KEY_KP_6            KeyCode::KP6
	#define KEY_KP_7            KeyCode::KP7
	#define KEY_KP_8            KeyCode::KP8
	#define KEY_KP_9            KeyCode::KP9
	#define KEY_KP_DECIMAL      KeyCode::KPDecimal
	#define KEY_KP_DIVIDE       KeyCode::KPDivide
	#define KEY_KP_MULTIPLY     KeyCode::KPMultiply
	#define KEY_KP_SUBTRACT     KeyCode::KPSubtract
	#define KEY_KP_ADD          KeyCode::KPAdd
	#define KEY_KP_ENTER        KeyCode::KPEnter
	#define KEY_KP_EQUAL        KeyCode::KPEqual

	#define KEY_LEFT_SHIFT      KeyCode::LeftShift
	#define KEY_LEFT_CONTROL    KeyCode::LeftControl
	#define KEY_LEFT_SUPER      KeyCode::LeftSuper

	#define KEY_ALT        KeyCode::Alt
    
	#define MOUSE_BUTTON_0      MouseCode::Button0
	#define MOUSE_BUTTON_1      MouseCode::Button1
	#define MOUSE_BUTTON_2      MouseCode::Button2
	#define MOUSE_BUTTON_3      MouseCode::Button3
	#define MOUSE_BUTTON_4      MouseCode::Button4
	#define MOUSE_BUTTON_5      MouseCode::Button5
	#define MOUSE_BUTTON_6     	MouseCode::Button6
	#define MOUSE_BUTTON_7     	MouseCode::Button7
	#define MOUSE_BUTTON_NEXT	MouseCode::ButtonNext
	#define MOUSE_BUTTON_LAST   MouseCode::ButtonLast
	#define MOUSE_BUTTON_LEFT   MouseCode::ButtonLeft
	#define MOUSE_BUTTON_RIGHT  MouseCode::ButtonRight
	#define MOUSE_BUTTON_MIDDLE MouseCode::ButtonMiddle
}