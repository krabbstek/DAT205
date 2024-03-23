#pragma once

#include "Common.h"

namespace core {

	class CORE_API Entity;

	enum CORE_API ComponentType
	{
		NONE = 0,
		TRANSFORM,
	};

	class CORE_API Component
	{
	public:
		virtual ~Component() {}

		virtual ComponentType GetComponentType() const = 0;

		inline Entity* GetEntity() const { return m_Entity; }

	private:
		Entity* m_Entity;

		friend class CORE_API Entity;
	};

}