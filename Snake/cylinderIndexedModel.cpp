#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "cylinderIndexedModel.hpp"

void snake::MakeCylinderIndexedModel(IndexedModel& model,
	const std::function<Weight(float)>& calcWeight,
	const std::function<glm::vec3(float)>& calcColor,
	const std::function<glm::vec3(float)>& calcNormal,
	const std::function<float(float)>& calcRadius, int stacks, int slices) {

	model.Clear();

	const float dt = 1.f / stacks;
	const float dangle = 360.f / slices;
	const float dtexX = 1.f / slices;
	float t = 0.f;
	for (int stack = 0; stack <= stacks; ++stack, t += dt) {
		Weight weight = calcWeight(t);
		glm::vec3 color = calcColor(t);
		glm::vec3 normal = calcNormal(t);
		float radius = calcRadius(t);

		glm::vec4 position(radius, 0.f, -1.f + 2.f*t, 1.f);
		float angle = 0.f;
		float texX = 0.f;

		for (int slice = 0; slice <= slices; ++slice, angle += dangle, texX += dtexX) {
			glm::mat4 rotation(glm::rotate(angle, glm::vec3(0, 0, 1)));
			glm::vec3 rotatedPosition(rotation * position);
			glm::vec3 rotatedNormal(rotation * glm::vec4(normal, 0));

			model.positions.push_back(rotatedPosition);
			model.texCoords.push_back(glm::vec2(texX, t));
			model.normals.push_back(rotatedNormal);
			model.colors.push_back(color);
			model.weights.push_back(weight);
		}
	}

	for (uint stack = 0U; stack < stacks; ++stack)
		for (uint slice = 0U; slice < slices; ++slice) {
			uint bottomLeft = stack * (slices + 1U) + slice;
			uint bottomRight = bottomLeft + 1U;
			uint topLeft = bottomLeft + slices + 1U;
			uint topRight = topLeft + 1U;

			model.indices.push_back(bottomLeft);
			model.indices.push_back(bottomRight);
			model.indices.push_back(topRight);

			model.indices.push_back(bottomLeft);
			model.indices.push_back(topRight);
			model.indices.push_back(topLeft);
		}
}