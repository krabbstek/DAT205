#pragma once

#include "Common.h"

#include "Event.h"

namespace core {

	class CORE_API KeyEvent : public Event
	{
	public:
		virtual ~KeyEvent() {}

		int GetKeyCode() const { return m_KeyCode; }

	protected:
		KeyEvent(int keycode) : m_KeyCode(keycode) {}

		int m_KeyCode;
	};


	class CORE_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode) : KeyEvent(keycode) {}
		~KeyPressedEvent() {}

		EventType GetEventType() const { return GetEventTypeStatic(); }
		static EventType GetEventTypeStatic() { return EventType::KeyPressed; }
	};

	class CORE_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}
		~KeyReleasedEvent() {}

		EventType GetEventType() const { return GetEventTypeStatic(); }
		static EventType GetEventTypeStatic() { return EventType::KeyReleased; }
	};

}