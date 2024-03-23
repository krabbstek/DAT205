#include "pch.h"

#include "mat4.h"

#define INDEX(ROW, COL) (ROW + (COL << 2))

#undef near
#undef far

namespace core {

	mat4::mat4()
	{
		memset(elements, 0, sizeof(elements));
	}

	mat4::mat4(float diagonal)
	{
		memset(elements, 0, sizeof(elements));
		elements[INDEX(0, 0)] = diagonal;
		elements[INDEX(1, 1)] = diagonal;
		elements[INDEX(2, 2)] = diagonal;
		elements[INDEX(3, 3)] = diagonal;
	}

	mat4::mat4(float dx, float dy, float dz)
	{
		memset(elements, 0, sizeof(elements));
		elements[INDEX(0, 0)] = dx;
		elements[INDEX(1, 1)] = dy;
		elements[INDEX(2, 2)] = dz;
		elements[INDEX(3, 3)] = 1.0f;
	}

	mat4::mat4(float dx, float dy, float dz, float dw)
	{
		memset(elements, 0, sizeof(elements));
		elements[INDEX(0, 0)] = dx;
		elements[INDEX(1, 1)] = dy;
		elements[INDEX(2, 2)] = dz;
		elements[INDEX(3, 3)] = dw;
	}

	mat4::mat4(const vec3& diagonal)
	{
		memset(elements, 0, sizeof(elements));
		elements[INDEX(0, 0)] = diagonal.x;
		elements[INDEX(1, 1)] = diagonal.y;
		elements[INDEX(2, 2)] = diagonal.z;
		elements[INDEX(3, 3)] = 1.0f;
	}

	mat4::mat4(const vec4& diagonal)
	{
		memset(elements, 0, sizeof(elements));
		elements[INDEX(0, 0)] = diagonal.x;
		elements[INDEX(1, 1)] = diagonal.y;
		elements[INDEX(2, 2)] = diagonal.z;
		elements[INDEX(3, 3)] = diagonal.w;
	}

	mat4::mat4(const vec3& col0, const vec3& col1, const vec3& col2)
	{
		elements[INDEX(0, 0)] = col0.x;
		elements[INDEX(1, 0)] = col0.y;
		elements[INDEX(2, 0)] = col0.z;
		elements[INDEX(3, 0)] = 0.0f;

		elements[INDEX(0, 1)] = col1.x;
		elements[INDEX(1, 1)] = col1.y;
		elements[INDEX(2, 1)] = col1.z;
		elements[INDEX(3, 1)] = 0.0f;

		elements[INDEX(0, 2)] = col2.x;
		elements[INDEX(1, 2)] = col2.y;
		elements[INDEX(2, 2)] = col2.z;
		elements[INDEX(3, 2)] = 0.0f;

		elements[INDEX(0, 2)] = 0.0f;
		elements[INDEX(1, 2)] = 0.0f;
		elements[INDEX(2, 2)] = 0.0f;
		elements[INDEX(3, 2)] = 0.0f;
	}

	mat4::mat4(const vec3& col0, const vec3& col1, const vec3& col2, const vec3& col3)
	{
		elements[INDEX(0, 0)] = col0.x;
		elements[INDEX(1, 0)] = col0.y;
		elements[INDEX(2, 0)] = col0.z;
		elements[INDEX(3, 0)] = 0.0f;

		elements[INDEX(0, 1)] = col1.x;
		elements[INDEX(1, 1)] = col1.y;
		elements[INDEX(2, 1)] = col1.z;
		elements[INDEX(3, 1)] = 0.0f;

		elements[INDEX(0, 2)] = col2.x;
		elements[INDEX(1, 2)] = col2.y;
		elements[INDEX(2, 2)] = col2.z;
		elements[INDEX(3, 2)] = 0.0f;

		elements[INDEX(0, 2)] = col3.x;
		elements[INDEX(1, 2)] = col3.y;
		elements[INDEX(2, 2)] = col3.z;
		elements[INDEX(3, 2)] = 0.0f;
	}

