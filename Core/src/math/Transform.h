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

		void Move(float x, float y, float z);
		void Move(const vec3& vec);
		void Move(const vec4& vec);

		void Rotate(float x, float y, float z);
		void Rotate(const vec3& vec);
		void Rotate(const vec4& vec);
		void RotateX(float radians);
		void RotateY(float radians);
		void RotateZ(float radians);

		void Scale(float x, float y, float z);
		void Scale(const vec3& vec);
		void Scale(const vec4& vec);

		mat4 GetTransformationMatrix() const;
		mat4 GetInverseTransformationMatrix() const;

		inline virtual ComponentType* GetComponentType() const { return GetComponentTypeStatic(); }
		static ComponentType* GetComponentTypeStatic();
	};

}