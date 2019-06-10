#include <glm/glm.hpp>

#include "infoBox.hpp"

snake::InfoBox::InfoBox(const BoxShader& shader,
	const PickingShader& pickingShader) :
	texture1("./res/textures/bricks.jpg"),
	texture2("./res/textures/box0.bmp") {
	IndexedModel model;
	MakeBoxIndexedModel(model);

	mesh1 = new BoxMesh(model, model, &texture1, pickingShader, shader);
	mesh2 = new BoxMesh(model, model, &texture2, pickingShader, shader);
}

snake::InfoBox::~InfoBox() {
	delete mesh1;
	delete mesh2;
}

void snake::InfoBox::MakeBoxIndexedModel(IndexedModel& model) {
	model.Clear();
	model.isBox = true;
	model.positions = {
		glm::vec3(-1, -1, -1), glm::vec3(-1, 1, -1), glm::vec3(1, 1, -1), glm::vec3(1, -1, -1),
		glm::vec3(-1, -1, 1), glm::vec3(-1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(1, -1, 1),
		glm::vec3(-1, -1, -1), glm::vec3(-1, -1, 1), glm::vec3(1, -1, 1), glm::vec3(1, -1, -1),
		glm::vec3(-1, 1, -1), glm::vec3(-1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, -1),
		glm::vec3(-1, -1, -1), glm::vec3(-1, -1, 1), glm::vec3(-1, 1, 1), glm::vec3(-1, 1, -1),
		glm::vec3(1, -1, -1), glm::vec3(1, -1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, -1) };

	model.texCoords = {
		glm::vec2(1, 0), glm::vec2(0, 0), glm::vec2(0, 1), glm::vec2(1, 1),
		glm::vec2(1, 0), glm::vec2(0, 0), glm::vec2(0, 1), glm::vec2(1, 1),
		glm::vec2(0, 1), glm::vec2(1, 1), glm::vec2(1, 0), glm::vec2(0, 0),
		glm::vec2(0, 1), glm::vec2(1, 1), glm::vec2(1, 0), glm::vec2(0, 0),
		glm::vec2(1, 1), glm::vec2(1, 0), glm::vec2(0, 0), glm::vec2(0, 1),
		glm::vec2(1, 1), glm::vec2(1, 0), glm::vec2(0, 0), glm::vec2(0, 1) };

	model.normals = {
		glm::vec3(0, 0, -1), glm::vec3(0, 0, -1), glm::vec3(0, 0, -1), glm::vec3(0, 0, -1),
		glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1),
		glm::vec3(0, -1, 0), glm::vec3(0, -1, 0), glm::vec3(0, -1, 0), glm::vec3(0, -1, 0),
		glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0),
		glm::vec3(-1, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(-1, 0, 0),
		glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0) };

	model.colors = {
		glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1),	glm::vec3(0, 0, 1),
		glm::vec3(0, 1, 1),	glm::vec3(0, 1, 1), glm::vec3(0, 1, 1),	glm::vec3(0, 1, 1),
		glm::vec3(0, 1, 0),	glm::vec3(0, 1, 0),	glm::vec3(0, 1, 0),	glm::vec3(0, 1, 0),
		glm::vec3(1, 1, 0),	glm::vec3(1, 1, 0),	glm::vec3(1, 1, 0), glm::vec3(1, 1, 0),
		glm::vec3(1, 0, 0),	glm::vec3(1, 0, 0),	glm::vec3(1, 0, 0),	glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 1),	glm::vec3(1, 0, 1),	glm::vec3(1, 0, 1),	glm::vec3(1, 0, 1) };

	model.indices = {
		0, 1, 2,
		0, 2, 3,

		6, 5, 4,
		7, 6, 4,

		10, 9, 8,
		11, 10, 8,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,

		22, 21, 20,
		23, 22, 20 };
}