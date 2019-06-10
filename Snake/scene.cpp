#include <iostream>

#include "scene.hpp"

namespace snake {
	const float far = 1e6f;
}

snake::Scene::Scene(uint maxBoxes, uint maxMonkeys, uint snakeLength, uint maxPoints):
pickingColor(0U),
pickingShader(), snakeShader(), boxShader(), monkeySahder(), caveShader(),
boxes(), monkeys(), cameras(),
projection(glm::perspective(60.0f, 4.0f/3.0f, 0.1f, far)),
currentCamera(0), maxPoints(maxPoints), currentPoints(0U) {

	infoBox = new InfoBox(boxShader, pickingShader);
	infoMonkey = new InfoMonkey(monkeySahder, pickingShader);
	infoSnake = new InfoSnake(snakeShader, pickingShader);
	infoCave = new InfoCave(caveShader, pickingShader);
	
	int sceneSize = 100;

	for(uint i = 0U; i < maxMonkeys; ++i, ++pickingColor)
		monkeys.push_back(new Monkey(*infoMonkey, pickingColor, sceneSize));

	for (uint i = 0U; i < maxBoxes; ++i, ++pickingColor)
		boxes.push_back(new Box(*infoBox, pickingColor, sceneSize));

	snake = new Snake(*infoSnake, snakeLength, boxes, pickingColor);
	cave = new Cave(*infoCave, pickingColor++, sceneSize);

	cameras = {new FrontCamera(projection), new TopCamera(projection)};
}

snake::Scene::Scene(): Scene(10U, 15U, 5U, 3U) {}

void snake::Scene::ForEach(const std::function<void(GameObject*)>& foo) {
	foo(snake);
	foo(cave);

	for(Monkey* monkey : monkeys)
		foo(monkey);

	for(Box* box : boxes)
		foo(box);
}

void snake::Scene::ForEach(const std::function<void(const GameObject*)>& foo) const {
	foo(snake);
	foo(cave);

	for (Monkey* monkey : monkeys)
		foo(monkey);

	for (Box* box : boxes)
		foo(box);
}

snake::Scene::~Scene() {
	ForEach([] (GameObject* gameObject) {
		delete gameObject;
	});

	for(Camera* camera : cameras)
		delete camera;

	delete infoBox;
	delete infoMonkey;
	delete infoSnake;
	delete infoCave;
}

void snake::Scene::CalcActions() {
	ForEach([] (GameObject* gameObject) {
		gameObject->CalcActions();
	});

	cameras[currentCamera]->FixView(*snake);

	ForEach([&] (GameObject* gameObject) {
		gameObject->CheckCollision(*snake);
	});

	if(snake->IsEnteredCave()) {
		//TODO: implement win game
	} else if(snake->IsDead()) {
		//TODO: implement game over
	} else {
		currentPoints += snake->CollectPoints();
		if(currentPoints >= maxPoints)
			cave->OpenCave();
	}
}

void snake::Scene::Draw() const {
	Camera* camera = cameras[currentCamera];
	ForEach([=] (const GameObject* gameObject) {
		gameObject->Draw(*camera);
	});
}

void snake::Scene::DrawForPicking() const {
	Camera* camera = cameras[currentCamera];
	ForEach([=] (const GameObject* gameObject) {
		gameObject->DrawForPicking(*camera);
	});
}

void snake::Scene::ApplyPickedColor(uint pickedColor) {
	GameObject* pickedObject = 0;

	if(pickedColor < monkeys.size())
		pickedObject = monkeys[pickedColor];
	else if(pickedColor == cave->pickingColor)
		pickedObject = cave;

	if(pickedObject)
		snake->SetDestination(*pickedObject);
}

void snake::Scene::SwitchCamera() {
	currentCamera = !currentCamera;
}

void snake::Scene::MoveUp() {
	snake->MoveUp();
}

void snake::Scene::MoveDown() {
	snake->MoveDown();
}

void snake::Scene::MoveRight() {
	snake->MoveRight();
}

void snake::Scene::MoveLeft() {
	snake->MoveLeft();
}

void snake::Scene::Resize(int width, int height) {
	glViewport(0, 0, width, height);
	projection = glm::perspective(60.0f, (float)width/(float)height, 0.1f, far);
}