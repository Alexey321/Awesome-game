#include "stdafx.h"
#include "Heart.h"


Heart::Heart(const sf::Vector2f& pos)
{
	actorLocation = pos;
}

void Heart::tick()
{

}

const sf::Vector2f & Heart::getActorLocation() const
{
	return actorLocation;
}
