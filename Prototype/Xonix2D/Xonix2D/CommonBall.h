#pragma once

#include "GameField.h"

#include <SFML\Graphics\CircleShape.hpp>

class CommonBall : public sf::CircleShape
{
public:
	CommonBall(const sf::Vector2f& pos, const float radius);

	void tick();

	void beginPlay();

	const sf::Vector2f& getActorLocation() const;

	void setGameField(std::shared_ptr<GameField> gameField);

	void checkCollision();
	// TODO: replace with enum class
	int isCollision();

private:
	sf::Vector2f actorLocation;

	std::shared_ptr<GameField> gameField;

	float velocity = 20;
};

