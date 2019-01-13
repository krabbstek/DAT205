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
		/// Does not do anything if component is nullptr.
		/// </summary>
		void AddComponent(Component* component);

		/// <summary>
		/// Returns pointer to entity's component of a specific type.
		/// Returns nullptr if entity does not contain a component of that type.
		/// </summary>
		template <typename T>
		T* GetComponent() const
		{
			return (T*)GetComponentInternal(T::GetComponentTypeStatic());
		}

		/// <summary>
		/// Removes component of specific type from entity.
		/// Also deletes component from memory.
		/// Returns true if component was removed and deleted.
		/// </summary>
		template <typename T>
		bool RemoveComponent()
		{
			return RemoveComponentInternal(T::GetComponentType());
		}

	protected:
		std::unordered_map<ComponentType*, Component*> m_Components;

	private:
		/// <summary>
		/// Looks for a component of a specific type in entity.
		/// Returns pointer to that component if it exists, returns nullptr otherwise.
		/// </summary>
		Component* GetComponentInternal(ComponentType* componentType) const;

		/// <summary>
		/// Removes component of specific component type from entity.
		/// Also deletes component from memory.
		/// Returns true if component was successfully removed, false otherwise.
		/// </summary>
		bool RemoveComponentInternal(ComponentType* componentType);
	};

}