#include "pch.h"

#include "Transform.h"

namespace core {

	Transform::Transform()
		: position(0.0f), rotation(0.0f), scale(1.0f)
	{
	}


	mat4 Transform::GetTransformationMatrix() const
	{
		mat4 t = mat4::Translate(position);
		Quaternion qr = Quaternion::RotateY(rotation.y) * Quaternion::RotateX(rotation.x) * Quaternion::RotateZ(rotation.z);
		mat4 r = qr.Matrix();
		mat4 s = mat4::Scale(scale);

		return t * r * s;
	}


	ComponentType* Transform::GetComponentTypeStatic()
	{
		static ComponentType type = { "Transform" };
		return &type;
	}

}