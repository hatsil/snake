#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "texture.hpp"
#include "shaders.hpp"
#include "indexedModel.hpp"
#include "kdtree.hpp"

namespace snake {
	class SnakeMesh {
	public:
		Kdtree collider;

		SnakeMesh(const IndexedModel& model,
			const IndexedModel& simpleModel,
			const Texture* texture,
			const PickingShader& pickingShader,
			const SnakeShader& shader);

		virtual ~SnakeMesh();

		void Draw(const glm::mat4 & projection,
			const glm::mat4 & prevLinkMV,
			const glm::mat4 & curLinkMV,
			const glm::mat4 & nextLinkMV,
			const glm::vec3& camPos) const;

		void DrawForPicking(const glm::mat4& mvp, uint pickingColor) const;

	private:
		void Draw() const;

		const SnakeShader& shader;
		const Texture* texture; //Do not delete this
		const PickingShader& pickingShader;
		uint numBuffers;
		std::vector<uint> m_vertexArrayBuffers;
		uint m_numIndices;
		uint m_vertexArrayObject;
	};
}