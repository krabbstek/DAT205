#pragma once

#include "Common.h"

#include "vec3.h"
#include "vec4.h"

namespace core {

	struct CORE_API mat4
	{
	public:
		union
		{
			float elements[4 * 4];
			vec4 v[4];
			struct
			{
				vec4 c0;
				vec4 c1;
				vec4 c2;
				vec4 c3;
			};
		};

	public:
		mat4();
		mat4(float diagonal);
		mat4(float dx, float dy, float dz);
		mat4(float dx, float dy, float dz, float dw);
		mat4(const vec3& diagonal);
		mat4(const vec4& diagonal);
		mat4(const vec3& col0, const vec3& col1, const vec3& col2);
		mat4(const vec3& col0, const vec3& col1, const vec3& col2, const vec3& col3);
		mat4(const vec4& col0, const vec4& col1, const vec4& col2, const vec4& col3);
		mat4(const float* elements);

		inline static mat4 Identity() { return mat4(1.0f); }

		inline static mat4 Scale(float s) { return mat4(s); }
		inline static mat4 Scale(float sx, float sy, float sz) { return mat4(sx, sy, sz); }
		inline static mat4 Scale(float sx, float sy, float sz, float sw) { return mat4(sx, sy, sz, sw); }
		inline static mat4 Scale(const vec3& scale) { return mat4(scale); }
		inline static mat4 Scale(const vec4& scale) { return mat4(scale); }

		static mat4 Translate(float t);
		static mat4 Translate(float tx, float ty, float tz);
		static mat4 Translate(const vec3& t);
		static mat4 Translate(const vec4& t);

		static mat4 RotateX(float radians);
		static mat4 RotateY(float radians);
		static mat4 RotateZ(float radians);
		static mat4 Rotate(float radians, float ax, float ay, float az);
		static mat4 Rotate(float radians, const vec3& axis);
		static mat4 Rotate(float radians, const vec4& axis);
		
		static mat4 Orthographic(float left, float right, float bottom, float top, float near, float far);
		static mat4 Perspective(float fov, float aspectRatio, float near, float far);
		static mat4 LookAt(const vec3& cameraPosition, const vec3& objectPosition, const vec3& up = { 0.0f, 1.0f, 0.0f });

		mat4& Add(const mat4& matrix);
		mat4& Subtract(const mat4& matrix);

		/* Standard matrix multiplication. */
		mat4& Multiply(const mat4& matrix);

		inline mat4& operator*=(const mat4& matrix) { return Multiply(matrix); }
		friend mat4 operator*(mat4 left, const mat4& right);

		float Determinant() const;

		mat4& Invert();
		mat4& Transpose();

		static mat4 Inverse(const mat4& matrix);
		static mat4 Transpose(const mat4& matrix);
	};

	int i = sizeof(mat4);

}