#pragma once

#include "Common.h"

namespace core {

	class CORE_API Renderable
	{
	public:
		virtual void Render() = 0;
	};

}