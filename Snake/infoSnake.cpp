#include <cmath>

#include "infoSnake.hpp"
#include "cylinderIndexedModel.hpp"

namespace snake {
	static const float epsilon = 1e-3f;
	static const glm::vec3 c1 = glm::vec3(1, 0, 0);
	static const glm::vec3 c2 = glm::vec3(0, 0, 1);

	static struct {
		std::function<Weight(float)> calcWeight = [](float t) -> Weight {
			if (t < 1.f / 3.f) {
				float t2 = t * 1.5f;
				return Weight(.5f - t2, t2 + .5f, 0.f);
			}
			
			if (t >= 1.f / 3.f && t <= 2.f / 3.f)
				return Weight(0.f, 1.f, 0.f);

			float t2 = 1.5f * (1.f - t);
			return Weight(0.f, .5f + t2, .5f - t2);
		};

		std::function<glm::vec3(float)> calcColor = [](float t) -> glm::vec3 {
			float t2 = t * 2.f;
			if (t < .5f)
				return (1.f - t2)*c1 + t2 * c2;

			t2 -= 1.0f;
			return (1.f - t2)*c2 + t2 * c1;
		};

		std::function<glm::vec3(float)> calcNormal = [](float t) -> glm::vec3 {
			return glm::vec3(1, 0, 0);
		};

		std::function<float(float)> calcRadius = [](float t) -> float {
			return 1.f;
		};

	} bellyFuncs;

	static struct {
		std::function<Weight(float)> calcWeight = [](float t) -> Weight {
			if (t < 1.f / 3.f) {
				float t2 = t * 1.5f;
				return Weight(.5f - t2, t2 + .5f, 0.f);
			}

			return Weight(0.f, 1.f, 0.f);
		};

		std::function<glm::vec3(float)> calcColor = [](float t) -> glm::vec3 {
			return (1.f - t)*c1 + t * c2;
		};

		std::function<glm::vec3(float)> calcNormal = [](float t) -> glm::vec3 {
			float t2 = 1.f - t * t;

			if (t2 < epsilon)
				return glm::vec3(1, 0, 0);

			if (t < epsilon)
				return glm::vec3(0, 0, 1);

			float m = std::sqrt(t2) / (2.f * t);

			return glm::normalize(glm::vec3(1, 0, m));
		};

		std::function<float(float)> calcRadius = [](float t) -> float { //radius
			float r2 = 1.f - t * t;
			if (r2 < 0.f)
				return 0.f;

			return std::sqrt(r2);
		};
	} headFuncs;

	static struct {
		std::function<Weight(float)> calcWeight = [](float t) -> Weight {
			if (t <= 2.f / 3.f)
				return Weight(0.f, 1.f, 0.f);

			float t2 = 1.5f * (1.f - t);
			return Weight(0.f, .5f + t2, .5f - t2);
		};

		std::function<glm::vec3(float)> calcColor = [](float t) -> glm::vec3 {
			return (1.f - t) * c2 + t * c1;
		};

		std::function<glm::vec3(float)> calcNormal = [](float t) -> glm::vec3 {
			float t2 = 1.f - t * t;

			if (t2 < epsilon)
				return glm::vec3(1, 0, 0);

			if (t < epsilon)
				return glm::vec3(0, 0, -1);

			float m = -std::sqrt(t2) / (2.f * t);

			return glm::normalize(glm::vec3(1, 0, m));
		};

		std::function<float(float)> calcRadius = [](float t) -> float {
			float t1 = 1.f - t;
			float r2 = 1.f - t1 * t1;
			
			if (r2 < 0.f)
				return 0.f;

			return std::sqrt(r2);
		};
	} tailFuncs;
}

snake::InfoSnake::InfoSnake(const SnakeShader& shader,
	const PickingShader& pickingShader):
	texture("./res/textures/snake_tex.jpg") {
	int stacks = 4;
	int slices = 4;
	
	IndexedModel model;
	IndexedModel simpleModel;
	
	MakeCylinderIndexedModel(model, bellyFuncs.calcWeight, bellyFuncs.calcColor, bellyFuncs.calcNormal, bellyFuncs.calcRadius);
	MakeCylinderIndexedModel(simpleModel, bellyFuncs.calcWeight, bellyFuncs.calcColor, bellyFuncs.calcNormal, bellyFuncs.calcRadius, stacks, slices);
	belly = new SnakeMesh(model, simpleModel, &texture, pickingShader, shader);

	MakeCylinderIndexedModel(model, headFuncs.calcWeight, headFuncs.calcColor, headFuncs.calcNormal, headFuncs.calcRadius);
	MakeCylinderIndexedModel(simpleModel, headFuncs.calcWeight, headFuncs.calcColor, headFuncs.calcNormal, headFuncs.calcRadius, stacks, slices);
	head = new SnakeMesh(model, simpleModel, &texture, pickingShader, shader);

	MakeCylinderIndexedModel(model, tailFuncs.calcWeight, tailFuncs.calcColor, tailFuncs.calcNormal, tailFuncs.calcRadius);
	MakeCylinderIndexedModel(simpleModel, tailFuncs.calcWeight, tailFuncs.calcColor, tailFuncs.calcNormal, tailFuncs.calcRadius, stacks, slices);
	tail = new SnakeMesh(model, simpleModel, &texture, pickingShader, shader);
}

snake::InfoSnake::~InfoSnake() {
	delete tail;
	delete head;
	delete belly;
}