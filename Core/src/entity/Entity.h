#pragma once

#include "Common.h"

#include "Component.h"

#include <unordered_map>

namespace core {

	class CORE_API Entity
	{
	public:
		/// <summary>
		///	Adds component to entity. Replaces old component of same type if it exists.
		///	Returns old component of same type if it exists.
		/// </summary>
		Component* AddComponent(Component* component);

		/// <summary>
		/// Returns pointer to entity's component of type componentType.
		/// Returns nullptr if entity does not contain a component of type componentType.
		/// </summary>
		Component* GetComponent(ComponentType componentType);

		/// <summary>
		/// Removes component from entity if the entity contains the exact same component.
		/// Returns true if the component was successfully removed, false otherwise.
		/// </summary>
		bool RemoveComponent(Component* component);
		/// <summary>
		/// Removes component of specific type from entity.
		/// Returns pointer to removed component.
		/// </summary>
		Component* RemoveComponent(ComponentType componentType);

	protected:
		std::unordered_map<ComponentType, Component*> m_Components;
	};

}