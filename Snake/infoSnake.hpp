#pragma once

#include "meshes.hpp"
#include "shaders.hpp"
#include "texture.hpp"

typedef unsigned int uint;

namespace snake {
	struct InfoSnake {
		//snake texture:
		Texture texture;
		
		//snake meshes:
		SnakeMesh* tail;
		SnakeMesh* head;
		SnakeMesh* belly;

		InfoSnake(const SnakeShader& shader,
			const PickingShader& pickingShader);

		virtual ~InfoSnake();
	};
}


