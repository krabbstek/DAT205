#include "pch.h"

#include "Quaternion.h"

#include "vec3.h"
#include "vec4.h"
#include "mat4.h"

namespace core {

	Quaternion::Quaternion()
		: x(0.0f), y(0.0f), z(0.0f), w(1.0f)
	{
	}

	Quaternion::Quaternion(float x, float y, float z)
		: x(x), y(y), z(z), w(0.0f)
	{
	}

	Quaternion::Quaternion(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w)
	{
	}

	Quaternion::Quaternion(const vec3& xyz)
		: x(xyz.x), y(xyz.y), z(xyz.z), w(0.0f)
	{
	}

	Quaternion::Quaternion(const vec3& xyz, float w)
		: x(xyz.x), y(xyz.y), z(xyz.z), w(w)
	{
	}

	Quaternion::Quaternion(const vec4& xyzw)
		: x(xyzw.x), y(xyzw.y), z(xyzw.z), w(xyzw.w)
	{
	}


	Quaternion Quaternion::Rotation(const vec3& axis, float radians)
	{
		radians *= 0.5f;
		float c = cos(radians);
		float s = sin(radians);
		return Quaternion(axis.x * s, axis.y * s, axis.z * s, c);
	}


	Quaternion& Quaternion::Multiply(const Quaternion& quat)
	{
		float nx = w * quat.x + x * quat.w + y * quat.z - z * quat.y;
		float ny = w * quat.y - x * quat.z + y * quat.w + z * quat.x;
		float nz = w * quat.z + x * quat.y - y * quat.x + z * quat.w;
		float nw = w * quat.w - x * quat.x - y * quat.y - z * quat.z;
		x = nx;
		y = ny;
		z = nz;
		w = nw;
		return *this;
	}

	Quaternion Quaternion::Multiply(const Quaternion& left, const Quaternion& right)
	{
		float nx = left.w * right.x + left.x * right.w + left.y * right.z - left.z * right.y;
		float ny = left.w * right.y - left.x * right.z + left.y * right.w + left.z * right.x;
		float nz = left.w * right.z + left.x * right.y - left.y * right.x + left.z * right.w;
		float nw = left.w * right.w - left.x * right.x - left.y * right.y - left.z * right.z;
		return Quaternion(nx, ny, nz, nw);
	}


	Quaternion& Quaternion::Conjugate()
	{
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}

	Quaternion Quaternion::Conjugate(const Quaternion& quat)
	{
		return Quaternion(-quat.x, -quat.y, -quat.z, quat.w);
	}


	Quaternion& Quaternion::Invert()
	{
		float invMagnitudeSq = 1.0f / (x * x + y * y + z * z + w * w);
		Conjugate();
		x *= invMagnitudeSq;
		y *= invMagnitudeSq;
		z *= invMagnitudeSq;
		w *= invMagnitudeSq;
		return *this;
	}

	Quaternion Quaternion::Inverse(const Quaternion& quat)
	{
		float invMagnitudeSq = 1.0f / (quat.x * quat.x + quat.y * quat.y + quat.z * quat.z + quat.w * quat.w);
		return Quaternion(-quat.x * invMagnitudeSq, -quat.y * invMagnitudeSq, -quat.z * invMagnitudeSq, quat.w * invMagnitudeSq);
	}


	Quaternion operator*(const Quaternion& left, const Quaternion& right)
	{
		return Quaternion::Multiply(left, right);
	}


	Quaternion& Quaternion::Normalize()
	{
		float invMagnitude = 1.0f / Magnitude();
		x *= invMagnitude;
		y *= invMagnitude;
		z *= invMagnitude;
		w *= invMagnitude;
		return *this;
	}


	float Quaternion::Magnitude() const
	{
		return sqrt(x * x + y * y + z * z + w * w);
	}

}