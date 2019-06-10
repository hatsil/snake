#pragma once

#include <glm/glm.hpp>

#include "shader.hpp"

namespace snake {
	class SnakeShader: public AdvancingShader {
	public:
		SnakeShader();
		virtual ~SnakeShader() {}

		void Update(const glm::mat4& projection,
			const glm::mat4& prevLinkMV,
			const glm::mat4& curLinkMV,
			const glm::mat4& nextLinkMV,
			const glm::vec3& camPos) const;
	};
}

