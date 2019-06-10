#include <cmath>

#include "snake.hpp"
#include "box.hpp"

namespace snake {
	static bool inline isFinite(const glm::vec3& v) {
		return std::isfinite(v.x) && std::isfinite(v.y) && std::isfinite(v.z);
	}

	static float CalcRotationAngle(const glm::vec3& from, const glm::vec3& to) {
		glm::vec3 rAxis = glm::cross(from, to);
		float length = glm::length(rAxis);
		rAxis /= length;
		if (isFinite(rAxis)) {
			float angle = glm::acos(glm::clamp(glm::dot(from, to), -1.0f, 1.0f));
			angle = glm::degrees(angle);
			glm::mat4 r1 = glm::rotate(angle, rAxis);
			glm::mat4 r2 = glm::rotate(-angle, rAxis);
			glm::vec4 from4(from, 0);
			glm::vec3 to1(r1 * from4);
			glm::vec3 to2(r2 * from4);
			return glm::dot(to, to1) > glm::dot(to, to2) ? angle : -angle;
		} else {
			return NAN;
		}
	}
}

const float snake::Snake::defaultSpeed = .5f;
const float snake::Snake::movingAngle = 3.f;

snake::Snake::Snake(const InfoSnake& infoSnake,
	uint snakeLength,
	const std::vector<Box*>& boxes,
	uint& pickingColor):
	GameObject(), boxes(boxes), links(),
	basePos(), numLinks(snakeLength), points(0U),
	isDead(false), isEnteredCave(false),
	speed(defaultSpeed), course() {

	scale = glm::scale(glm::vec3(1, 1, 1.25));
	linkLength = glm::length(scale * glm::vec4(0, 0, 2, 0));
	this->snakeLength = linkLength * numLinks;
	globalTrans = glm::translate(glm::vec3(scale * glm::vec4(basePos,0)));
	
	//Tail link
	links.push_back(new Link(*infoSnake.tail, pickingColor++, scale));

	//Belly links
	for(uint i = 2U; i < numLinks; ++i)
		links.push_back(new Link(*infoSnake.belly, pickingColor++, scale));

	//Head link
	links.push_back(new Link(*infoSnake.head, pickingColor++, scale));

	CalcTransformations();
}

