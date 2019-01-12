#pragma once

#include "Common.h"

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"
#include "Quaternion.h"

#include "entity/Component.h"

namespace core {

	/// <summary>
	/// Left-most rotation axis is the most "general" one.
	/// </summary>
	enum CORE_API RotationOrder
	{
		XYZ = 1,
		XZY,
		YXZ,
		YZX,
		ZXY,
		ZYX,
	};

	struct CORE_API Transform
	{
	public:
		vec3 position;
		vec3 rotation;
		vec3 scale;

	public:
		Transform();

		mat4 GetTransformationMatrix() const;

		inline void SetRotationOrder(RotationOrder rotationOrder) { m_RotationOrder = rotationOrder; }

		virtual ComponentType GetComponentType() const { return TRANSFORM; }

	private:
		RotationOrder m_RotationOrder;
	};

}