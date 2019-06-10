#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "texture.hpp"
#include "shaders.hpp"
#include "indexedModel.hpp"
#include "kdtree.hpp"

typedef unsigned int uint;

namespace snake {
	class Mesh {
	public:
		Kdtree collider;

		Mesh(const IndexedModel& model,
			const IndexedModel& simpleModel,
			const Texture* texture,
			const Shader& shader,
			const PickingShader& pickingShader,
			uint numBuffers);

		virtual ~Mesh();
		void DrawForPicking(const glm::mat4& mvp, uint pickingColor) const;

	protected:
		void Draw() const;

		const Texture* texture; //Do not delete this
		const PickingShader& pickingShader;
		uint numBuffers;
		std::vector<uint> m_vertexArrayBuffers;
		uint m_numIndices;
		uint m_vertexArrayObject;
	};
}