#include "pch.h"

#include "Transform.h"

namespace core {

	Transform::Transform()
		: position(0.0f), rotation(0.0f), scale(1.0f)
	{
	}


	void Transform::Move(float x, float y, float z)
	{
		position.x += x;
		position.y += y;
		position.z += z;
	}

	void Transform::Move(const vec3& vec)
	{
		position.x += vec.x;
		position.y += vec.y;
		position.z += vec.z;
	}

	void Transform::Move(const vec4& vec)
	{
		position.x += vec.x;
		position.y += vec.y;
		position.z += vec.z;
	}


	void Transform::Rotate(float x, float y, float z)
	{
		rotation.x += x;
		rotation.y += y;
		rotation.z += z;
	}

	void Transform::Rotate(const vec3& vec)
	{
		rotation.x += vec.x;
		rotation.y += vec.y;
		rotation.z += vec.z;
	}

	void Transform::Rotate(const vec4& vec)
	{
		rotation.x += vec.x;
		rotation.y += vec.y;
		rotation.z += vec.z;
	}

	void Transform::RotateX(float radians)
	{
		rotation.x += radians;
	}

	void Transform::RotateY(float radians)
	{
		rotation.y += radians;
	}

	void Transform::RotateZ(float radians)
	{
		rotation.z += radians;
	}


	void Transform::Scale(float x, float y, float z)
	{
		scale.x *= x;
		scale.y *= y;
		scale.z *= z;
	}

	void Transform::Scale(const vec3& vec)
	{
		scale.x *= vec.x;
		scale.y *= vec.y;
		scale.z *= vec.z;
	}

	void Transform::Scale(const vec4& vec)
	{
		scale.x *= vec.x;
		scale.y *= vec.y;
		scale.z *= vec.z;
	}


	mat4 Transform::GetTransformationMatrix() const
	{
		mat4 t = mat4::Translate(position);
		Quaternion qr = Quaternion::RotateY(rotation.y) * Quaternion::RotateX(rotation.x) * Quaternion::RotateZ(rotation.z);
		mat4 r = qr.Matrix();
		mat4 s = mat4::Scale(scale);

		return t * r * s;
	}

	mat4 Transform::GetInverseTransformationMatrix() const
	{
		mat4 t = mat4::Translate(-position);
		Quaternion qr = Quaternion::RotateZ(-rotation.z) * Quaternion::RotateX(-rotation.x) * Quaternion::RotateY(-rotation.y);
		mat4 r = qr.Matrix();
		mat4 s = mat4::Scale(1.0f / scale.x, 1.0f / scale.y, 1.0f / scale.z);

		return s * r * t;
	}


	ComponentType* Transform::GetComponentTypeStatic()
	{
		static ComponentType type = { "Transform" };
		return &type;
	}

}