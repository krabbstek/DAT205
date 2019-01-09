#include "pch.h"

#include "Entity.h"

namespace core {

	Component* Entity::AddComponent(Component* component)
	{
		if (!component)
			return nullptr;

		ComponentType type = component->GetComponentType();
		auto entry = m_Components.find(type);
		Component* old;
		if (entry == m_Components.end())
			old = nullptr;
		else
			old = entry->second;

		m_Components[type] = component;
		return component;
	}


	Component* Entity::GetComponent(ComponentType componentType)
	{
		if (!componentType)
			return nullptr;

		auto entry = m_Components.find(componentType);
		if (entry == m_Components.end())
			return nullptr;
		return entry->second;
	}


	bool Entity::RemoveComponent(Component* component)
	{
		if (!component)
			return false;

		ComponentType type = component->GetComponentType();
		auto entry = m_Components.find(type);
		if (entry == m_Components.end())
			return false;

		if (entry->second == component)
		{
			m_Components.erase(type);
			return true;
		}
		else
			return false;
	}

	Component* Entity::RemoveComponent(ComponentType componentType)
	{
		if (!componentType)
			return nullptr;

		auto entry = m_Components.find(componentType);
		if (entry == m_Components.end())
			return nullptr;

		Component* component = entry->second;
		m_Components.erase(componentType);
		return component;
	}

}