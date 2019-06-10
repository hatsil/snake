#pragma once

#include "meshes.hpp"
#include "shaders.hpp"

namespace snake {
	struct InfoMonkey {
		MonkeyMesh* mesh;

		InfoMonkey(const MonkeyShader& shader,
			const PickingShader& pickingShader);

		virtual ~InfoMonkey();

	private:
		static void MakeMonkeyIndexedModel(IndexedModel& model, IndexedModel& simpleModel);
	};
}