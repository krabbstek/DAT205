#include "pch.h"

#include "Entity.h"

namespace core {

	Entity::~Entity()
	{
		for (std::pair<ComponentType*, Component*> component : m_Components)
			delete component.second;
	}


	void Entity::AddComponent(Component* component)
	{
		if (!component)
			return;

		ComponentType* type = component->GetComponentType();
		auto entry = m_Components.find(type);
		if (entry != m_Components.end())
			delete entry->second;

		m_Components[type] = component;
	}


	Component* Entity::GetComponentInternal(ComponentType* componentType) const
	{
		auto entry = m_Components.find(componentType);
		if (entry == m_Components.end())
			return nullptr;

		return entry->second;
	}


	bool Entity::RemoveComponentInternal(ComponentType* componentType)
	{
		Component* component;
		auto entry = m_Components.find(componentType);
		if (entry != m_Components.end())
		{
			delete entry->second;
			m_Components.erase(componentType);
			return true;
		}
		return false;
	}

}