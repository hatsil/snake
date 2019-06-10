#pragma once

namespace snake {
	class Snake;
	struct Camera;
	class GameObject {
	public:
		GameObject() {}
		virtual ~GameObject() {}
		
		virtual void CalcActions() = 0;
		virtual void CheckCollision(Snake& snake) = 0;
		virtual void Draw(const Camera& camera) const = 0;
		virtual void DrawForPicking(const Camera& camera) const {}
	};
}