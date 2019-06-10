#include "snake.hpp"

namespace snake {
	static bool CheckCollision(const Node* mine, const glm::mat4& myTrans,
		const Node* otherNode, const glm::mat4& otherTrans) {
		//should never happen....
		if (!mine || !otherNode)
			return false;

		if (mine->isLeaf() && otherNode->isLeaf()) {
			BBox myBox((mine->parent ? mine->parent->data : mine->data), myTrans);
			BBox otherBox = BBox((otherNode->parent ? otherNode->parent->data : otherNode->data), otherTrans);
			return BoxesCollide(myBox, otherBox);
		}

		if (mine->isLeaf())
			return CheckCollision(mine, myTrans, otherNode->left, otherTrans) ||
				CheckCollision(mine, myTrans, otherNode->right, otherTrans);

		if (otherNode->isLeaf())
			return CheckCollision(mine->left, myTrans, otherNode, otherTrans) ||
				CheckCollision(mine->right, myTrans, otherNode, otherTrans);

		if (BoxesCollide(BBox(mine->data, myTrans),
			BBox(otherNode->data, otherTrans))) {
			return CheckCollision(mine->left, myTrans, otherNode->left, otherTrans) ||
				CheckCollision(mine->left, myTrans, otherNode->right, otherTrans) ||
				CheckCollision(mine->right, myTrans, otherNode->left, otherTrans) ||
				CheckCollision(mine->right, myTrans, otherNode->right, otherTrans);
		}

		// No collision here. Return empty vector
		return false;
	}
}

bool snake::Link::CollidedWith(const Kdtree& otherCollider,
	const::glm::mat4& otherTrans) const {
	const Node* mine = mesh.collider.getRoot();
	glm::mat4 myTrans = GetTransScaled();
	const Node* otherNode = otherCollider.getRoot();
	return CheckCollision(mine, myTrans, otherNode, otherTrans);
}