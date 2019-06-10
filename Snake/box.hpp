#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "gameObject.hpp"
#include "infoBox.hpp"
#include "random.hpp"

typedef unsigned int uint;

namespace snake {
	class Box: public GameObject {
		static const float speed;
		BoxMesh* mesh; //don't delete this!!!
		
		uint pickingColor;
		glm::mat4 r; //rotation
		glm::mat4 s; //scale
		glm::vec3 position;
		glm::vec3 moveTo;
		glm::vec3 dir;
		int sceneSize;
		float radius;

		inline glm::mat4 GetTrans() const {
			return glm::translate(position) * r * s;
		}

		inline float GetRandomCoordinate() const {
			return (random() > .5f ? random() : -random()) * .5f * sceneSize;
		}

		inline glm::vec3 GetRandomLocation() const {
			return glm::vec3(GetRandomCoordinate(),
				GetRandomCoordinate(),
				GetRandomCoordinate());
		}

		void CalcDir();
		
	public:
		Box(const InfoBox& infoBox, uint pickingColor, int sceneSize);
		virtual ~Box() {}
		
		void CalcActions() override;
		void CheckCollision(Snake& snake) override;
		void Draw(const Camera& camera) const override;
	};
}