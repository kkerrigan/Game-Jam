#pragma once
#include "LinearMath/btVector3.h"
#include <glm/vec3.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
namespace physics
{
	/*!	\class	bullet_adapters
		\brief  
		\author Daniel Maclam
		\date	2019-02-11 8:05 PM
		*/
	namespace bullet_adapters
	{
		inline btVector3 to_bullet(const glm::vec3 value)
		{
			return btVector3(value.x, value.y, value.z);
		}

		inline glm::vec3 to_glm(const btVector3 value)
		{
			return glm::vec3(value.x(), value.y(),value.z());
		}

		inline btScalar to_bullet(const float value)
		{
			return btScalar(value);
		}

		inline float to_glm(const btScalar value)
		{
			return static_cast<float>(value);
		}

		inline btQuaternion to_bullet(const glm::quat quaternion)
		{
			return btQuaternion(
				to_bullet(quaternion.x), 
				to_bullet(quaternion.y),
				to_bullet(quaternion.z), 
				to_bullet(quaternion.w)
			);
		}

		inline glm::quat to_glm(const btQuaternion quaternion)
		{
			return glm::quat(
				to_glm(quaternion.w()), 
				to_glm(quaternion.x()),
				to_glm(quaternion.y()), 
				to_glm(quaternion.z())
			);
		}

		inline glm::quat create_quaternion_from_euler_angles(const glm::vec3 angles)
		{
			const glm::vec3 orientation = glm::vec3(glm::radians(angles.x), glm::radians(angles.y), glm::radians(angles.z));
			return glm::quat(glm::vec3(orientation.x, orientation.y, orientation.z));
		}

	}
}
