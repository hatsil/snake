#pragma once

#include "shaders.hpp"

namespace snake {
	class CaveShader: public AdvancingShader {
	public:
		CaveShader();
		virtual ~CaveShader() {}

		void Update(const glm::mat4 & projection,
			const glm::mat4 & MV,
			const glm::vec3& camPos,
			float timeStamp) const;
	};
}

