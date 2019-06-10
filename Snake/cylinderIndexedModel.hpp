#pragma once

#include <functional>

#include <glm/glm.hpp>

#include "indexedModel.hpp"

namespace snake {
	void MakeCylinderIndexedModel(IndexedModel& model,
		const std::function<Weight(float)>& calcWeight,
		const std::function<glm::vec3(float)>& calcColor,
		const std::function<glm::vec3(float)>& calcNormal,
		const std::function<float(float)>& calcRadius,
		int stacks = 20,
		int slices = 20);
}