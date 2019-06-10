#include "box.hpp"
#include "cameras.hpp"
#include "snake.hpp"

namespace snake{
	static const float epsilon = .5f;

	static inline float max(float a, float b) {
		return a > b ? a : b;
	}
}

const float snake::Box::speed = .2f;

snake::Box::Box(const InfoBox& infoBox, uint pickingColor, int sceneSize) :
	GameObject(), pickingColor(pickingColor), r(1), sceneSize(sceneSize) {
	//select mesh:
	mesh = random() > .5f ? infoBox.mesh1 : infoBox.mesh2;
	
	//init translation:
	moveTo = position = GetRandomLocation();
	CalcDir();

	//init orientation:
	glm::vec3 v;
	v.x = random() > .5f ? random() : -random();
	v.y = random() > .5f ? random() : -random();
	v.z = random() > .5f ? random() : -random();
	float length = glm::length(v);
	if (length > 0.001) {
		v /= length;
		float angle = random() * 360;
		r = glm::rotate(angle, v);
	}

	//init scale:
	v.x = max(random() * 10, 1.f);
	v.y = max(random() * 10, 1.f);
	v.z = max(random() * 10, 1.f);
	radius = glm::length(v);
	s = glm::scale(v);
}

void snake::Box::CheckCollision(Snake& snake) {
	const Kdtree& collider = mesh->collider;
	glm::mat4 trans = GetTrans();
	if (snake.CollidedWith(collider, trans, position, radius))
		snake.SetIsDead();
}

void snake::Box::Draw(const Camera& camera) const {
	glm::mat4 modelView = camera.view * GetTrans();
	glm::vec3 camPos(camera.view * glm::vec4(0, 0, 0, 1));
	mesh->Draw(camera.projection, modelView, camPos);
}

void snake::Box::CalcActions() {
	CalcDir();
	position += speed * dir;
}

void snake::Box::CalcDir() {
	float dist = glm::distance(moveTo, position);
	if(dist > epsilon)
		return;
	
	do {
		moveTo = GetRandomLocation();
		dist = glm::distance(moveTo, position);
	} while (dist < epsilon);

	dir = glm::normalize(moveTo - position);
}