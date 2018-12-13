#pragma once

#include <SFML\Graphics\Text.hpp>

class HUD : public sf::Text
{
public:
	HUD(const int windowWidth, const int windowHeight);

	void tick();

	int getHeight() const;

private:
	int height = 50;
};

