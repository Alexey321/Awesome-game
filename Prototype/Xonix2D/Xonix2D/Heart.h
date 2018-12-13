#pragma once

#include <SFML\Graphics\ConvexShape.hpp>

// TODO: implement Heart
class Heart : public sf::ConvexShape
{
public:
	Heart(const sf::Vector2f& pos);

	void tick();

	const sf::Vector2f& getActorLocation() const;
private:
	sf::Vector2f actorLocation;

};