	mat4::mat4(const vec4& col0, const vec4& col1, const vec4& col2, const vec4& col3)
	{
		elements[INDEX(0, 0)] = col0.x;
		elements[INDEX(1, 0)] = col0.y;
		elements[INDEX(2, 0)] = col0.z;
		elements[INDEX(3, 0)] = col0.w;

		elements[INDEX(0, 1)] = col1.x;
		elements[INDEX(1, 1)] = col1.y;
		elements[INDEX(2, 1)] = col1.z;
		elements[INDEX(3, 1)] = col1.w;

		elements[INDEX(0, 2)] = col2.x;
		elements[INDEX(1, 2)] = col2.y;
		elements[INDEX(2, 2)] = col2.z;
		elements[INDEX(3, 2)] = col2.w;

		elements[INDEX(0, 2)] = col3.x;
		elements[INDEX(1, 2)] = col3.y;
		elements[INDEX(2, 2)] = col3.z;
		elements[INDEX(3, 2)] = col3.w;
	}

	mat4::mat4(const float* elements)
	{
		memcpy(this->elements, elements, sizeof(this->elements));
	}


	mat4 mat4::Translate(float t)
	{
		float elements[16];
		memset(elements, 0, sizeof(elements));

		elements[INDEX(0, 0)] = 1.0f;
		elements[INDEX(1, 1)] = 1.0f;
		elements[INDEX(2, 2)] = 1.0f;
		elements[INDEX(3, 3)] = 1.0f;

		elements[INDEX(0, 3)] = t;
		elements[INDEX(1, 3)] = t;
		elements[INDEX(2, 3)] = t;

		return mat4(elements);
	}

	mat4 mat4::Translate(float tx, float ty, float tz)
	{
		float elements[16];
		memset(elements, 0, sizeof(elements));

		elements[INDEX(0, 0)] = 1.0f;
		elements[INDEX(1, 1)] = 1.0f;
		elements[INDEX(2, 2)] = 1.0f;
		elements[INDEX(3, 3)] = 1.0f;

		elements[INDEX(0, 3)] = tx;
		elements[INDEX(1, 3)] = ty;
		elements[INDEX(2, 3)] = tz;

		return mat4(elements);
	}

	mat4 mat4::Translate(const vec3& t)
	{
		float elements[16];
		memset(elements, 0, sizeof(elements));

		elements[INDEX(0, 0)] = 1.0f;
		elements[INDEX(1, 1)] = 1.0f;
		elements[INDEX(2, 2)] = 1.0f;
		elements[INDEX(3, 3)] = 1.0f;

		elements[INDEX(0, 3)] = t.x;
		elements[INDEX(1, 3)] = t.y;
		elements[INDEX(2, 3)] = t.z;

		return mat4(elements);
	}

	mat4 mat4::Translate(const vec4& t)
	{
		float elements[16];
		memset(elements, 0, sizeof(elements));

		elements[INDEX(0, 0)] = 1.0f;
		elements[INDEX(1, 1)] = 1.0f;
		elements[INDEX(2, 2)] = 1.0f;
		elements[INDEX(3, 3)] = 1.0f;

		elements[INDEX(0, 3)] = t.x;
		elements[INDEX(1, 3)] = t.y;
		elements[INDEX(2, 3)] = t.z;

		return mat4(elements);
	}


	mat4 mat4::RotateX(float radians)
	{
		float elements[16];
		memset(elements, 0, sizeof(elements));
		float s = sin(radians);
		float c = cos(radians);

		elements[INDEX(0, 0)] = 1.0f;

		elements[INDEX(1, 1)] = c;
		elements[INDEX(1, 2)] = -s;
		elements[INDEX(2, 1)] = s;
		elements[INDEX(2, 2)] = c;

		elements[INDEX(3, 3)] = 1.0f;

		return mat4(elements);
	}

