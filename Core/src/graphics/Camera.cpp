#include "pch.h"

#include "Camera.h"

#include "entity/Entity.h"

#include "math/Transform.h"

namespace core {

	Camera::Camera()
		: m_ProjectionMatrix(1.0f), m_ViewMatrix(1.0f)
	{
	}


	void Camera::GenerateViewMatrix()
	{
		Transform* transform = GetEntity()->GetComponent<Transform>();
		m_ViewMatrix = transform->GetInverseTransformationMatrix();
	}


	ComponentType* Camera::GetComponentTypeStatic()
	{
		static ComponentType type = { "Camera" };
		return &type;
	}

}