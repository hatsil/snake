#pragma once

#include <functional>

#include <GLFW/glfw3.h>

#include "infoes.hpp"
#include "gameObjects.hpp"
#include "cameras.hpp"
#include "shaders.hpp"

typedef unsigned int uint;

namespace snake {
	class Scene {
	private:
		uint pickingColor;
		PickingShader pickingShader;
		SnakeShader snakeShader;
		BoxShader boxShader;
		MonkeyShader monkeySahder;
		CaveShader caveShader;

		//meshes & shaders info:
		InfoBox* infoBox;
		InfoMonkey* infoMonkey;
		InfoSnake* infoSnake;
		InfoCave* infoCave;

		//game objects:
		std::vector<Box*> boxes;
		std::vector<Monkey*> monkeys;
		Snake* snake;
		Cave* cave;

		//cameras:
		glm::mat4 projection;
		int currentCamera;
		std::vector<Camera*> cameras;

		//points:
		uint maxPoints;
		uint currentPoints;

		void ForEach(const std::function<void(GameObject*)>& foo);
		void ForEach(const std::function<void(const GameObject*)>& foo) const;

	public:
		Scene(uint maxBoxes, uint maxMonkeys, uint snakeLength, uint maxPoints);
		Scene();
		virtual ~Scene();

		void CalcActions();
		void Draw() const;
		void DrawForPicking() const;
		void ApplyPickedColor(uint pickedColor);
		void SwitchCamera();
		void MoveUp();
		void MoveDown();
		void MoveRight();
		void MoveLeft();
		void Resize(int width, int height);
	};
}