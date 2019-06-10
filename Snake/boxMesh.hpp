#pragma once

#include "mesh.hpp"

namespace snake {
	class BoxMesh : public Mesh {
	public:
		BoxMesh(const IndexedModel& model,
			const IndexedModel& simpleModel,
			const Texture* texture,
			const PickingShader& pickingShader,
			const BoxShader& shader);

		virtual ~BoxMesh() {}

		void Draw(const glm::mat4 & projection, const glm::mat4 & MV, const glm::vec3& camPos) const;

	private:
		const BoxShader& shader;
	};
}