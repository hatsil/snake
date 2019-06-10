#pragma once

#include "meshes.hpp"
#include "shaders.hpp"
#include "texture.hpp"

namespace snake {
	struct InfoBox {
		Texture texture1;
		Texture texture2;
		BoxMesh* mesh1;
		BoxMesh* mesh2;

		InfoBox(const BoxShader& shader,
			const PickingShader& pickingShader);

		virtual ~InfoBox();

	private:
		static void MakeBoxIndexedModel(IndexedModel& model);
	};
}