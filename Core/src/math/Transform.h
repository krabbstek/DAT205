#pragma once

#include "Common.h"

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"
#include "Quaternion.h"

#include "entity/Component.h"

namespace core {

	struct CORE_API Transform : public Component
	{
	public:
		vec3 position;
		vec3 rotation;
		vec3 scale;

	public:
		Transform();

		mat4 GetTransformationMatrix() const;
		mat4 GetInverseTransformationMatrix() const;

		inline virtual ComponentType* GetComponentType() const { return GetComponentTypeStatic(); }
		static ComponentType* GetComponentTypeStatic();
	};

}