#pragma once

#include <SFML\Graphics\ConvexShape.hpp>

// TODO: implement prickle
class Prickle : public sf::ConvexShape
{
public:
	Prickle();

	void tick();
};