	mat4 mat4::RotateY(float radians)
	{
		float elements[16];
		memset(elements, 0, sizeof(elements));
		float s = sin(radians);
		float c = cos(radians);

		elements[INDEX(0, 0)] = c;
		elements[INDEX(0, 2)] = s;
		elements[INDEX(2, 0)] = -s;
		elements[INDEX(2, 2)] = c;

		elements[INDEX(1, 1)] = 1.0f;

		elements[INDEX(3, 3)] = 1.0f;

		return mat4(elements);
	}

	mat4 mat4::RotateZ(float radians)
	{
		mat4 result;

		float elements[16];
		memset(elements, 0, sizeof(elements));
		float s = sin(radians);
		float c = cos(radians);

		elements[INDEX(0, 0)] = c;
		elements[INDEX(0, 1)] = -s;
		elements[INDEX(1, 0)] = s;
		elements[INDEX(1, 1)] = c;

		elements[INDEX(2, 2)] = 1.0f;

		elements[INDEX(3, 3)] = 1.0f;

		return mat4(elements);
	}

	mat4 mat4::Rotate(float radians, float ax, float ay, float az)
	{
		float elements[16];

		float c = cos(radians);
		float nc = 1.0f - c;
		float s = sin(radians);

		float ax2 = ax * ax;
		float axy = ax * ay;
		float axz = ax * az;
		float ay2 = ay * ay;
		float ayz = ay * az;
		float az2 = az * az;

		float axy_nc = axy * nc;
		float axz_nc = axz * nc;
		float ayz_nc = ayz * nc;

		float ax_s = ax * s;
		float ay_s = ay * s;
		float az_s = az * s;

		elements[INDEX(0, 0)] = c + ax2 * nc;
		elements[INDEX(0, 1)] = axy_nc - az_s;
		elements[INDEX(0, 2)] = axz_nc + ay_s;

		elements[INDEX(1, 0)] = axy_nc + az_s;
		elements[INDEX(1, 1)] = c + ay2 * nc;
		elements[INDEX(1, 2)] = ayz_nc - ax_s;

		elements[INDEX(2, 0)] = axz_nc - ay_s;
		elements[INDEX(2, 1)] = ayz_nc + ax_s;
		elements[INDEX(2, 2)] = c + az2 * nc;

		elements[INDEX(0, 3)] = 0.0f;
		elements[INDEX(1, 3)] = 0.0f;
		elements[INDEX(2, 3)] = 0.0f;

		elements[INDEX(3, 0)] = 0.0f;
		elements[INDEX(3, 1)] = 0.0f;
		elements[INDEX(3, 2)] = 0.0f;

		elements[INDEX(3, 3)] = 1.0f;

		return mat4(elements);
	}

	mat4 mat4::Rotate(float radians, const vec3& axis)
	{
		float elements[16];

		float c = cos(radians);
		float nc = 1.0f - c;
		float s = sin(radians);

		float ax2 = axis.x * axis.x;
		float axy = axis.x * axis.y;
		float axz = axis.x * axis.z;
		float ay2 = axis.y * axis.y;
		float ayz = axis.y * axis.z;
		float az2 = axis.z * axis.z;

		float axy_nc = axy * nc;
		float axz_nc = axz * nc;
		float ayz_nc = ayz * nc;

		float ax_s = axis.x * s;
		float ay_s = axis.y * s;
		float az_s = axis.z * s;

		elements[INDEX(0, 0)] = c + ax2 * nc;
		elements[INDEX(0, 1)] = axy_nc - az_s;
		elements[INDEX(0, 2)] = axz_nc + ay_s;

		elements[INDEX(1, 0)] = axy_nc + az_s;
		elements[INDEX(1, 1)] = c + ay2 * nc;
		elements[INDEX(1, 2)] = ayz_nc - ax_s;

		elements[INDEX(2, 0)] = axz_nc - ay_s;
		elements[INDEX(2, 1)] = ayz_nc + ax_s;
		elements[INDEX(2, 2)] = c + az2 * nc;

		elements[INDEX(0, 3)] = 0.0f;
		elements[INDEX(1, 3)] = 0.0f;
		elements[INDEX(2, 3)] = 0.0f;

		elements[INDEX(3, 0)] = 0.0f;
		elements[INDEX(3, 1)] = 0.0f;
		elements[INDEX(3, 2)] = 0.0f;

		elements[INDEX(3, 3)] = 1.0f;

		return mat4(elements);
	}

