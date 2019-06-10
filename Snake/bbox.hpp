#pragma once

#include <glm/glm.hpp>

namespace snake {
	static glm::vec3 zeroVec(0);
	struct BBox {
		glm::vec3 position, xAxis, yAxis, zAxis;

		// Half of each value w,h,d
		float width, height, depth;		

		inline BBox(const glm::vec3& position,
			const glm::vec3& xAxis,
			const glm::vec3& yAxis,
			const glm::vec3& zAxis,
			float width, float height, float depth) :
			depth(depth), height(height), position(position),
			width(width), xAxis(xAxis), yAxis(yAxis), zAxis(zAxis) {}

		inline BBox() : BBox(zeroVec, zeroVec, zeroVec, zeroVec, 0, 0, 0) {}

		inline BBox(const BBox& other):
			BBox(other.position, other.xAxis, other.yAxis,
				other.zAxis, other.width, other.height, other.depth) {}

		inline BBox(const BBox& other, const glm::vec3& offsetPos): BBox(other) {
			position += offsetPos;
		}

		inline BBox(const BBox& other, const glm::mat4& transformation): BBox(other) {
			// Get position after translation
			position = glm::vec3(transformation * glm::vec4(position, 1));

			// Get axes of object compared to world
			xAxis = glm::vec3(transformation * glm::vec4(1, 0, 0, 0));
			yAxis = glm::vec3(transformation * glm::vec4(0, 1, 0, 0));
			zAxis = glm::vec3(transformation * glm::vec4(0, 0, 1, 0));

			width = glm::length(xAxis);
			height = glm::length(yAxis);
			depth = glm::length(zAxis);

			xAxis /= width;
			yAxis /= height;
			zAxis /= depth;
		}

		inline BBox(const BBox& other, const glm::vec3& offsetPos,
			const glm::vec3& xAxis, const glm::vec3& yAxis, const glm::vec3& zAxis): BBox(other) {
			this->position += offsetPos;
			this->xAxis = xAxis;
			this->yAxis = yAxis;
			this->zAxis = zAxis;
		}

		// Lexicographic ordering 
		inline bool SmallerThanByAxis(int axis, const BBox& other) const {
			return position[axis] < other.position[axis];
		}

		inline bool operator==(const BBox& other) const {
			return depth == other.depth &&
				height == other.height &&
				position == other.position &&
				width == other.width &&
				xAxis == other.xAxis &&
				yAxis == other.yAxis &&
				zAxis == other.zAxis;
		}

		inline bool operator!=(const BBox& other) const {
			return !operator==(other);
		}

		inline BBox& operator=(const BBox& toCopy) {
			if(this != &toCopy) // self-assignment check expected
				copy(toCopy);
			
			return *this;
		}

	private:
		inline void copy(const BBox& other) {
			this->depth = other.depth;
			this->height = other.height;
			this->position = other.position;
			this->width = other.width;
			this->xAxis = other.xAxis;
			this->yAxis = other.yAxis;
			this->zAxis = other.zAxis;
		}
	};

	//BBox NullBox = BBox(glm::vec3(-1.0f), glm::vec3(-1.0f), glm::vec3(-1.0f), glm::vec3(-1.0f), -1.0f, -1.0f, -1.0f);

	// True if boxes collide
	bool BoxesCollide(const BBox& a, const BBox& b);

	// Returns 2 boxes (1 of each KDTree) if they collide (return the boxes that collide)
	// Or return empty vector if there's no collision
	//vector<BBox> ObjectsCollide(KDTree& a, KDTree& b, vec3& wolrdPosA, vec3& wolrdPosB);

	//void DrawBox(BBox& toDraw, glm::vec3 color);

	bool VertexTriangleIntersect(const glm::vec3& vertex, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
}