#pragma once

#include "shader.hpp"

namespace snake {
	class BoxShader : public AdvancingShader {
	public:
		BoxShader();
		virtual ~BoxShader() {}
	};
}