//void snake::Snake::CalcActions() {
//	////collect positions
//	//std::vector<glm::vec3> oldPositions;
//	//for(uint i = 0U; i < numLinks; ++i)
//	//	oldPositions.push_back(GetBasePosition(i));
//
//	//oldPositions.push_back(GetTipPosition(numLinks-1U));
//	//
//	//std::vector<glm::vec3> newPositions(oldPositions);
//	//
//	////set target
//	//std::cout << glm::distance(target, oldPositions[numLinks]) << std::endl;
//	//if (glm::distance(target, oldPositions[numLinks]) < 0.1)
//	//	MoveTarget();
//
//	//glm::vec3 target = this->target;
//	//
//	////Fabrik:
//	//for (uint i = numLinks; i > 0; --i) {
//	//	glm::vec3 dir(glm::normalize(newPositions[i - 1U] - target));
//	//	newPositions[i] = target;
//	//	target += linkLength * dir;
//	//}
//	//newPositions[0U] = target;
//	////if (glm::distance(basePos, this->target) > snakeLength)
//	//	basePos = target;
//	//
//	//	// Step 2 - place effector on original position and calc forwards
//	//	//target = basePos;
//
//	//	//for (uint i = 1; i < newPositions.size(); ++i) {
//	//	//	// Place new joint origin on target.
//	//	//	newPositions[i - 1] = target;
//
//	//	//	// Direction from new joint origin position back to joint tip
//	//	//	glm::vec3 dir = glm::normalize(newPositions[i] - target);
//
//	//	//	//advance target
//	//	//	target += linkLength * dir;
//	//	//}
//	//	//newPositions[numLinks] = target;
//
//
//	////set rotations
//	//bool anyRotation = false;
//	//for (uint i = 0U; i < numLinks; ++i) {
//	//	glm::vec3 oldAxis(glm::normalize(oldPositions[i + 1U] - oldPositions[i]));
//	//	glm::vec3 newAxis(glm::normalize(newPositions[i + 1U] - newPositions[i]));
//	//	anyRotation |= links[i]->Rotate(oldAxis, newAxis);
//	//}
//
//	//if (!anyRotation)
//	//	MoveTarget();
//
//
//	///*for(uint i = 0U; i < numLinks; ++i) {
//	//	glm::vec3 oldAxis(glm::normalize(oldPositions[i+1U] - oldPositions[i]));
//	//	glm::vec3 newAxis(glm::normalize(newPositions[i+1U] - newPositions[i]));
//	//	float angle = glm::degrees(std::acos(glm::dot(oldAxis, newAxis)));
//	//	glm::vec3 rotAxis = glm::cross(oldAxis, newAxis);
//	//	float norm = glm::length(rotAxis);
//	//	if(std::isfinite(angle) & std::isfinite(norm) & norm > epsilon)
//	//		links[i]->Rotate(angle, glm::normalize(rotAxis));
//	//}*/
//
//	//CalcTransformations();
//
//
//	std::vector<glm::vec3> jointPoints;
//	//glm::vec3 origin = basePos;
//
//	// Add all points in ascending order
//	for (uint i = 0U; i < numLinks; ++i)
//		jointPoints.push_back(GetBasePosition(i));
//
//	jointPoints.push_back(GetTipPosition(numLinks-1U));
//
//	// FABRIK
//	// Step 1 - place effector on target and calc backwards
//
//	// Fake animation
//	MoveTarget();
//	glm::vec3 target = this->target;
//	
//
//	for (int i = jointPoints.size() - 1; i > 0; --i) {
//		// Place new joint tip on target.
//		
//
//		// Direction from new joint tip position back to initial joint origin
//		glm::vec3 dir = glm::normalize(jointPoints[i] - target);
//
//		jointPoints[i] = target;
//
//		//advance the target
//		target += linkLength * dir;
//	}
//
//	jointPoints[0] = basePos = target;
//
//	// Step 2 - place effector on original position and calc forwards
//	//target = origin;
//
//	//for (int i = 1; i < jointPoints.size(); i++) {
//	//	// Place new joint origin on target.
//	//	jointPoints[i - 1] = target;
//
//	//	// Direction from new joint origin position back to joint tip
//	//	glm::vec3 dir = glm::normalize(jointPoints[i] - target);
//
//	//	//advance target
//	//	target += linkLength * dir;
//	//}
//
//	//jointPoints[numLinks] = target;
//
//	// The old z axis
//	glm::vec3 oldZ(0, 0, 1);
//
//	// Rotate each joint
//	for (int i = 0; i < numLinks; i++) {
//		const glm::vec3& p1 = jointPoints[i];
//		const glm::vec3& p2 = jointPoints[i + 1];
//
//		glm::vec3 newZ = glm::normalize(p2 - p1);
//		glm::vec3 rAxis = glm::cross(oldZ, newZ);
//
//		if (glm::length(rAxis) > 0.001) {
//			//calculate euler angles
//			float angX = glm::acos(glm::clamp(glm::dot(oldZ, newZ), -1.0f, 1.0f));
//
//			
//			angX = glm::degrees(angX);
//			rAxis = glm::normalize(rAxis);
//			links[i]->r = glm::rotate(angX, rAxis);
//			oldZ = newZ;
//		}
//	}
//	CalcTransformations();
//}

