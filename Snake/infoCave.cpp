#include <cmath>

#include "infoCave.hpp"
#include "cylinderIndexedModel.hpp"

namespace snake {
	static const float epsilon = 1e-3f;
	static const glm::vec3 red = glm::vec3(1, 0, 0);
	static const glm::vec3 green = glm::vec3(0, 1, 0);
	static const glm::vec3 blue = glm::vec3(0, 0, 1);

	static struct {
		std::function<Weight(float)> calcWeight = [](float t) -> Weight {
			return Weight(0.f, 1.f, 0.f);
		};

		std::function<glm::vec3(float)> calcColor = [](float t) -> glm::vec3 {
			if(t < .5f) {
				float t2 = 2.f * t;
				if(t2 < 2.f / 3.f) {
					float t3 = 1.5f * t2;
					return green + t3 * red;
				} else {
					float t3 = 3.f * (1.f - t2);
					return t3 * green + red;
				}
			} else {
				float t2 = 2.f * t - 1.f;
				if(t2 < 2.f / 3.f) {
					float t3 = 1.5f * t2;
					return red + t3 * blue;
				} else {
					float t3 = 3.f * (1.f - t2);
					return t3 * red + blue;
				}
			}
		};

		std::function<glm::vec3(float)> calcNormal = [](float t) -> glm::vec3 {
			if(t < epsilon)
				return glm::vec3(0, 0, -1);

			if(t > 1.f - epsilon)
				return glm::vec3(0, 0, 1);
			
			float z = 2.f * t - 1.f;
			float x = std::sqrt(1.f - z*z);
			return glm::normalize(glm::vec3(x, 0, z));
		};

		std::function<float(float)> calcRadius = [](float t) -> float { //radius
			float t2minus1 = 2.f * t - 1.f;
			float insideSqrt = 1.f - t2minus1 * t2minus1;
			
			if(insideSqrt < epsilon)
				return 0.f;

			return std::sqrt(insideSqrt);
		};
	} caveFuncs;
}

snake::InfoCave::InfoCave(const CaveShader& shader,
	const PickingShader& pickingShader) {
	int simpleStacks = 4;
	int simpleSlices = 6;
	int stacks = 100;
	int slices = 360;

	IndexedModel model;
	IndexedModel simpleModel;
	MakeCylinderIndexedModel(model, caveFuncs.calcWeight, caveFuncs.calcColor, caveFuncs.calcNormal, caveFuncs.calcRadius, stacks, slices);
	MakeCylinderIndexedModel(simpleModel, caveFuncs.calcWeight, caveFuncs.calcColor, caveFuncs.calcNormal, caveFuncs.calcRadius, simpleStacks, simpleSlices);

	mesh = new CaveMesh(model, simpleModel, pickingShader, shader);
}

snake::InfoCave::~InfoCave() {
	delete mesh;
}