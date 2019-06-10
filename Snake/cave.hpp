#pragma once

#include <glm/glm.hpp>

#include "infoCave.hpp"
#include "gameObject.hpp"
#include "shaders.hpp"

typedef unsigned int uint;

namespace snake {
	class Scene;
	class Cave: public GameObject {
		friend Scene;
		const InfoCave& infoCave;
		uint pickingColor;
		bool isOpen;
		glm::mat4 t;
		glm::mat4 scale;
		int openCounter;
		int timeSlice;
		float timeStamp;
		glm::vec3 position;

		static const glm::mat4 sDraw;
		static const glm::mat4 sCollide;

		inline glm::mat4 GetDrawingTrans() const {
			return t * scale * sDraw;
		}

		inline glm::mat4 GetCollisionTrans() const {
			return t * scale * sCollide;
		}

	public:
		Cave(const InfoCave& infoCave, uint pickingColor, int sceneSize);
		
		virtual ~Cave() {}
		
		void CalcActions() override;
		void CheckCollision(Snake& snake) override;
		
		void Draw(const Camera& camera) const override;
		void DrawForPicking(const Camera& camera) const override;

		inline void OpenCave() {
			isOpen = true;
		}
	};
}