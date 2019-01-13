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

	Quaternion Quaternion::RotateX(float radians)
	{
		radians *= 0.5f;
		return Quaternion(sin(radians), 0.0f, 0.0f, cos(radians));
	}

	Quaternion Quaternion::RotateY(float radians)
	{
		radians *= 0.5f;
		return Quaternion(0.0f, sin(radians), 0.0f, cos(radians));
	}

	Quaternion Quaternion::RotateZ(float radians)
	{
		radians *= 0.5f;
		return Quaternion(0.0f, 0.0f, sin(radians), cos(radians));
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


	vec3 Quaternion::TransformPosition(const vec3& vec) const
	{
		float w1 = -x * vec.x - y * vec.y - z * vec.z;
		float x1 = w * vec.x + y * vec.z - z * vec.y;
		float y1 = w * vec.y + z * vec.x - x * vec.z;
		float z1 = w * vec.z + x * vec.y - y * vec.x;

		//float w2 = w1 * w + x1 * x + y1 * y + z1 * z;
		float x2 = -w1 * x + x1 * w - y1 * z + z1 * y;
		float y2 = -w1 * y + x1 * z + y1 * w - z1 * x;
		float z2 = -w1 * z - x1 * y + y1 * x + z1 * w;

		return vec3(x2, y2, z2);
	}

	vec4 Quaternion::TransformPosition(const vec4& vec) const
	{
		float w1 = -x * vec.x - y * vec.y - z * vec.z;
		float x1 = w * vec.x + y * vec.z - z * vec.y;
		float y1 = w * vec.y + z * vec.x - x * vec.z;
		float z1 = w * vec.z + x * vec.y - y * vec.x;

		//float w2 = w1 * w + x1 * x + y1 * y + z1 * z;
		float x2 = -w1 * x + x1 * w - y1 * z + z1 * y;
		float y2 = -w1 * y + x1 * z + y1 * w - z1 * x;
		float z2 = -w1 * z - x1 * y + y1 * x + z1 * w;

		return vec4(x2, y2, z2, 1.0f);
	}


	mat4 Quaternion::Matrix() const
	{
		mat4 rotation;

		float w2 = w * w;
		float x2 = x * x;
		float y2 = y * y;
		float z2 = z * z;

		float xy = 2.0f * x * y;
		float xz = 2.0f * x * z;
		float yz = 2.0f * y * z;

		float xw = 2.0f * x * w;
		float yw = 2.0f * y * w;
		float zw = 2.0f * z * w;

		rotation.elements[0 + 0 * 4] = w2 + x2 - y2 - z2;
		rotation.elements[1 + 0 * 4] = xy + zw;
		rotation.elements[2 + 0 * 4] = xz - yw;

		rotation.elements[0 + 1 * 4] = xy - zw;
		rotation.elements[1 + 1 * 4] = w2 - x2 + y2 - z2;
		rotation.elements[2 + 1 * 4] = yz + xw;

		rotation.elements[0 + 2 * 4] = xz + yw;
		rotation.elements[1 + 2 * 4] = yz - xw;
		rotation.elements[2 + 2 * 4] = w2 - x2 - y2 + z2;

		rotation.elements[3 + 3 * 4] = 1.0f;

		return rotation;
	}


	Quaternion CORE_API operator*(const Quaternion& left, const Quaternion& right)
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