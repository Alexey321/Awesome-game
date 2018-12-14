#include "stdafx.h"

#include "CommonBall.h"
#include "GameField.h"

#include <time.h>
#include <stdlib.h>
#include <math.h>

CommonBall::CommonBall(const sf::Vector2f& pos, const float radius)
{
	actorLocation = pos;
	setFillColor({ 255, 255, 255, 255 });
	setPosition(pos);
	setRadius(radius);
}

void CommonBall::tick()
{
	// TODO: animate ball
	// Warning: finding cell with ball considering as the ball is right in the same position as cube   label: akjsh6jh
	checkCollision();
}

void CommonBall::checkCollision()
{
	switch (isCollision())
	{ // TODO: replace with enum class
	case 1: // player
		break;
	case 2: // heart
		break;
	case 3: // prickle
		break;
	case 4: // border
		break;
	case 5: // cube
		break;
	case 6: // cube marked as player tail
		break;
	default:
		break;
	}
}

int CommonBall::isCollision()
{
	// TODO: check collison with another mesh objects
	// 1 -- collided with player
	// 2 -- heart
	// 3 -- prickle
	// 4 -- border
	// 5 -- cube
	// 6 -- cube marked as player tail
	return 0;
}

void CommonBall::beginPlay()
{
	/*
	float angle = std::rand() * 361.0f;
	int x = velocity * std::sin(angle);
	int y = velocity * std::cos(angle);
	//addImpulse(sf::Vector2f(x, y)); // add initial impulse to the ball
	*/
}

void CommonBall::setGameField(std::shared_ptr<GameField> gameField)
{
	this->gameField = gameField;
}

const sf::Vector2f& CommonBall::getActorLocation() const
{
	return actorLocation;
}