//void snake::Snake::CalcActions() {
//	std::vector<glm::vec3> jointsPositions(numLinks+1U, glm::vec3(0));
//
//	for (uint i = 0U; i < numLinks; ++i)
//		jointsPositions[i] = GetBasePosition(i);
//
//	jointsPositions[numLinks] = GetTipPosition(numLinks - 1U);
//
//	//for (const auto& v : jointsPositions)
//	//	std::cout << v << std::endl;
//
//	
//
//	/*MoveTarget();*/
//	glm::vec3 target = GetTarget();
//
//	for (int i = numLinks; i > 0; --i) {
//		glm::vec3 base = jointsPositions[i - 1];
//		glm::vec3 tip = jointsPositions[i];
//		jointsPositions[i] = target;
//		target = FindMinPositionSphere(target, linkLength, base, tip);
//	}
//
//	//std::cout << target << std::endl;
//	basePos = target - jointsPositions[0U];
//	jointsPositions[0U] = target;
//
//	// The old z axis
//	glm::vec3 oldZ(0, 0, 1), oldY(0, 1, 0);
//	
//	// Rotate each joint
//	for (uint i = 0; i < numLinks; i++) {
//		const glm::vec3& p1 = jointsPositions[i];
//		const glm::vec3& p2 = jointsPositions[i + 1U];
//	
//		glm::vec3 newZ = glm::normalize(p2 - p1);
//		glm::vec3 rAxis = glm::cross(oldZ, newZ);
//	
//		if (glm::length(rAxis) > 0.001) {
//			glm::vec3 vProj = glm::normalize(newZ - glm::dot(newZ, oldZ) * oldZ);
//
//			//calculate euler angles
//			float angZ = glm::acos(glm::clamp(glm::dot(oldY, vProj), -1.0f, 1.0f)),
//				angX = glm::acos(glm::clamp(glm::dot(oldZ, newZ), -1.0f, 1.0f));
//
//			angZ = glm::degrees(angZ);
//			angX = glm::degrees(angX);
//
//			glm::vec3 oldX = glm::cross(oldY, oldZ);
//
//			if (glm::dot(vProj, oldX) > 0)
//				angZ = -angZ;
//
//			rAxis = glm::normalize(rAxis);
//			glm::mat4 r = glm::rotate(angX, rAxis);
//			oldY = glm::vec3(r * glm::vec4(oldY, 0));
//			oldZ = newZ;
//			
//			//shapeNewRotation(angZ, -angX, i);
//			glm::mat4 phi = glm::rotate(angZ, glm::vec3(0, 0, 1));
//			glm::mat4 psi = glm::rotate(-angZ, glm::vec3(0, 0, 1));
//			glm::mat4 theta = glm::rotate(-angX, glm::vec3(1, 0, 0));
//			links[i]->r = phi * theta * psi;
//		}
//	}
//
//	CalcTransformations();
//}

void snake::Snake::CalcActions() {
	std::vector<glm::vec3> joints(numLinks + 1U, glm::vec3());
	for (uint i = 0U; i < numLinks; ++i)
		joints[i] = GetBasePosition(i);

	joints[numLinks] = GetTipPosition(numLinks - 1U);

	//std::system("pause");
	glm::vec3 target = GetTarget();

	for (uint i = numLinks; i > 0U; --i) {
		glm::vec3 dir = glm::normalize(joints[i-1U] - target);
		joints[i] = target;
		target += linkLength * dir;
	}

	//std::cout << target << std::endl;

	basePos = joints[0U] = target;

	glm::vec3 orgZ(0, 0, 1);

	//assign rotations:
	for (uint i = 0; i < numLinks-1U; i++) {
		const glm::vec3& p1 = joints[i];
		const glm::vec3& p2 = joints[i + 1U];
		glm::vec3 newZ = glm::normalize(p2 - p1);
		float angX = CalcRotationAngle(orgZ, newZ);
		if (std::isfinite(angX)) {
			glm::vec3 rAxis = glm::normalize(glm::cross(orgZ, newZ));
			links[i]->r = glm::rotate(angX, rAxis);
		}
	}

	//fix rotations around the zAxis to prevent twists on the skin, to make the snake look better:
	for (int i = numLinks - 2U; i > -1; --i) {
		const glm::mat4& rChild = links[i + 1]->r;
		glm::mat4& rParent = links[i]->r;
		glm::vec3 zChild(rChild * glm::vec4(0, 0, 1, 0));
		glm::vec3 zParent(rParent * glm::vec4(0, 0, 1, 0));
		float rotAngle = CalcRotationAngle(zParent, zChild);
		if (std::isfinite(rotAngle)) {
			glm::vec3 rotAxis = glm::normalize(glm::cross(zParent, zChild));
			glm::mat4 fixRotMat = glm::rotate(rotAngle, rotAxis);
			glm::vec3 yParent(fixRotMat * rParent * glm::vec4(0, 1, 0, 0));
			glm::vec3 yChild(rChild * glm::vec4(0, 1, 0, 0));
			float fixAngle = CalcRotationAngle(yParent, yChild);
			if (std::isfinite(fixAngle))
				rParent = glm::rotate(fixAngle, zParent) * rParent;
		}
	}

	CalcTransformations();
}

