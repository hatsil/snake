#include "caveMesh.hpp"

snake::CaveMesh::CaveMesh(const IndexedModel& model,
	const IndexedModel& simpleModel,
	const PickingShader& pickingShader,
	const CaveShader& shader): Mesh(model, simpleModel, 0, shader, pickingShader, 5U), shader(shader) {}

void snake::CaveMesh::Draw(const glm::mat4 & projection,
	const glm::mat4 & MV,
	const glm::vec3& camPos,
	float timeStamp) const {
	shader.Update(projection, MV, camPos, timeStamp);
	Mesh::Draw();
}
