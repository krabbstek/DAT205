#pragma once

#include "Common.h"

namespace core {

	enum EventType
	{
		None = 0,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved,
	};


	class CORE_API Event
	{
	public:
		virtual ~Event() {}

		virtual EventType GetEventType() const = 0;
	};

}