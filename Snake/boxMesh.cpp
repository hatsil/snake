#include "boxMesh.hpp"

snake::BoxMesh::BoxMesh(const IndexedModel& model,
	const IndexedModel& simpleModel,
	const Texture* texture,
	const PickingShader& pickingShader,
	const BoxShader& shader): Mesh(model, simpleModel, texture, shader, pickingShader, 5U), shader(shader) {}

void snake::BoxMesh::Draw(const glm::mat4 & projection, const glm::mat4 & MV, const glm::vec3& camPos) const {
	shader.Update(projection, MV, camPos);
	Mesh::Draw();
}