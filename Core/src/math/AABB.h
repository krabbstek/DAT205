#pragma once

#include "Common.h"

#include "vec3.h"
#include "mat4.h"

namespace core {

	struct CORE_API AABB
	{
	public:
		vec3 min;
		vec3 max;

		AABB();
		AABB(const vec3& min, const vec3& max);
		AABB(float posX, float posY, float posZ, float sizeX, float sizeY, float sizeZ);

		AABB& Add(float x, float y, float z);
		AABB& Add(const vec3& vec);
		static AABB Add(AABB aabb, float x, float y, float z);
		static AABB Add(AABB aabb, const vec3& vec);

		AABB& Subtract(float x, float y, float z);
		AABB& Subtract(const vec3& vec);
		static AABB Subtract(AABB aabb, float x, float y, float z);
		static AABB Subtract(AABB aabb, const vec3& vec);

		AABB& Transform(const mat4& transformationMatrix);
		static AABB Transform(AABB aabb, const mat4& transformationMatrix);

		friend AABB operator+(const AABB& aabb, float f);
		friend AABB operator+(const AABB& aabb, const vec3& vec);
		friend AABB operator-(const AABB& aabb, float f);
		friend AABB operator-(const AABB& aabb, const vec3& vec);

		AABB& operator+=(float f);
		AABB& operator+=(const vec3& vec);
		AABB& operator-=(float f);
		AABB& operator-=(const vec3& vec);

		bool Intersects(const AABB& other) const;
		bool Contains(const vec3& point) const;

		vec3 GetPosition() const { return 0.5f * vec3(min.x + max.x, min.y + max.y, min.z + max.z); }
		vec3 GetSize() const { return max - min; }

		void SetPosition(float x, float y, float z);
		void SetPosition(const vec3& position);
		void SetSize(float x, float y, float z);
		void SetSize(vec3 size);
	};

}