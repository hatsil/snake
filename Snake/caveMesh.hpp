#pragma once

#include "mesh.hpp"

namespace snake {
	class CaveMesh: public Mesh {
	public:
		CaveMesh(const IndexedModel& model,
			const IndexedModel& simpleModel,
			const PickingShader& pickingShader,
			const CaveShader& shader);

		virtual ~CaveMesh() {}

		void Draw(const glm::mat4 & projection,
			const glm::mat4 & MV,
			const glm::vec3& camPos,
			float timeStamp) const;

	private:
		const CaveShader& shader;
	};
}

