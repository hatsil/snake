#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "monkey.hpp"
#include "cameras.hpp"
#include "snake.hpp"
#include "random.hpp"

const glm::mat4 snake::Monkey::s = glm::scale(glm::vec3(1.25f, 1.25f, 1.25f));

snake::Monkey::Monkey(const InfoMonkey& infoMonkey, uint pickingColor, int sceneSize) :
	GameObject(), infoMonkey(infoMonkey),
	pickingColor(pickingColor),
	isAlive(true), r(1) {
	position.x = (random() > .5f ? random() : -random()) * .5f * sceneSize;
	position.y = (random() > .5f ? random() : -random()) * .5f * sceneSize;
	position.z = (random() > .5f ? random() : -random()) * .5f * sceneSize;
	t = glm::translate(position);
	radius = s[0][0] * .6f;
}

void snake::Monkey::CalcActions() {
	if (isAlive)
		r = glm::rotate(r, 1.0f, glm::vec3(0, 1, 0));
}

void snake::Monkey::CheckCollision(Snake& snake) {
	if (isAlive) {
		const Kdtree& collider = infoMonkey.mesh->collider;
		glm::mat4 trans = GetTrans();
		if (snake.CollidedWith(collider, trans, position, radius)) {
			snake.AddPoint();
			isAlive = false;
		}
	}
}

void snake::Monkey::Draw(const Camera& camera) const {
	if (isAlive) {
		glm::mat4 modelView = camera.view * GetTrans();
		glm::vec3 camPos(camera.view * glm::vec4(0, 0, 0, 1));
		infoMonkey.mesh->Draw(camera.projection, modelView, camPos);
	}
}

void snake::Monkey::DrawForPicking(const Camera& camera) const {
	if (isAlive) {
		glm::mat4 mvp = camera.ViewProjection() * GetTrans();
		infoMonkey.mesh->DrawForPicking(mvp, pickingColor);
	}
}