	mat4 mat4::Rotate(float radians, const vec4& axis)
	{
		float elements[16];

		float c = cos(radians);
		float nc = 1.0f - c;
		float s = sin(radians);

		float ax2 = axis.x * axis.x;
		float axy = axis.x * axis.y;
		float axz = axis.x * axis.z;
		float ay2 = axis.y * axis.y;
		float ayz = axis.y * axis.z;
		float az2 = axis.z * axis.z;

		float axy_nc = axy * nc;
		float axz_nc = axz * nc;
		float ayz_nc = ayz * nc;

		float ax_s = axis.x * s;
		float ay_s = axis.y * s;
		float az_s = axis.z * s;

		elements[INDEX(0, 0)] = c + ax2 * nc;
		elements[INDEX(0, 1)] = axy_nc - az_s;
		elements[INDEX(0, 2)] = axz_nc + ay_s;

		elements[INDEX(1, 0)] = axy_nc + az_s;
		elements[INDEX(1, 1)] = c + ay2 * nc;
		elements[INDEX(1, 2)] = ayz_nc - ax_s;

		elements[INDEX(2, 0)] = axz_nc - ay_s;
		elements[INDEX(2, 1)] = ayz_nc + ax_s;
		elements[INDEX(2, 2)] = c + az2 * nc;

		elements[INDEX(0, 3)] = 0.0f;
		elements[INDEX(1, 3)] = 0.0f;
		elements[INDEX(2, 3)] = 0.0f;

		elements[INDEX(3, 0)] = 0.0f;
		elements[INDEX(3, 1)] = 0.0f;
		elements[INDEX(3, 2)] = 0.0f;

		elements[INDEX(3, 3)] = 1.0f;

		return mat4(elements);
	}

	mat4 mat4::Orthographic(float left, float right, float bottom, float top, float near, float far)
	{
		float elements[16];
		memset(elements, 0, sizeof(elements));

		float _rl = 1.0f / (right - left);
		float _tb = 1.0f / (top - bottom);
		float _nf = 1.0f / (near - far);

		elements[INDEX(0, 0)] = 2.0f * _rl;
		elements[INDEX(1, 1)] = 2.0f * _tb;
		elements[INDEX(2, 2)] = 2.0f * _nf;

		elements[INDEX(0, 3)] = -(right + left) * _rl;
		elements[INDEX(1, 3)] = -(top + bottom) * _tb;
		elements[INDEX(2, 3)] = (far + near) * _nf;

		elements[INDEX(3, 3)] = 1.0f;

		return mat4(elements);
	}

	mat4 mat4::Perspective(float fov, float aspectRatio, float near, float far)
	{
		float elements[16];
		memset(elements, 0, sizeof(elements));

		float _t = 1.0f / tan(0.5f * fov);
		float _nf = 1.0f / (near - far);

		elements[INDEX(0, 0)] = _t / aspectRatio;
		elements[INDEX(1, 1)] = _t;

		elements[INDEX(2, 2)] = (far + near) * _nf;
		elements[INDEX(2, 3)] = 2 * far * near * _nf;

		elements[INDEX(3, 2)] = -1.0f;

		return mat4(elements);
	}

