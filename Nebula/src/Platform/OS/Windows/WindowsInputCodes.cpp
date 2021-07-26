#include "WindowsInput.h"

#include <Core/PlatformInfo.h>

namespace Nebula {
    std::unordered_map<int, KeyCode> WindowsInput::KeyCodeMapWinToNeb = {
        { VK_SPACE, KeyCode::Space},
		{ VK_OEM_7, KeyCode::Apostrophe}, /* ' */
		{ VK_OEM_COMMA, KeyCode::Comma}, /* , */
		{ VK_OEM_MINUS, KeyCode::Minus}, /* - */
		{ VK_OEM_PERIOD, KeyCode::Period}, /* . */
		{ VK_OEM_2, KeyCode::Slash}, /* / */
		{ 0x30, KeyCode::D0}, /* 0 */
		{ 0x31, KeyCode::D1}, /* 1 */
		{ 0x32, KeyCode::D2}, /* 2 */
		{ 0x33, KeyCode::D3}, /* 3 */
		{ 0x34, KeyCode::D4}, /* 4 */
		{ 0x35, KeyCode::D5}, /* 5 */
		{ 0x36, KeyCode::D6}, /* 6 */
		{ 0x37, KeyCode::D7}, /* 7 */
		{ 0x38, KeyCode::D8}, /* 8 */
		{ 0x39, KeyCode::D9}, /* 9 */
		{ VK_OEM_1, KeyCode::Semicolon}, /* ; */
		{ VK_OEM_PLUS, KeyCode::Equal}, /* = */
		{ 0x41, KeyCode::A},
		{ 0x42, KeyCode::B},
		{ 0x43, KeyCode::C},
		{ 0x44, KeyCode::D},
		{ 0x45, KeyCode::E},
		{ 0x46, KeyCode::F},
		{ 0x47, KeyCode::G},
		{ 0x48, KeyCode::H},
		{ 0x49, KeyCode::I},
		{ 0x4A, KeyCode::J},
		{ 0x4B, KeyCode::K},
		{ 0x4C, KeyCode::L},
		{ 0x4D, KeyCode::M},
		{ 0x4E, KeyCode::N},
		{ 0x4F, KeyCode::O},
		{ 0x50, KeyCode::P},
		{ 0x51, KeyCode::Q},
		{ 0x52, KeyCode::R},
		{ 0x53, KeyCode::S},
		{ 0x54, KeyCode::T},
		{ 0x55, KeyCode::U},
		{ 0x56, KeyCode::V},
		{ 0x57, KeyCode::W},
		{ 0x58, KeyCode::X},
		{ 0x59, KeyCode::Y},
		{ 0x5A, KeyCode::Z},
		{ VK_CONTROL, KeyCode::LeftControl},
		{ VK_SHIFT, KeyCode::LeftShift},
		{ VK_OEM_4, KeyCode::LeftBracket},  /* [ */
		{ VK_OEM_5, KeyCode::Backslash},  /* \ */
		{ VK_OEM_6, KeyCode::RightBracket},  /* ] */
		{ VK_OEM_3, KeyCode::GraveAccent},  /* ` */
		{ VK_ESCAPE, KeyCode::Escape},
		{ VK_RETURN, KeyCode::Enter},
		{ VK_TAB, KeyCode::Tab},
		{ VK_BACK, KeyCode::Backspace},
		{ VK_INSERT, KeyCode::Insert},
		{ VK_DELETE, KeyCode::Delete},
		{ VK_RIGHT, KeyCode::Right},
		{ VK_LEFT, KeyCode::Left},
		{ VK_DOWN, KeyCode::Down},
		{ VK_UP, KeyCode::Up},
		{ VK_PRIOR, KeyCode::PageUp},
		{ VK_NEXT, KeyCode::PageDown},
		{ VK_HOME, KeyCode::Home},
		{ VK_END, KeyCode::End},
		{ VK_CAPITAL, KeyCode::CapsLock},
		{ VK_SCROLL, KeyCode::ScrollLock},
		{ VK_NUMLOCK, KeyCode::NumLock},
		{ VK_SNAPSHOT, KeyCode::PrintScreen},
		{ VK_PAUSE, KeyCode::Pause},
		{ VK_F1, KeyCode::F1},
		{ VK_F2, KeyCode::F2},
		{ VK_F3, KeyCode::F3},
		{ VK_F4, KeyCode::F4},
		{ VK_F5, KeyCode::F5},
		{ VK_F6, KeyCode::F6},
		{ VK_F7, KeyCode::F7},
		{ VK_F8, KeyCode::F8},
		{ VK_F9, KeyCode::F9},
		{ VK_F10, KeyCode::F10},
		{ VK_F11, KeyCode::F11},
		{ VK_F12, KeyCode::F12},
		{ VK_F13, KeyCode::F13},
		{ VK_F14, KeyCode::F14},
		{ VK_F15, KeyCode::F15},
		{ VK_F16, KeyCode::F16},
		{ VK_F17, KeyCode::F17},
		{ VK_F18, KeyCode::F18},
		{ VK_F19, KeyCode::F19},
		{ VK_F20, KeyCode::F20},
		{ VK_F21, KeyCode::F21},
		{ VK_F22, KeyCode::F22},
		{ VK_F23, KeyCode::F23},
		{ VK_F24, KeyCode::F24},
		{ VK_NUMPAD0, KeyCode::KP0},
		{ VK_NUMPAD1, KeyCode::KP1},
		{ VK_NUMPAD2, KeyCode::KP2},
		{ VK_NUMPAD3, KeyCode::KP3},
		{ VK_NUMPAD4, KeyCode::KP4},
		{ VK_NUMPAD5, KeyCode::KP5},
		{ VK_NUMPAD6, KeyCode::KP6},
		{ VK_NUMPAD7, KeyCode::KP7},
		{ VK_NUMPAD8, KeyCode::KP8},
		{ VK_NUMPAD9, KeyCode::KP9},
		{ VK_DECIMAL, KeyCode::KPDecimal},
		{ VK_DIVIDE, KeyCode::KPDivide},
		{ VK_MULTIPLY, KeyCode::KPMultiply},
		{ VK_SUBTRACT, KeyCode::KPSubtract},
		{ VK_ADD, KeyCode::KPAdd},
		{ VK_RETURN, KeyCode::KPEnter},
		{ VK_LSHIFT, KeyCode::LeftShift},
		{ VK_LCONTROL, KeyCode::LeftControl},
		{ VK_LMENU, KeyCode::LeftAlt},
		{ VK_RSHIFT, KeyCode::RightShift},
		{ VK_RCONTROL, KeyCode::RightControl},
		{ VK_RMENU, KeyCode::RightAlt},
		{ VK_MENU, KeyCode::Menu}
    };

