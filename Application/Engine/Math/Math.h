#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtx/color_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Transform.h"

namespace nc
{
	inline float random()
	{
		return rand() / static_cast<float>(RAND_MAX); // 0 - RAND_MAX ->  0 / 10 = 0 | 5 / 10 = 0.5 | 10 / 10 = 1
	}

	inline float random(float min, float max) // 6.0 - 10.0
	{
		if (min > max)
		{
			std::swap(min, max);
		}
		return min + ((max - min) * random()); // 0 - 4
	}
}