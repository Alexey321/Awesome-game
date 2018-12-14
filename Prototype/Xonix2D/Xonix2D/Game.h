#pragma once

#include "GameField.h"
#include "HUD.h"

#include <vector>
#include <memory>

#include <SFML\Graphics.hpp>

class Game
{
public:
	Game();

	int run();

private:
	void actorsPostConstructor();

	void processEvent(const sf::Event& event);

	void update();
	void render();

	std::unique_ptr<sf::RenderWindow> window;

	const int width = 640;
	const int height = 440;

	const int frameRate = 60;

	std::shared_ptr<GameField> gameField;
	std::unique_ptr<HUD> hud;
};

