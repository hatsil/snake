#pragma once

#include "mesh.hpp"

namespace snake {
	class MonkeyMesh : public Mesh {
	public:
		MonkeyMesh(const IndexedModel& model,
			const IndexedModel& simpleModel,
			const PickingShader& pickingShader,
			const MonkeyShader& shader);

		virtual ~MonkeyMesh() {}

		void Draw(const glm::mat4 & projection, const glm::mat4 & MV, const glm::vec3& camPos) const;

	private:
		const MonkeyShader& shader;
	};
}