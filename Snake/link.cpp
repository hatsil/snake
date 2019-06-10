#include "snake.hpp"

snake::Link::Link(const SnakeMesh& mesh, uint pickingColor, const glm::mat4& scale):
	mesh(mesh), pickingColor(pickingColor),
	r(glm::mat4(1)), s(scale) {
	glm::vec3 tVec(scale * glm::vec4(0, 0, 1, 0));
	t1 = t2 = glm::translate(tVec);
}

void snake::Link::Draw(const Camera& camera, const glm::mat4& parentTrans, Link* nextLink) const {
	const glm::mat4& projection = camera.projection;
	const glm::mat4& view = camera.view;
	glm::mat4 nextTrans(nextLink ? nextLink->GetTrans() : glm::mat4(1));

	glm::mat4 prevModelView = view * parentTrans;
	glm::mat4 curModelView = view * GetTrans();
	glm::mat4 nextModelView = view * nextTrans;
	glm::vec3 camPos(camera.view * glm::vec4(0, 0, 0, 1));

	mesh.Draw(projection, prevModelView * s, curModelView * s, nextModelView * s, camPos);
}

void snake::Link::DrawForPicking(const Camera& camera) const {
	mesh.DrawForPicking(camera.ViewProjection() * GetTrans() * s, pickingColor);
}

void snake::Link::AddRotationToHead(Link& head, float angle, const glm::vec3& rotAxis) {
	glm::mat4& r = head.r;
	r = glm::rotate(r, angle, rotAxis);
}