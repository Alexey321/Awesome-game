#include "stdafx.h"

#include "GameField.h"
#include "Player.h"
#include "Heart.h"
#include "Prickle.h"
#include "CommonBall.h"
#include "Cube.h"

#include <iostream>
#include <time.h>
#include <stdlib.h>

GameField::GameField(const int width, const int height)
{
	initStyles(width, height);
}

void GameField::tick()
{

}

void GameField::beginPlay()
{
	fillUpWithCubes();
	initFlags();
	player = std::make_shared<Player>(borderWidth, borderWidth, sf::Vector2i(cubes.size() / 2 - 1, cubes[0].size() - 1));
	spawnInitialBalls();
}

void GameField::setGameField(std::shared_ptr<GameField> gameField)
{
	this->gameField = gameField;
}

void GameField::initStyles(const int width, const int height)
{
	this->width = width;
	this->height = height;
	setOutlineColor({ 0, 153, 0, 255 });
	setOutlineThickness(borderWidth);
	setFillColor({ 0, 0, 0, 255 });
	setPosition(borderWidth, borderWidth);
	setSize(sf::Vector2f(width - 2 * borderWidth, height - 2 * borderWidth));
}

void GameField::fillUpWithCubes()
{
	float i = 0;
	const float il = width;

	cubes.reserve(il / borderWidth);
	for (; i < il; i += borderWidth)
	{
		float j = 0;
		const float jl = height;
		std::vector<std::shared_ptr<Cube>> temp;
		temp.reserve(jl / borderWidth);
		for (; j < jl; j += borderWidth)
		{
			sf::Vector2f spawnLocation = { i, j };
			std::shared_ptr<Cube> cube = std::make_shared<Cube>(spawnLocation, borderWidth, borderWidth);
			cube->beginPlay();
			cube->setActorHiddenInGame(true);
			cube->setActorEnableCollision(false);
			temp.push_back(cube);
		}
		cubes.push_back(temp);
	}
}

void GameField::spawnInitialBalls()
{
	srand(time(0));
	for (int i = 0; i < ballsCount; i++)
	{
		
		int y = std::rand() % cubes.size();
		int x = std::rand() % cubes[y].size();
		
		std::shared_ptr<CommonBall> commonBall = spawnBall(y, x);
		if (commonBall)
		{
			commonBall->setGameField(gameField);
			commonBall->beginPlay();
			commonBalls.push_back(commonBall);
		}
		else
		{
			--i;
			continue;
		}
	}
}

std::shared_ptr<CommonBall> GameField::spawnBall(const int y, const int x)
{
	if (cubes[y][x]->getFlag() == 0) // if nothing blocks us
	{ // then spawn
		return std::make_shared<CommonBall>(cubes[y][x]->getPosition(), borderWidth / 2);
	}
	else
		return nullptr;
}

void GameField::initFlags()
{
	for(const auto& cubesArr : cubes)
		for (const auto& cube : cubesArr)
			cube->setFlag(0);

	for (int e = 0; e < cubes.size(); e++)
	{
		for (int e1 = 0; e1 < cubes[e].size(); e1++)
		{ // TODO: optimize
			if (
				e == 0 || e == cubes.size() - 1 || e1 == 0 || e1 == cubes[e].size() - 1
			)
			{
				cubes[e][e1]->setFlag(2);
			}
		}
	}
}

std::shared_ptr<Player> GameField::getPlayer() const
{
	return player;
}

std::vector<std::shared_ptr<CommonBall>>& GameField::getCommonBalls()
{
	return commonBalls;
}

std::vector<std::shared_ptr<Heart>>& GameField::getHearts()
{
	return hearts;
}

std::vector<std::shared_ptr<Prickle>>& GameField::getPrickles()
{
	return prickles;
}

std::vector<std::vector<std::shared_ptr<Cube>>>& GameField::getCubes()
{
	return cubes;
}

std::vector<sf::Vector2i>& GameField::getTail()
{
	return tail;
}

int GameField::getBallsCount() const
{
	return ballsCount;
}

int GameField::getWidth() const
{
	return width;
}

int GameField::getHeight() const
{
	return height;
}

int GameField::getBorderWidth() const
{
	return borderWidth;
}

float GameField::getPercentageOfFieldToWin() const
{
	return percentageOfFieldToWin;
}

void GameField::setPercentageOfConquered(const float percentageOfConquered)
{
	this->percentageOfConquered = percentageOfConquered;
}
