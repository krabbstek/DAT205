#include "pch.h"

#include "Entity.h"

namespace core {

	Entity::~Entity()
	{
		for (std::pair<ComponentType, Component*> component : m_Components)
		{
			component.second->m_Entity = nullptr;
			delete component.second;
		}
	}


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
		if (RemoveComponentInternal(component))
		{
			delete component;
			return true;
		}
		else
			return false;
	}

	bool Entity::RemoveComponent(ComponentType componentType)
	{
		Component* component = RemoveComponentInternal(componentType);
		if (component)
		{
			delete component;
			return true;
		}
		return false;
	}


	bool Entity::RemoveComponentInternal(Component* component)
	{
		if (!component)
			return false;

		ComponentType componentType = component->GetComponentType();
		if (!componentType)
			return false;

		auto entry = m_Components.find(componentType);
		if (entry != m_Components.end())
		{
			if (entry->second == component)
			{
				component->m_Entity = nullptr;
				m_Components.erase(componentType);
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}

	Component* Entity::RemoveComponentInternal(ComponentType componentType)
	{
		if (!componentType)
			return nullptr;

		Component* component;
		auto entry = m_Components.find(componentType);
		if (entry != m_Components.end())
		{
			component = entry->second;
			component->m_Entity = nullptr;
			m_Components.erase(componentType);
			return component;
		}
		else
			return nullptr;
	}

}