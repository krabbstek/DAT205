#pragma once

#include "Common.h"

namespace core {

	struct vec3;
	struct vec4;
	struct mat4;

	struct CORE_API Quaternion
	{
	public:
		float x, y, z, w;

	public:
		Quaternion();
		Quaternion(float x, float y, float z);
		Quaternion(float x, float y, float z, float w);
		Quaternion(const vec3& xyz);
		Quaternion(const vec3& xyz, float w);
		Quaternion(const vec4& xyzw);

		static Quaternion Rotation(const vec3& axis, float radians);

		Quaternion& Multiply(const Quaternion& quat);
		static Quaternion Multiply(const Quaternion& left, const Quaternion& right);

		Quaternion& Conjugate();
		static Quaternion Conjugate(const Quaternion& quat);

		Quaternion& Invert();
		static Quaternion Inverse(const Quaternion& quat);

		vec3 TransformPosition(const vec3& vec) const;
		vec4 TransformPosition(const vec4& vec) const;

		mat4 Matrix() const;

		friend Quaternion CORE_API operator*(const Quaternion& left, const Quaternion& right);
		inline Quaternion& operator*=(const Quaternion& quat) { return Multiply(quat); }

		Quaternion& Normalize();
		inline static Quaternion Normalize(Quaternion quat) { return quat.Normalize(); }

		float Magnitude() const;
	};

}