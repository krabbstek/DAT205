#pragma once

#include "Common.h"

namespace core {

	class CORE_API Entity;

	/// <summary>
	/// Used internally to map components of different types.
	/// </summary>
	struct CORE_API ComponentType
	{
		const char* name;
	};

	/// <summary>
	/// Base class of components for entities.
	/// </summary>
	class CORE_API Component
	{
	public:
		virtual ~Component() {}

		/// <summary>
		/// To be implemented by each sub-class of Component.
		/// Used to internally map components of different types.
		/// </summary>
		virtual ComponentType* GetComponentType() const = 0;

		inline Entity* GetEntity() const { return m_Entity; }

	private:
		Entity* m_Entity;

		friend class CORE_API Entity;
	};

}