	mat4 mat4::LookAt(const vec3& cameraPosition, const vec3& objectPosition, const vec3& up)
	{
		vec3 zAxis = vec3::Normalize(objectPosition - cameraPosition);
		vec3 xAxis = vec3::Normalize(vec3::Cross(up, zAxis));
		vec3 yAxis = vec3::Cross(zAxis, xAxis);
		vec3 t(vec3::Dot(xAxis, cameraPosition), vec3::Dot(yAxis, cameraPosition), vec3::Dot(zAxis, cameraPosition));

		float elements[16];

		elements[INDEX(0, 0)] = xAxis.x;
		elements[INDEX(1, 0)] = yAxis.x;
		elements[INDEX(2, 0)] = zAxis.x;

		elements[INDEX(0, 1)] = xAxis.y;
		elements[INDEX(1, 1)] = yAxis.y;
		elements[INDEX(2, 1)] = zAxis.y;

		elements[INDEX(0, 2)] = xAxis.z;
		elements[INDEX(1, 2)] = yAxis.z;
		elements[INDEX(2, 2)] = zAxis.z;

		elements[INDEX(0, 3)] = t.x;
		elements[INDEX(1, 3)] = t.y;
		elements[INDEX(2, 3)] = t.z;

		elements[INDEX(3, 0)] = 0.0f;
		elements[INDEX(3, 1)] = 0.0f;
		elements[INDEX(3, 2)] = 0.0f;

		elements[INDEX(3, 3)] = 1.0f;

		return mat4(elements);
	}


	mat4& mat4::Add(const mat4& matrix)
	{
		elements[0] += matrix.elements[0];
		elements[1] += matrix.elements[1];
		elements[2] += matrix.elements[2];
		elements[3] += matrix.elements[3];

		elements[4] += matrix.elements[4];
		elements[5] += matrix.elements[5];
		elements[6] += matrix.elements[6];
		elements[7] += matrix.elements[7];

		elements[8] += matrix.elements[8];
		elements[9] += matrix.elements[9];
		elements[10] += matrix.elements[10];
		elements[11] += matrix.elements[11];

		elements[12] += matrix.elements[12];
		elements[13] += matrix.elements[13];
		elements[14] += matrix.elements[14];
		elements[15] += matrix.elements[15];

		return *this;
	}

	mat4& mat4::Subtract(const mat4& matrix)
	{
		elements[0] -= matrix.elements[0];
		elements[1] -= matrix.elements[1];
		elements[2] -= matrix.elements[2];
		elements[3] -= matrix.elements[3];

		elements[4] -= matrix.elements[4];
		elements[5] -= matrix.elements[5];
		elements[6] -= matrix.elements[6];
		elements[7] -= matrix.elements[7];

		elements[8] -= matrix.elements[8];
		elements[9] -= matrix.elements[9];
		elements[10] -= matrix.elements[10];
		elements[11] -= matrix.elements[11];

		elements[12] -= matrix.elements[12];
		elements[13] -= matrix.elements[13];
		elements[14] -= matrix.elements[14];
		elements[15] -= matrix.elements[15];

		return *this;
	}


	mat4& mat4::Multiply(const mat4& matrix)
	{
		float newElements[16];
		memset(newElements, 0, sizeof(newElements));

		int i, j;
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				newElements[INDEX(i, j)] += elements[INDEX(i, 0)] * matrix.elements[INDEX(0, j)];
				newElements[INDEX(i, j)] += elements[INDEX(i, 1)] * matrix.elements[INDEX(1, j)];
				newElements[INDEX(i, j)] += elements[INDEX(i, 2)] * matrix.elements[INDEX(2, j)];
				newElements[INDEX(i, j)] += elements[INDEX(i, 3)] * matrix.elements[INDEX(3, j)];
			}
		}

		memcpy(elements, newElements, sizeof(elements));
		return *this;
	}


	mat4 CORE_API operator*(mat4 left, const mat4& right)
	{
		left.Multiply(right);
		return mat4(left.elements);
	}


#define a11 elements[INDEX(0, 0)]
#define a12 elements[INDEX(0, 1)]
#define a13 elements[INDEX(0, 2)]
#define a14 elements[INDEX(0, 3)]
#define a21 elements[INDEX(1, 0)]
#define a22 elements[INDEX(1, 1)]
#define a23 elements[INDEX(1, 2)]
#define a24 elements[INDEX(1, 3)]
#define a31 elements[INDEX(2, 0)]
#define a32 elements[INDEX(2, 1)]
#define a33 elements[INDEX(2, 2)]
#define a34 elements[INDEX(2, 3)]
#define a41 elements[INDEX(3, 0)]
#define a42 elements[INDEX(3, 1)]
#define a43 elements[INDEX(3, 2)]
#define a44 elements[INDEX(3, 3)]

