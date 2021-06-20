#pragma once

#include "Event.h"

namespace Nebula{
	class KeyEvent : public Event
		{
		protected:
			KeyEvent(int keycode)
				: keyCode(keycode) {}

			int keyCode;
		public:
			inline int GetKeyCode() const { return keyCode; }

			EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	};

	class KeyPressedEvent : public KeyEvent
	{
		int repeatCount;
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), repeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return repeatCount; }

		const char* ToString() const override
		{
			char* ss = {};
			sprintf(ss, "KeyPressedEvent: %d ( %d repeats)\n", keyCode, repeatCount);
			return ss;
		}

		EVENT_CLASS_TYPE(EventType::KeyPressed);
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {}

		const char* ToString() const override
		{
			char* ss = {};
			sprintf(ss, "KeyReleasedEvent: %d\n", keyCode);
			return ss;
		}

		EVENT_CLASS_TYPE(EventType::KeyReleased);
	};


	class KeyHeldEvent : public KeyEvent
	{
	public:
		KeyHeldEvent(int keycode)
			: KeyEvent(keycode) {}

		const char* ToString() const override
		{
			char* ss;
			sprintf(ss, "KeyHeldEvent: %d\n", keyCode);
			return ss;
		}

		EVENT_CLASS_TYPE(EventType::KeyHeld);
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode) {}

		EVENT_CLASS_TYPE(EventType::KeyTyped)
	};
}