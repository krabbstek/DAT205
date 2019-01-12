#include "pch.h"

#include "Transform.h"

namespace core {

	Transform::Transform()
		: m_RotationOrder(YXZ)
	{
	}


	mat4 Transform::GetTransformationMatrix() const
	{
		mat4 t = mat4::Translate(position);
		Quaternion qx = Quaternion::RotateX(rotation.x);
		Quaternion qy = Quaternion::RotateY(rotation.y);
		Quaternion qz = Quaternion::RotateZ(rotation.z);
		Quaternion q;
		switch (m_RotationOrder)
		{
		case XYZ:
			q = qx * qy * qz;
			break;
		case XZY:
			q = qx * qz * qy;
			break;
		case YXZ:
			q = qy * qx * qz;
			break;
		case YZX:
			q = qy * qz = qx;
			break;
		case ZXY:
			q = qz * qx * qy;
			break;
		case ZYX:
			q = qz * qy * qx;
			break;
		}
		mat4 r = q.Matrix();
		mat4 s = mat4::Scale(scale);
		return t * r * s;
	}

}