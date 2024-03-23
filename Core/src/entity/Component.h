#pragma once

#include "Common.h"

namespace core {

	enum ComponentType
	{
		NONE = 0,
	};

	class CORE_API Component
	{
	public:
		virtual ComponentType GetComponentType() const = 0;
	};

}