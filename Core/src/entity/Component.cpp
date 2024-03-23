#include "pch.h"

#include "Component.h"

#include "Entity.h"

namespace core {

	Component::~Component()
	{
		if (!m_Entity)
		{
			m_Entity->RemoveComponentInternal(GetComponentType());
		}
	}

}