#define b11 newElements[INDEX(0, 0)]
#define b12 newElements[INDEX(0, 1)]
#define b13 newElements[INDEX(0, 2)]
#define b14 newElements[INDEX(0, 3)]
#define b21 newElements[INDEX(1, 0)]
#define b22 newElements[INDEX(1, 1)]
#define b23 newElements[INDEX(1, 2)]
#define b24 newElements[INDEX(1, 3)]
#define b31 newElements[INDEX(2, 0)]
#define b32 newElements[INDEX(2, 1)]
#define b33 newElements[INDEX(2, 2)]
#define b34 newElements[INDEX(2, 3)]
#define b41 newElements[INDEX(3, 0)]
#define b42 newElements[INDEX(3, 1)]
#define b43 newElements[INDEX(3, 2)]
#define b44 newElements[INDEX(3, 3)]

	float mat4::Determinant() const
	{
		float det = a11 * a22 * a33 * a44 - a11 * a22 * a34 * a43
			- a11 * a23 * a32 * a44 + a11 * a23 * a34 * a42
			+ a11 * a24 * a32 * a43 - a11 * a24 * a33 * a42
			- a12 * a21 * a33 * a44 + a12 * a21 * a34 * a43
			+ a12 * a23 * a31 * a44 - a12 * a23 * a34 * a41
			- a12 * a24 * a31 * a43 + a12 * a24 * a33 * a41
			+ a13 * a21 * a32 * a44 - a13 * a21 * a34 * a42
			- a13 * a22 * a31 * a44 + a13 * a22 * a34 * a41
			+ a13 * a24 * a31 * a42 - a13 * a24 * a32 * a41
			- a14 * a21 * a32 * a43 + a14 * a21 * a33 * a42
			+ a14 * a22 * a31 * a43 - a14 * a22 * a33 * a41
			- a14 * a23 * a31 * a42 + a14 * a23 * a32 * a41;

		return det;
	}


	mat4& mat4::Invert()
	{
		float newElements[16];

		b11 = a22 * a33 * a44 - a22 * a34 * a43 - a23 * a32 * a44
			+ a23 * a34 * a42 + a24 * a32 * a43 - a24 * a33 * a42;
		b12 = -(a12 * a33 * a44 - a12 * a34 * a43 - a13 * a32 * a44
			+ a13 * a34 * a42 + a14 * a32 * a43 - a14 * a33 * a42);
		b13 = a12 * a23 * a44 - a12 * a24 * a43 - a13 * a22 * a44
			+ a13 * a24 * a42 + a14 * a22 * a43 - a14 * a23 * a42;
		b14 = -(a12 * a23 * a34 - a12 * a24 * a33 - a13 * a22 * a34
			+ a13 * a24 * a32 + a14 * a22 * a33 - a14 * a23 * a32);

		b21 = -(a21 * a33 * a44 - a21 * a34 * a43 - a23 * a31 * a44
			+ a23 * a34 * a41 + a24 * a31 * a43 - a24 * a33 * a41);
		b22 = a11 * a33 * a44 - a11 * a34 * a43 - a13 * a31 * a44 
			+ a13 * a34 * a41 + a14 * a31 * a43 - a14 * a33 * a41;
		b23 = -(a11 * a23 * a44 - a11 * a24 * a43 - a13 * a21 * a44
			+ a13 * a24 * a41 + a14 * a21 * a43 - a14 * a23 * a41);
		b24 = a11 * a23 * a34 - a11 * a24 * a33 - a13 * a21 * a34
			+ a13 * a24 * a31 + a14 * a21 * a33 - a14 * a23 * a31;

		b31 = a21 * a32 * a44 - a21 * a34 * a42 - a22 * a31 * a44
			+ a22 * a34 * a41 + a24 * a31 * a42 - a24 * a32 * a41;
		b32 = -(a11 * a32 * a44 - a11 * a34 * a42 - a12 * a31 * a44
			+ a12 * a34 * a41 + a14 * a31 * a42 - a14 * a32 * a41);
		b33 = a11 * a22 * a44 - a11 * a24 * a42 - a12 * a21 * a44
			+ a12 * a24 * a41 + a14 * a21 * a42 - a14 * a22 * a41;
		b34 = -(a11 * a22 * a34 - a11 * a24 * a32 - a12 * a21 * a34
			+ a12 * a24 * a31 + a14 * a21 * a32 - a14 * a22 * a31);

		b41 = -(a21 * a32 * a43 - a21 * a33 * a42 - a22 * a31 * a43
			+ a22 * a33 * a41 + a23 * a31 * a42 - a23 * a32 * a41);
		b42 = a11 * a32 * a43 - a11 * a33 * a42 - a12 * a31 * a43
			+ a12 * a33 * a41 + a13 * a31 * a42 - a13 * a32 * a41;
		b43 = -(a11 * a22 * a43 - a11 * a23 * a42 - a12 * a21 * a43
			+ a12 * a23 * a41 + a13 * a21 * a42 - a13 * a22 * a41);
		b44 = a11 * a22 * a33 - a11 * a23 * a32 - a12 * a21 * a33
			+ a12 * a23 * a31 + a13 * a21 * a32 - a13 * a22 * a31;

		float detInv = 1.0f / Determinant();
		for (int i = 0; i < 16; i += 4)
		{
			newElements[i + 0] *= detInv;
			newElements[i + 1] *= detInv;
			newElements[i + 2] *= detInv;
			newElements[i + 3] *= detInv;
		}

		memcpy(elements, newElements, sizeof(elements));
		return *this;
	}

	mat4& mat4::Transpose()
	{
		float tmp;

		tmp = a12;
		a12 = a21;
		a21 = tmp;

		tmp = a13;
		a13 = a31;
		a31 = tmp;

		tmp = a14;
		a14 = a41;
		a41 = tmp;

		tmp = a23;
		a23 = a32;
		a32 = tmp;

		tmp = a24;
		a24 = a42;
		a42 = tmp;

		tmp = a34;
		a34 = a43;
		a43 = tmp;

		return *this;
	}


