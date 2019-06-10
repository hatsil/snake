#include "cameras.hpp"
#include "snake.hpp"


//class Snake;
//struct Camera {
//	const glm::mat4& projection;
//	glm::mat4 view;
//
//	glm::mat4 ViewProjection() const {
//		return projection * view;
//	}
//
//	Camera(const glm::mat4& projection) : projection(projection), view(glm::mat4(1)) {}
//	virtual ~Camera() {}
//
//	virtual void FixView(Snake& snake) = 0;
//};



//struct FrontCamera : public Camera {
//	FrontCamera(const glm::mat4& projection) : Camera(projection) {}
//	virtual ~FrontCamera() {}
//

void snake::FrontCamera::FixView(const Snake& snake) {
	snake.SetFrontView(view);
}

void snake::TopCamera::FixView(const Snake& snake) {
	snake.SetTopView(view);
}
//};

//struct TopCamera : public Camera {
//	TopCamera(const glm::mat4& projection) : Camera(projection) {}
//	virtual ~TopCamera() {}
//
//	void FixView(Snake& snake) override;
//};