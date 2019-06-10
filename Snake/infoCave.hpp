#pragma once

#include "meshes.hpp"
#include "shaders.hpp"

namespace snake {
	struct InfoCave {
		CaveMesh* mesh;

		InfoCave(const CaveShader& shader,
			const PickingShader& pickingShader);

		virtual ~InfoCave();
	};
}