#define a11 matrix.elements[INDEX(0, 0)]
#define a12 matrix.elements[INDEX(0, 1)]
#define a13 matrix.elements[INDEX(0, 2)]
#define a14 matrix.elements[INDEX(0, 3)]
#define a21 matrix.elements[INDEX(1, 0)]
#define a22 matrix.elements[INDEX(1, 1)]
#define a23 matrix.elements[INDEX(1, 2)]
#define a24 matrix.elements[INDEX(1, 3)]
#define a31 matrix.elements[INDEX(2, 0)]
#define a32 matrix.elements[INDEX(2, 1)]
#define a33 matrix.elements[INDEX(2, 2)]
#define a34 matrix.elements[INDEX(2, 3)]
#define a41 matrix.elements[INDEX(3, 0)]
#define a42 matrix.elements[INDEX(3, 1)]
#define a43 matrix.elements[INDEX(3, 2)]
#define a44 matrix.elements[INDEX(3, 3)]


	mat4 mat4::Inverse(const mat4& matrix)
	{
		float newElements[16];

		b11 = a22 * a33 * a44 - a22 * a34 * a43 - a23 * a32 * a44
			+ a23 * a34 * a42 + a24 * a32 * a43 - a24 * a33 * a42;
		b12 = -(a12 * a33 * a44 - a12 * a34 * a43 - a13 * a32 * a44
			+ a13 * a34 * a42 + a14 * a32 * a43 - a14 * a33 * a42);
		b13 = a12 * a23 * a44 - a12 * a24 * a43 - a13 * a22 * a44
			+ a13 * a24 * a42 + a14 * a22 * a43 - a14 * a23 * a42;
		b14 = -(a12 * a23 * a34 - a12 * a24 * a33 - a13 * a22 * a34
			+ a13 * a24 * a32 + a14 * a22 * a33 - a14 * a23 * a32);

		b21 = -(a21 * a33 * a44 - a21 * a34 * a43 - a23 * a31 * a44
			+ a23 * a34 * a41 + a24 * a31 * a43 - a24 * a33 * a41);
		b22 = a11 * a33 * a44 - a11 * a34 * a43 - a13 * a31 * a44
			+ a13 * a34 * a41 + a14 * a31 * a43 - a14 * a33 * a41;
		b23 = -(a11 * a23 * a44 - a11 * a24 * a43 - a13 * a21 * a44
			+ a13 * a24 * a41 + a14 * a21 * a43 - a14 * a23 * a41);
		b24 = a11 * a23 * a34 - a11 * a24 * a33 - a13 * a21 * a34
			+ a13 * a24 * a31 + a14 * a21 * a33 - a14 * a23 * a31;

		b31 = a21 * a32 * a44 - a21 * a34 * a42 - a22 * a31 * a44
			+ a22 * a34 * a41 + a24 * a31 * a42 - a24 * a32 * a41;
		b32 = -(a11 * a32 * a44 - a11 * a34 * a42 - a12 * a31 * a44
			+ a12 * a34 * a41 + a14 * a31 * a42 - a14 * a32 * a41);
		b33 = a11 * a22 * a44 - a11 * a24 * a42 - a12 * a21 * a44
			+ a12 * a24 * a41 + a14 * a21 * a42 - a14 * a22 * a41;
		b34 = -(a11 * a22 * a34 - a11 * a24 * a32 - a12 * a21 * a34
			+ a12 * a24 * a31 + a14 * a21 * a32 - a14 * a22 * a31);

		b41 = -(a21 * a32 * a43 - a21 * a33 * a42 - a22 * a31 * a43
			+ a22 * a33 * a41 + a23 * a31 * a42 - a23 * a32 * a41);
		b42 = a11 * a32 * a43 - a11 * a33 * a42 - a12 * a31 * a43
			+ a12 * a33 * a41 + a13 * a31 * a42 - a13 * a32 * a41;
		b43 = -(a11 * a22 * a43 - a11 * a23 * a42 - a12 * a21 * a43
			+ a12 * a23 * a41 + a13 * a21 * a42 - a13 * a22 * a41);
		b44 = a11 * a22 * a33 - a11 * a23 * a32 - a12 * a21 * a33
			+ a12 * a23 * a31 + a13 * a21 * a32 - a13 * a22 * a31;

		float detInv = 1.0f / matrix.Determinant();
		for (int i = 0; i < 16; i += 4)
		{
			newElements[i + 0] *= detInv;
			newElements[i + 1] *= detInv;
			newElements[i + 2] *= detInv;
			newElements[i + 3] *= detInv;
		}

		return mat4(newElements);
	}

	mat4 mat4::Transpose(const mat4& matrix)
	{
		float newElements[16];
		memcpy(newElements, matrix.elements, sizeof(newElements));

		b12 = a21;
		b21 = a12;

		b13 = a31;
		b31 = a13;

		b14 = a41;
		b41 = a14;

		b23 = a32;
		b32 = a23;

		b24 = a42;
		b42 = a24;

		b34 = a43;
		b43 = a34;

		return mat4(newElements);
	}

#undef a11
#undef a12
#undef a13
#undef a14
#undef a21
#undef a22
#undef a23
#undef a24
#undef a31
#undef a32
#undef a33
#undef a34
#undef a41
#undef a42
#undef a43
#undef a44

#undef b11
#undef b12
#undef b13
#undef b14
#undef b21
#undef b22
#undef b23
#undef b24
#undef b31
#undef b32
#undef b33
#undef b34
#undef b41
#undef b42
#undef b43
#undef b44

}