#pragma once

#include <glm/glm.hpp>

#include "infoMonkey.hpp"
#include "gameObject.hpp"

typedef unsigned int uint;

namespace snake {
	class Monkey : public GameObject {
		const InfoMonkey& infoMonkey;
		uint pickingColor;
		bool isAlive;
		glm::mat4 t;
		glm::mat4 r;
		glm::vec3 position;
		float radius;
		static const glm::mat4 s;

	public:
		Monkey(const InfoMonkey& infoMonkey, uint pickingColor, int sceneSize);

		virtual ~Monkey() {}

		void CalcActions() override;
		void CheckCollision(Snake& snake) override;

		void Draw(const Camera& camera) const override;
		void DrawForPicking(const Camera& camera) const override;

		inline glm::mat4 GetTrans() const {
			return t * r * s;
		}
	};
}