#pragma once

#include "Common.h"

#include "Component.h"

#include <unordered_map>

namespace core {

	class CORE_API Entity
	{
	public:
		Entity() {}
		virtual ~Entity();

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
		/// Also deletes component from memory.
		/// Returns true if the component was successfully removed, false otherwise.
		/// </summary>
		bool RemoveComponent(Component* component);
		/// <summary>
		/// Removes component of specific type from entity.
		/// Also deletes component from memory.
		/// Returns true if component was removed and deleted.
		/// </summary>
		bool RemoveComponent(ComponentType componentType);

	protected:
		std::unordered_map<ComponentType, Component*> m_Components;

	private:
		/// <summary>
		/// Removes exact component if entity contains it.
		/// Returns true if a component was removed, nullptr otherwise.
		/// Does not delete component from memory.
		/// </summary>
		bool RemoveComponentInternal(Component* component);
		/// <summary>
		/// Removes component of specific component type from entity.
		/// Returns pointer to removed component if a component was removed, nullptr otherwise.
		/// Does not delete component from memory.
		/// </summary>
		Component* RemoveComponentInternal(ComponentType componentType);

		friend class CORE_API Component;
	};

}