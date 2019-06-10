#pragma once

#include <string>
#include <vector>
#include <functional>

#include <glm/glm.hpp>

typedef unsigned int uint;

namespace snake {
	enum Attributes {
		POSITION, TEXCOORD, COLOR, NORMAL, WEIGHT_PREV, WEIGHT_CUR, WEIGHT_NEXT
	};

	class Shader {
	public:
		Shader(const std::string& fileName);
		virtual ~Shader();

		int GetAttribIndex(Attributes attribute) const;
	
	protected:
		static std::string LoadShader(const std::string& fileName);
		
		static void CheckShaderError(uint shader,
			uint flag,
			bool isProgram,
			const std::string& errorMessage);

		static uint CreateShader(const std::string& text, uint type);

		uint m_program;
		std::vector<uint> m_shaders;
		std::vector<uint> m_uniforms;
	};

	class AdvancingShader: public Shader {
	public:
		AdvancingShader(const char* fileName);

		virtual void Update(const glm::mat4 & projection,
			const glm::mat4 & MV, const glm::vec3& camPos) const;

		virtual ~AdvancingShader() {}
	};
}