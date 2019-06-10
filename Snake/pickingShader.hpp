#pragma once

#include <glm/glm.hpp>

#include "shader.hpp"

namespace snake {
	class PickingShader: public Shader {
	public:
		PickingShader();
		virtual ~PickingShader() {}

		void Update(const glm::mat4& mvp, uint pickingColor) const;
	};
}
