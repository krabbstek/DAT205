#pragma once

#include "Common.h"

#include "entity/Component.h"

#include "math/mat4.h"

namespace core {

	class CORE_API Camera : public Component
	{
	public:
		Camera();

		const mat4& GenerateViewMatrix();

		vec3 GetForward() const;

		inline void SetPerspective(float fov, float aspectRatio, float near, float far) { m_ProjectionMatrix = mat4::Perspective(fov, aspectRatio, near, far); }
		inline void SetOrthographic(float left, float right, float bottom, float top, float near, float far) { m_ProjectionMatrix = mat4::Orthographic(left, right, bottom, top, near, far); }

		inline const mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline const mat4& GetViewMatrix() const { return m_ViewMatrix; }

		inline virtual ComponentType* GetComponentType() const { return GetComponentTypeStatic(); }
		static ComponentType* GetComponentTypeStatic();

	private:
		mat4 m_ProjectionMatrix;
		mat4 m_ViewMatrix;
	};

}