    std::unordered_map<KeyCode, int> WindowsInput::KeyCodeMapNebToWin = {
        { KeyCode::Space, VK_SPACE },
		{ KeyCode::Apostrophe, VK_OEM_7 }, /* ' */
		{ KeyCode::Comma, VK_OEM_COMMA }, /* , */
		{ KeyCode::Minus, VK_OEM_MINUS }, /* - */
		{ KeyCode::Period, VK_OEM_PERIOD }, /* . */
		{ KeyCode::Slash, VK_OEM_2 }, /* / */
		{ KeyCode::D0, 0x30 }, /* 0 */
		{ KeyCode::D1, 0x31 }, /* 1 */
		{ KeyCode::D2, 0x32 }, /* 2 */
		{ KeyCode::D3, 0x33 }, /* 3 */
		{ KeyCode::D4, 0x34 }, /* 4 */
		{ KeyCode::D5, 0x35 }, /* 5 */
		{ KeyCode::D6, 0x36 }, /* 6 */
		{ KeyCode::D7, 0x37 }, /* 7 */
		{ KeyCode::D8, 0x38 }, /* 8 */
		{ KeyCode::D9, 0x39 }, /* 9 */
		{ KeyCode::Semicolon, VK_OEM_1 }, /* ; */
		{ KeyCode::Equal, VK_OEM_PLUS }, /* = */
		{ KeyCode::A, 0x41 },
		{ KeyCode::B, 0x42 },
		{ KeyCode::C, 0x43 },
		{ KeyCode::D, 0x44 },
		{ KeyCode::E, 0x45 },
		{ KeyCode::F, 0x46 },
		{ KeyCode::G, 0x47 },
		{ KeyCode::H, 0x48 },
		{ KeyCode::I, 0x49 },
		{ KeyCode::J, 0x4A },
		{ KeyCode::K, 0x4B },
		{ KeyCode::L, 0x4C },
		{ KeyCode::M, 0x4D },
		{ KeyCode::N, 0x4E },
		{ KeyCode::O, 0x4F },
		{ KeyCode::P, 0x50 },
		{ KeyCode::Q, 0x51 },
		{ KeyCode::R, 0x52 },
		{ KeyCode::S, 0x53 },
		{ KeyCode::T, 0x54 },
		{ KeyCode::U, 0x55 },
		{ KeyCode::V, 0x56 },
		{ KeyCode::W, 0x57 },
		{ KeyCode::X, 0x58 },
		{ KeyCode::Y, 0x59 },
		{ KeyCode::Z, 0x5A },
		{ KeyCode::LeftBracket, VK_OEM_4 },  /* [ */
		{ KeyCode::Backslash, VK_OEM_5 },  /* \ */
		{ KeyCode::RightBracket, VK_OEM_6 },  /* ] */
		{ KeyCode::GraveAccent, VK_OEM_3 },  /* ` */
		{ KeyCode::Escape, VK_ESCAPE },
		{ KeyCode::Enter, VK_RETURN },
		{ KeyCode::Tab, VK_TAB },
		{ KeyCode::Backspace, VK_BACK },
		{ KeyCode::Insert, VK_INSERT },
		{ KeyCode::Delete, VK_DELETE },
		{ KeyCode::Right, VK_RIGHT },
		{ KeyCode::Left, VK_LEFT },
		{ KeyCode::Down, VK_DOWN },
		{ KeyCode::Up, VK_UP },
		{ KeyCode::PageUp, VK_PRIOR },
		{ KeyCode::PageDown, VK_NEXT },
		{ KeyCode::Home, VK_HOME },
		{ KeyCode::End, VK_END },
		{ KeyCode::CapsLock, VK_CAPITAL },
		{ KeyCode::ScrollLock, VK_SCROLL },
		{ KeyCode::NumLock, VK_NUMLOCK },
		{ KeyCode::PrintScreen, VK_SNAPSHOT },
		{ KeyCode::Pause, VK_PAUSE },
		{ KeyCode::LeftControl, VK_CONTROL},
		{ KeyCode::LeftShift, VK_SHIFT},
		{ KeyCode::F1, VK_F1 },
		{ KeyCode::F2, VK_F2 },
		{ KeyCode::F3, VK_F3 },
		{ KeyCode::F4, VK_F4 },
		{ KeyCode::F5, VK_F5 },
		{ KeyCode::F6, VK_F6 },
		{ KeyCode::F7, VK_F7 },
		{ KeyCode::F8, VK_F8 },
		{ KeyCode::F9, VK_F9 },
		{ KeyCode::F10, VK_F10 },
		{ KeyCode::F11, VK_F11 },
		{ KeyCode::F12, VK_F12 },
		{ KeyCode::F13, VK_F13 },
		{ KeyCode::F14, VK_F14 },
		{ KeyCode::F15, VK_F15 },
		{ KeyCode::F16, VK_F16 },
		{ KeyCode::F17, VK_F17 },
		{ KeyCode::F18, VK_F18 },
		{ KeyCode::F19, VK_F19 },
		{ KeyCode::F20, VK_F20 },
		{ KeyCode::F21, VK_F21 },
		{ KeyCode::F22, VK_F22 },
		{ KeyCode::F23, VK_F23 },
		{ KeyCode::F24, VK_F24 },
		{ KeyCode::KP0, VK_NUMPAD0 },
		{ KeyCode::KP1, VK_NUMPAD1 },
		{ KeyCode::KP2, VK_NUMPAD2 },
		{ KeyCode::KP3, VK_NUMPAD3 },
		{ KeyCode::KP4, VK_NUMPAD4 },
		{ KeyCode::KP5, VK_NUMPAD5 },
		{ KeyCode::KP6, VK_NUMPAD6 },
		{ KeyCode::KP7, VK_NUMPAD7 },
		{ KeyCode::KP8, VK_NUMPAD8 },
		{ KeyCode::KP9, VK_NUMPAD9 },
		{ KeyCode::KPDecimal, VK_DECIMAL },
		{ KeyCode::KPDivide, VK_DIVIDE },
		{ KeyCode::KPMultiply, VK_MULTIPLY },
		{ KeyCode::KPSubtract, VK_SUBTRACT },
		{ KeyCode::KPAdd, VK_ADD },
		{ KeyCode::KPEnter, VK_RETURN },
		{ KeyCode::LeftShift, VK_LSHIFT },
		{ KeyCode::LeftControl, VK_LCONTROL },
		{ KeyCode::LeftAlt, VK_LMENU },
		{ KeyCode::RightShift, VK_RSHIFT },
		{ KeyCode::RightControl, VK_RCONTROL },
		{ KeyCode::RightAlt, VK_RMENU },
		{ KeyCode::Menu, VK_MENU }
    };

    std::unordered_map<MouseCode, int> WindowsInput::MouseCodeMapNebToWin = {
        { MouseCode::ButtonNext,    VK_XBUTTON1 },
        { MouseCode::ButtonLast,    VK_XBUTTON2 },
        { MouseCode::ButtonLeft,    VK_LBUTTON },
        { MouseCode::ButtonRight,   VK_RBUTTON },
        { MouseCode::ButtonMiddle,  VK_MBUTTON }
    };

    std::unordered_map<int, MouseCode> WindowsInput::MouseCodeMapWinToNeb = {
        { 0x0020, MouseCode::ButtonNext },
        { 0x0040, MouseCode::ButtonLast },
        { 0x0001 , MouseCode::ButtonLeft },
        { 0x0002 , MouseCode::ButtonRight },
        { 0x0010 , MouseCode::ButtonMiddle }
    };
    
}