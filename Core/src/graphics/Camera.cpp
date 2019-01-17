#include "pch.h"

#include "Camera.h"

#include "entity/Entity.h"

#include "math/Transform.h"

namespace core {

	Camera::Camera()
		: m_ProjectionMatrix(1.0f), m_ViewMatrix(1.0f)
	{
	}


	const mat4& Camera::GenerateViewMatrix()
	{
		Transform* transform = GetEntity()->GetComponent<Transform>();
		if (transform)
			m_ViewMatrix = transform->GetInverseTransformationMatrix();
		return m_ViewMatrix;
	}


	vec3 Camera::GetForward() const
	{
		Transform* transform = GetEntity()->GetComponent<Transform>();
		if (transform)
		{
			vec3 p(0.0f, 0.0f, -1.0f);
			Quaternion qx = Quaternion::RotateX(transform->rotation.x);
			Quaternion qy = Quaternion::RotateY(transform->rotation.y);
			Quaternion qz = Quaternion::RotateZ(transform->rotation.z);
			Quaternion q = qx * qy * qz;
			return q.TransformPosition(p);
		}
		return vec3(0.0f, 0.0f, -1.0f);
	}


	ComponentType* Camera::GetComponentTypeStatic()
	{
		static ComponentType type = { "Camera" };
		return &type;
	}

}