void snake::Snake::Draw(const Camera& camera) const {
	glm::mat4 trans = glm::translate(globalTrans, basePos);
	for (uint i = 0U; i < numLinks; ++i) {
		links[i]->Draw(camera, trans, (i + 1U < numLinks ? links[i + 1U] : 0));
		trans = links[i]->GetTrans();
	}
}

void snake::Snake::DrawForPicking(const Camera& camera) const {
	for(Link* link : links)
		link->DrawForPicking(camera);
}

void snake::Snake::CalcTransformations() {
	glm::mat4 trans = glm::translate(globalTrans, basePos);
	for (Link* link : links) {
		link->t1 = trans;
		glm::vec3 tipPos(link->GetTransScaled() * glm::vec4(0, 0, 1, 1));
		trans = glm::translate(tipPos);
	}

	//update sphere:
	uint midIndex = (numLinks - 1U) / 2;
	center = (GetBasePosition(midIndex) + GetTipPosition(midIndex)) * .5f;
}

void snake::Snake::MoveUp() {
	Link& head = GetHead();
	Link::AddRotationToHead(head, -movingAngle, glm::vec3(1, 0, 0));
}

void snake::Snake::MoveDown() {
	Link& head = GetHead();
	Link::AddRotationToHead(head, movingAngle, glm::vec3(1, 0, 0));
}

void snake::Snake::MoveRight() {
	Link& head = GetHead();
	Link::AddRotationToHead(head, -movingAngle, glm::vec3(0, 1, 0));
}

void snake::Snake::MoveLeft() {
	Link& head = GetHead();
	Link::AddRotationToHead(head, movingAngle, glm::vec3(0, 1, 0));
}

void snake::Snake::SetDestination(const GameObject& destination) {
	//TODO: implement set course bonus
}

glm::vec3 snake::Snake::GetTarget() {
	if (course.empty()) {
		uint headNum = numLinks - 1U;
		glm::vec3 headTipPos = GetTipPosition(headNum);
		glm::vec3 headBasePos = GetBasePosition(headNum);
		glm::vec3 dir = glm::normalize(headTipPos - headBasePos);
		return headTipPos + speed * dir;
	} else {
		//TODO: implement course movement bonus
	}
	return glm::vec3();
}

bool snake::Snake::CollidedWith(const Kdtree& collider,
	const glm::mat4& trans,
	const glm::vec3& center,
	float radius) const {
	float dist = glm::distance(this->center, center);
	if(dist > .5f * snakeLength + radius)
		return false;
	
	for (int i = numLinks - 1U; i >= 0; --i)
		if (links[i]->CollidedWith(collider, trans))
			return true;

	return false;
}

void snake::Snake::SetFrontView(glm::mat4& view) const {
	//glm::vec3 pos(0.0f, 30.0f, -20.0f);
	//glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
	//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	//view = glm::lookAt(pos, pos + forward, up) * glm::scale(glm::vec3(-1,1,-1)) * glm::rotate(-90.0f, glm::vec3(1, 0, 0));

	uint headNum = numLinks - 1U;
	glm::vec3 headTipPos = GetTipPosition(headNum);
	glm::vec3 up(links[headNum]->r * glm::vec4(0, 1, 0, 0));
	glm::vec3 forward(links[headNum]->r * glm::vec4(0, 0, 1, 0));
	headTipPos += .01f * forward;
	view = glm::lookAt(headTipPos, headTipPos + forward, up);
}

void snake::Snake::SetTopView(glm::mat4& view) const {
	uint headNum = numLinks - 1U;

	glm::vec3 tipPos = GetTipPosition(headNum);

	glm::vec3 headUp(links[headNum]->r * glm::vec4(0, 1, 0, 0));
	glm::vec3 headForward(links[headNum]->r * glm::vec4(0, 0, 1, 0));

	glm::vec3 eye = tipPos + 20.f * headUp - 30.f * headForward;
	glm::vec3 forward = glm::normalize(headForward - .7f * headUp);
	view = glm::lookAt(eye, eye + forward, headUp);
}
