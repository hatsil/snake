#pragma once

#include <glm/glm.hpp>

namespace snake {
	class Snake;
	struct Camera {
		const glm::mat4& projection;
		glm::mat4 view;

		glm::mat4 ViewProjection() const {
			return projection * view;
		}

		Camera(const glm::mat4& projection): projection(projection), view(glm::mat4(1)) {}
		virtual ~Camera() {}
		
		virtual void FixView(const Snake& snake) = 0;
	};

	struct FrontCamera: public Camera {
		FrontCamera(const glm::mat4& projection): Camera(projection) {}
		virtual ~FrontCamera() {}
		
		void FixView(const Snake& snake) override;
	};

	struct TopCamera: public Camera {
		TopCamera(const glm::mat4& projection): Camera(projection) {}
		virtual ~TopCamera() {}
		
		void FixView(const Snake& snake) override;
	};
}




