#include "infoMonkey.hpp"
#include "obj_loader.hpp"

snake::InfoMonkey::InfoMonkey(const MonkeyShader& shader,
	const PickingShader& pickingShader) {
	IndexedModel model;
	IndexedModel simpleModel;
	MakeMonkeyIndexedModel(model, simpleModel);
	mesh = new MonkeyMesh(model, simpleModel, pickingShader, shader);
}

snake::InfoMonkey::~InfoMonkey() {
	delete mesh;
}

void snake::InfoMonkey::MakeMonkeyIndexedModel(IndexedModel& model, IndexedModel& simpleModel) {
	const char* monkeyPath = "./res/objs/monkey3.obj";
	OBJModel(monkeyPath).ToIndexedModel(model);
	OBJModel(monkeyPath, 100U).ToIndexedModel(simpleModel);
}