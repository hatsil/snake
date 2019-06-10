#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "cave.hpp"
#include "cameras.hpp"
#include "snake.hpp"
#include "random.hpp"

const glm::mat4 snake::Cave::sDraw = glm::scale(glm::vec3(20));
const glm::mat4 snake::Cave::sCollide = glm::scale(glm::vec3(5));

snake::Cave::Cave(const InfoCave& infoCave, uint pickingColor, int sceneSize) :
	GameObject(), infoCave(infoCave),
	pickingColor(pickingColor), scale(glm::scale(glm::vec3(0))),
	openCounter(1), isOpen(false), timeSlice(0), timeStamp(0.f) {
	position.x = (random() > .5f ? random() : -random()) * .5f * sceneSize;
	position.y = (random() > .5f ? random() : -random()) * .5f * sceneSize;
	position.z = (random() > .5f ? random() : -random()) * .5f * sceneSize;
	t = glm::translate(position);
}

void snake::Cave::CalcActions() {
	const static int totalTimeSlices = 30;
	const static int totalOpenCount = 200;
	timeStamp = (float)timeSlice / totalTimeSlices;
	timeSlice = (timeSlice + 1) % totalTimeSlices;
	if(isOpen){
		float openScaleFactor = (float)openCounter / totalOpenCount;
		scale = glm::scale(glm::vec3(openScaleFactor));
		openCounter = std::min(totalOpenCount, openCounter + 1);
	}
}

void snake::Cave::CheckCollision(Snake& snake) {
	if(isOpen) {
		const Kdtree& collider = infoCave.mesh->collider;
		glm::mat4 trans = GetCollisionTrans();
		float radius = glm::distance(glm::vec3(trans * glm::vec4(0, 0, 1, 1)), position);
		if(snake.CollidedWith(collider, trans, position, radius))
			snake.SetIsEnteredCave();
	}
}

void snake::Cave::Draw(const Camera& camera) const {
	if(isOpen) {
		glm::mat4 modelView = camera.view * GetDrawingTrans();
		glm::vec3 camPos(camera.view * glm::vec4(0, 0, 0, 1));
		infoCave.mesh->Draw(camera.projection, modelView, camPos, timeStamp);
	}
}

void snake::Cave::DrawForPicking(const Camera& camera) const {
	if(isOpen) {
		glm::mat4 mvp = camera.ViewProjection() * GetDrawingTrans();
		infoCave.mesh->DrawForPicking(mvp, pickingColor);
	}
}