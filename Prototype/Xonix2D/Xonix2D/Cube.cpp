#include "stdafx.h"
#include "Cube.h"


Cube::Cube(const sf::Vector2f& pos, const int width, const int height)
{
	actorLocation = pos;
	setFillColor({ 153, 153, 153, 255 });
	setPosition(pos);
	setSize(sf::Vector2f({ static_cast<float>(width), static_cast<float>(height) }));
}


void Cube::tick()
{
}

void Cube::beginPlay()
{

}

void Cube::setFlag(const int flag)
{
	this->flag = flag;
}

int Cube::getFlag() const
{
	return flag;
}

void Cube::setActorHiddenInGame(const bool bHiden)
{
	this->bHiden = bHiden ? 0 : 255;
	setFillColor({ 153, 153, 153, this->bHiden });
}

void Cube::setActorEnableCollision(const bool bEnableCollision)
{
	this->bEnableCollision = bEnableCollision;
}

bool Cube::getActorEnableCollision() const
{
	return bEnableCollision;
}

const sf::Vector2f& Cube::getActorLocation() const
{
	return actorLocation;
}

void Cube::addTag(const std::string & tag)
{
	tags.push_back(tag);
}

void Cube::removeSingleTag(const std::string & tag)
{
	for (int i = 0; i < tags.size(); ++i)
	{
		if (tags[i] == tag)
		{
			tags.erase(tags.begin() + i);
			break;
		}
	}
}
