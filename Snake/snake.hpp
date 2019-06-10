#pragma once

#include <vector>
#include <list>
#include <cmath>
#include <iostream>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "kdtree.hpp"

#include "gameObject.hpp"
#include "infoSnake.hpp"
#include "cameras.hpp"

typedef unsigned int uint;

namespace snake {
	inline std::ostream& operator<<(std::ostream& out, const glm::vec3& v) {
		return out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	}

	inline std::ostream& operator<<(std::ostream& out, const glm::vec4& v) {
		return out << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w <<")";
	}

	class Snake;
	class Link {
	public:
		Link(const SnakeMesh& mesh, uint pickingColor, const glm::mat4& scale);

		void Draw(const Camera& camera, const glm::mat4& parentTrans, Link* nextLink) const;
		void DrawForPicking(const Camera& camera) const;

		inline glm::mat4 GetTrans() const {
			return t1 * r * t2;
		}

		inline glm::mat4 GetTransScaled() const {
			return GetTrans() * s;
		}

		virtual ~Link() {}

	private:
		friend Snake;
		
		//for movment by the arrwos
		static void AddRotationToHead(Link& head, float angle, const glm::vec3& rotAxis);

		//collisionDetection.cpp
		bool CollidedWith(const Kdtree& otherCollider,
			const::glm::mat4& otherTrans) const;
		
		uint pickingColor;
		const SnakeMesh& mesh;
		glm::mat4 t1;
		glm::mat4 t2;
		glm::mat4 r;
		glm::mat4 s;
	};

	class Box;
	class Snake: public GameObject {
	public:
		Snake(const InfoSnake& infoSnake,
			uint snakeLength,
			const std::vector<Box*>& boxes,
			uint& pickingColor);
		
		virtual ~Snake() {
			for(Link* link : links)
				delete link;
		}

		//MARK: GameObject functions
		void CalcActions() override;
		void CheckCollision(Snake& snake) override {}
		void Draw(const Camera& camera) const override;
		void DrawForPicking(const Camera& camera) const override;

		inline void Deactivate() {
			speed = 0;
		}

		//MARK: Snake functions
		void MoveUp();
		void MoveDown();
		void MoveRight();
		void MoveLeft();
		bool CollidedWith(const Kdtree& collider,
			const glm::mat4& trans,
			const glm::vec3& center,
			float radius) const;
		void SetDestination(const GameObject& destination);
		void SetFrontView(glm::mat4& view) const;
		void SetTopView(glm::mat4& view) const;

		inline bool IsEnteredCave() const {
			return isEnteredCave;
		}

		inline bool IsDead() const {
			return isDead;
		}

		inline void SetIsDead() {
			Deactivate();
			isDead = true;
		}

		inline void SetIsEnteredCave() {
			Deactivate();
			isEnteredCave = true;
		}

		inline uint CollectPoints() {
			uint ans = points;
			points = 0U;
			return ans;
		}

		inline void AddPoint() {
			++points;
		}

	private:

		inline Link& GetHead() {
			return *links[numLinks - 1U];
		}

		inline const Link& GetHead() const {
			return *links[numLinks - 1U];
		}

		void CalcTransformations();

		inline glm::vec3 GetTipPosition(uint linkNum) const { 
			const Link& link = *links[linkNum];
			return glm::vec3(link.GetTransScaled() * glm::vec4(0,0,1,1));
		}

		inline glm::vec3 GetBasePosition(uint linkNum) const {
			const Link& link = *links[linkNum];
			return glm::vec3(link.GetTransScaled() * glm::vec4(0, 0, -1, 1));
		}

		glm::vec3 GetTarget();

		static const float movingAngle;
		static const float defaultSpeed;
		const std::vector<Box*>& boxes;
		std::vector<Link*> links;

		uint numLinks;
		uint points;
		bool isDead;
		bool isEnteredCave;
		float speed;
		glm::vec3 basePos;
		std::list<glm::vec3> course;
		glm::mat4 scale;
		glm::mat4 globalTrans;
		float linkLength;
		float snakeLength;
		glm::vec3 center;
	};
}