#include "monkeyMesh.hpp"

snake::MonkeyMesh::MonkeyMesh(const IndexedModel& model,
	const IndexedModel& simpleModel,
	const PickingShader& pickingShader,
	const MonkeyShader& shader): Mesh(model, simpleModel, 0, shader, pickingShader, 5U), shader(shader) {}

void snake::MonkeyMesh::Draw(const glm::mat4 & projection, const glm::mat4 & MV, const glm::vec3& camPos) const {
	shader.Update(projection, MV, camPos);
	Mesh::Draw();
}