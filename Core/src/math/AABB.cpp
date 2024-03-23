#include "pch.h"

#include "AABB.h"

namespace core {

	AABB::AABB()
	{
	}

	AABB::AABB(const vec3& min, const vec3& max)
		: min(min), max(max)
	{
	}

	AABB::AABB(float posX, float posY, float posZ, float sizeX, float sizeY, float sizeZ)
		: min(posX - 0.5f * sizeX, posY - 0.5f * sizeY, posZ - 0.5f * sizeZ), max(posX + 0.5f * sizeX, posY + 0.5f * sizeY, posZ + 0.5f * sizeZ)
	{
	}


	AABB& AABB::Add(float x, float y, float z)
	{
		min.x += x;
		min.y += y;
		min.z += z;
		max.x += x;
		max.y += y;
		max.z += z;
		return *this;
	}

	AABB& AABB::Add(const vec3& vec)
	{
		min += vec;
		max += vec;
		return *this;
	}

	AABB AABB::Add(AABB aabb, float x, float y, float z)
	{
		return aabb.Add(x, y, z);
	}

	AABB AABB::Add(AABB aabb, const vec3& vec)
	{
		return aabb.Add(vec);
	}


	AABB& AABB::Subtract(float x, float y, float z)
	{
		min.x -= x;
		min.y -= y;
		min.z -= z;
		max.x -= x;
		max.y -= y;
		max.z -= z;
		return *this;
	}

	AABB& AABB::Subtract(const vec3& vec)
	{
		min -= vec;
		max -= vec;
		return *this;
	}

	AABB AABB::Subtract(AABB aabb, float x, float y, float z)
	{
		return aabb.Subtract(x, y, z);
	}

	AABB AABB::Subtract(AABB aabb, const vec3& vec)
	{
		return aabb.Subtract(vec);
	}


	AABB& AABB::Transform(const mat4& transformationMatrix)
	{
		return Add(transformationMatrix.GetColumn(3).xyz);
	}

	AABB AABB::Transform(AABB aabb, const mat4& transformationMatrix)
	{
		return aabb.Transform(transformationMatrix);
	}


	AABB operator+(const AABB& aabb, float f)
	{
		return AABB::Add(aabb, f);
	}

	AABB operator+(const AABB& aabb, const vec3& vec)
	{
		return AABB::Add(aabb, vec);
	}

	AABB operator-(const AABB& aabb, float f)
	{
		return AABB::Subtract(aabb, f);
	}

	AABB operator-(const AABB& aabb, const vec3& vec)
	{
		return AABB::Subtract(aabb, vec);
	}


	AABB& AABB::operator+=(float f)
	{
		return Add(f);
	}

	AABB& AABB::operator+=(const vec3& vec)
	{
		return Add(vec);
	}

	AABB& AABB::operator-=(float f)
	{
		return Subtract(f);
	}

	AABB& AABB::operator-=(const vec3& vec)
	{
		return Subtract(vec);
	}


	bool AABB::Intersects(const AABB& other) const
	{
		return !(min.x > other.max.x || max.x < other.min.x ||
			min.y > other.max.y || max.y < other.min.y ||
			min.z > other.max.z || max.z < other.min.z);
	}

	bool AABB::Contains(const vec3& point) const
	{
		return (point >= min && point <= max);
	}


	void AABB::SetPosition(float x, float y, float z)
	{
		vec3 currentPos = 0.5f * (min + max);
		vec3 d(x - currentPos.x, y - currentPos.y, z - currentPos.z);
		min += d;
		max += d;
	}

	void AABB::SetPosition(const vec3& position)
	{
		vec3 currentPos = 0.5f * (min + max);
		vec3 d(position.x - currentPos.x, position.y - currentPos.y, position.z - currentPos.z);
		min += d;
		max += d;
	}

	void AABB::SetSize(float x, float y, float z)
	{
		vec3 position = 0.5f * (min + max);
		x *= 0.5f;
		y *= 0.5f;
		z *= 0.5f;
		min = vec3(position.x - x, position.y - y, position.z - z);
		max = vec3(position.x + x, position.y + y, position.z + z);
	}

	void AABB::SetSize(vec3 size)
	{
		vec3 position = 0.5f * (min + max);
		size *= 0.5f;
		min = vec3(position.x - size.x, position.y - size.y, position.z - size.z);
		max = vec3(position.x + size.x, position.y + size.y, position.z + size.z);
	}

}