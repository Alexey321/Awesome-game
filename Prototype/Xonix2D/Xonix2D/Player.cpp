#include "stdafx.h"

#include "Player.h"
#include "GameField.h"
#include "Cube.h"
#include "CommonBall.h"
#include "Heart.h"

#include <functional>
#include <time.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>

#define OUT

Player::Player(const int width, const int height, const sf::Vector2i& pos)
{
	this->pos = { pos.y, pos.x }; // reverce coordinates for simplicity
	startPos = this->pos;
	setFillColor({ 255, 255, 255, 255 });
	setSize(sf::Vector2f(width, height));
}

void Player::beginPlay()
{
	amountOfCells = (gameField->getCubes().size() - 1) * (gameField->getCubes()[0].size() - 1);

	setPos(startPos);
}

void Player::setGameField(std::shared_ptr<GameField> gameField)
{
	this->gameField = gameField;
}

void Player::checkLives()
{
	if (lives <= 0) // if we have lost all our lives
	{ // then go to level 1
		freeGameField();
		resetLevelStateToDefault();
		resetCommonBalls();
	}
}

void Player::updatePosition()
{
	if (currDirection == "Up")
	{ // TODO: replace with enum class
		goUp();
	}
	else if (currDirection == "Down")
	{
		goDown();
	}
	else if (currDirection == "Right")
	{
		goRight();
	}
	else if (currDirection == "Left")
	{
		goLeft();
	}
}

void Player::goUp()
{
	++pos.y;
	if (pos.y > gameField->getCubes().size() - 1)
	{
		pos.y = gameField->getCubes().size() - 1;
	}
	setPos(pos);
}

void Player::goDown()
{
	--pos.y;
	if (pos.y < 0)
	{
		pos.y = 0;
	}
	setPos(pos);
}

void Player::goLeft()
{
	--pos.x;
	if (pos.x < 0)
	{
		pos.x = 0;
	}
	setPos(pos);
}

void Player::goRight()
{
	++pos.x;
	if (pos.x > gameField->getCubes()[pos.y].size() - 1)
	{
		pos.x = gameField->getCubes()[pos.y].size() - 1;
	}
	setPos(pos);
}

void Player::resetCommonBalls()
{
	// TODO: optimize all for conditions
	for (int i = 0; i < gameField->getCommonBalls().size() - gameField->getBallsCount(); ++i) // delete superfluous commonBalls
	{
		gameField->getCommonBalls().erase(gameField->getCommonBalls().begin() + i);
	}
}

void Player::tick()
{
	checkLives();
	static float counter = 0;
	if (counter > speed)
	{
		counter = 0;
		updatePosition();
	}
	else
	{
		counter += 0.1;
	}
}

void Player::checkHeartHit(const sf::Vector2f& newLocation)
{
	for (int i = 0; i < gameField->getHearts().size(); ++i) // Check if we hit Heart 
	{ // TODO: Optimize this shit
		if (newLocation.x == gameField->getHearts()[i]->getActorLocation().x
			&&
			newLocation.y == gameField->getHearts()[i]->getActorLocation().y
			) // then add live, destroy Heart, SpawnSprikes
		{
			// TODO: implement particle effect here
			//gameField->getHearts()[i]->SpawnSprikes();
			gameField->getHearts().erase(gameField->getHearts().begin() + i);

			++lives;
		}
	}
}

bool Player::isPosValid()
{
	return pos.y >= 0 && pos.y < gameField->getCubes().size()
		&&
		pos.x >= 0 && pos.x < gameField->getCubes()[pos.y].size();
}

bool Player::isMovingBackward()
{
	return !(pos.y == 0 || pos.y == gameField->getCubes().size() - 1 || pos.x == 0 || pos.x == gameField->getCubes()[pos.y].size() - 1);
}

void Player::setPos(const sf::Vector2i& pos) // TODO: implement smooth movement
{
	// TODO: implement event based system for examble as boost library provides
	//	i.e. onMoveBack, onHide, etc.
	if (isPosValid()) // just in case, we'll check
	{
		std::shared_ptr<Cube> a = gameField->getCubes()[pos.y][pos.x];
		sf::Vector2f newLocation = a->getActorLocation();

		setPosition(newLocation);

		checkHeartHit(newLocation);

		if (isMovingBackward())
		{ // on move back
			if (gameField->getTail().size() - 2 >= 0 && gameField->getTail().size() - 2 < gameField->getTail().size()
				&&
				gameField->getTail()[gameField->getTail().size() - 2] == pos
			) // hide
			{
				//sf::Vector2i posToHide = gameField->getTail().Pop();
				//std::shared_ptr<Cube> b = gameField->getCubes()[posToHide.y][posToHide.x];
				//b->setActorHiddenInGame(true);
				//b->setActorEnableCollision(false);
				//gameField->getCubes()[posToHide.y][posToHide.x]->setFlag(0);
			}
			else // on move ahead
			{
				if (gameField->getCubes()[pos.y][pos.x]->getFlag() == 3) // if the field is already captured
				{
					if (gameField->getTail().size()) // if we step on the capture area after drawing tail
					{
						currDirection = "None"; // stop moving
						Conquer();
					}
					else
					{
						//
					}
				}
				else // step forward
				{
					a->setActorHiddenInGame(false);
					a->setActorEnableCollision(true);
					gameField->getTail().push_back(pos);
					gameField->getCubes()[pos.y][pos.x]->addTag("tail");
					gameField->getCubes()[pos.y][pos.x]->setFlag(1);
				}
			}
		}
		else // on border
		{
			if (gameField->getTail().size() == 1) // step back on border
			{
				sf::Vector2i posToHide = gameField->getTail().back();
				gameField->getTail().pop_back();
				std::shared_ptr<Cube> b = gameField->getCubes()[posToHide.y][posToHide.x];
				b->setActorHiddenInGame(true);
				b->setActorEnableCollision(false);
				gameField->getCubes()[posToHide.y][posToHide.x]->setFlag(0);
			}
			else if (gameField->getTail().size() > 1) // conquer
			{
				Conquer();
			}
		}
	}
	
}

///** Begin: movement events */
/*
void Player::onMoveBack(const sf::Vector2i& pos)
{

}
void Player::onMoveForward(const sf::Vector2i& pos)
{

}
*/
////** End: movement events */

void Player::tryToConquer()
{
	for (int i = 0; i < gameField->getCommonBalls().size(); ++i)
	{
		const sf::Vector2f& CommonBallLoc = gameField->getCommonBalls()[i]->getActorLocation();

		[this, &CommonBallLoc]()
		{

			for (int j = 0; j < gameField->getCubes().size(); ++j)
			{

				for (int m = 0; m < gameField->getCubes()[j].size(); ++m)
				{

					if ( // label: akjsh6jh
						CommonBallLoc == gameField->getCubes()[j][m]->getActorLocation()
						) // find cube under the ball
					{

						_Conquer(j, m);

						return;
					}
				}
			}
		}();
	}
}

void Player::processConqueredRegion()
{
	bool hidden = !gameField->getCubes()[res[0][0]][res[0][1]]->getActorEnableCollision();

	for (int y = 1; y < gameField->getCubes().size() - 1; ++y)
	{
		for (int x = 1; x < gameField->getCubes()[y].size() - 1; ++x)
		{
			gameField->getCubes()[y][x]->setActorHiddenInGame(false);
			gameField->getCubes()[y][x]->setActorEnableCollision(true);
			gameField->getCubes()[y][x]->setFlag(3);
		}
	}

	for (int eee = 0; eee < res.size(); ++eee)
	{
		gameField->getCubes()[res[eee][0]][res[eee][1]]->setActorHiddenInGame(true);
		gameField->getCubes()[res[eee][0]][res[eee][1]]->setActorEnableCollision(false);
		gameField->getCubes()[res[eee][0]][res[eee][1]]->setFlag(0);
	}

	round.clear();

	if (hidden)
	{
		gameField->getCubes()[res[0][0]][res[0][1]]->setActorHiddenInGame(true);
		gameField->getCubes()[res[0][0]][res[0][1]]->setActorEnableCollision(false);
	}
	else
	{
		gameField->getCubes()[res[0][0]][res[0][1]]->setActorHiddenInGame(false);
		gameField->getCubes()[res[0][0]][res[0][1]]->setActorEnableCollision(true);

		++amountOfConqueredCells;
	}
}

void Player::processConqueredCells()
{
	for (int y = 1; y < gameField->getCubes().size() - 1; ++y) // count amount of captured cells
	{	// TODO: Optimize this shit (// remove repeatings from the res array)
		for (int x = 1; x < gameField->getCubes()[y].size() - 1; ++x)
		{
			if (gameField->getCubes()[y][x]->getFlag() == 3)
			{
				++amountOfConqueredCells;

				for (int i = 0; i < gameField->getHearts().size(); ++i) // Check if we Conquered Heart 
				{ // TODO: Optimize this shit!!!
					if (gameField->getCubes()[y][x]->getActorLocation().x == gameField->getHearts()[i]->getActorLocation().x
						&&
						gameField->getCubes()[y][x]->getActorLocation().y == gameField->getHearts()[i]->getActorLocation().y
						) // then add live, delete Heart, SpawnSprikes
					{
						// TODO: implement particle effect here
						//gameField->getHearts()[i]->SpawnSprikes();
						gameField->getHearts().erase(gameField->getHearts().begin() + i);
						++lives;
					}
				}
			}

		}
	}
}

void Player::freeGameField()
{
	for (int y = 1; y < gameField->getCubes().size() - 1; ++y) // free gameField
	{ // TODO: Endure into a separate function p1
		for (int x = 1; x < gameField->getCubes()[y].size() - 1; ++x)
		{
			gameField->getCubes()[y][x]->setActorHiddenInGame(true);
			gameField->getCubes()[y][x]->setActorEnableCollision(false);

			gameField->getCubes()[y][x]->setFlag(0);
		}
	}
}

void Player::resetLevelStateToDefault()
{
	pos = startPos; // reset poition and counters
	std::shared_ptr<Cube> a = gameField->getCubes()[pos.y][pos.x];
	sf::Vector2f newLocation = a->getActorLocation();
	setPosition(newLocation);
	currDirection = "None";
	amountOfConqueredCells = 0;

	++level; // next level
}

void Player::enharderGame()
{
	// TODO: Enharder game
}

void Player::spawnOneBall()
{
	srand(time(0));
	std::function<void(void)> spawnBallFun = [this, &spawnBallFun]()
	{ // add one more ball
		int y = std::rand() % gameField->getCubes().size();
		int x = std::rand() % gameField->getCubes()[y].size();
		std::shared_ptr<CommonBall> commonBall = gameField->spawnBall(y, x);

		if (commonBall)
		{
			commonBall->setGameField(gameField);
			commonBall->beginPlay();
			gameField->getCommonBalls().push_back(commonBall);

			//++gameField->ballsCount; // don't realy need this
		}
		else // if failed to spawn
		{ // then try again
			spawnBallFun();
		}
	};
	spawnBallFun(); // TODO: optimize
}

void Player::checkForNextLevel()
{
	if (amountOfConqueredCells / static_cast<float>(amountOfCells) >= gameField->getPercentageOfFieldToWin()) // if PercentageOfFieldToWin of the field captured
	{
		freeGameField();
		resetLevelStateToDefault();
		enharderGame();
		spawnOneBall();
	}
}

void Player::unmarkTailMarkedCells()
{
	for (int i = 0; i < gameField->getTail().size(); ++i)
	{
		gameField->getCubes()[gameField->getTail()[i].y][gameField->getTail()[i].x]->removeSingleTag("tail");
	}
}

void Player::releaceAuxiliaryVariablesForConqueredRegionCalculation()
{
	gameField->setPercentageOfConquered(amountOfConqueredCells / static_cast<float>(amountOfCells) * 100);
	amountOfConqueredCells = 0;
	gameField->getTail().clear();
}

void Player::Conquer()
{
	tryToConquer();
	if (res.size()) // if we can conquire
	{
		processConqueredRegion();
		res.clear();
		processConqueredCells();
		checkForNextLevel();
		unmarkTailMarkedCells();
	}
	releaceAuxiliaryVariablesForConqueredRegionCalculation();
}

void Player::_Conquer(int y, int x)
{
	std::vector<std::vector<int>> r = {
		{ y - 1, x - 1 },
		{ y	   , x - 1 },
		{ y + 1, x - 1 },
		{ y + 1, x },
		{ y + 1, x + 1 },
		{ y    , x + 1 },
		{ y - 1, x + 1 },
		{ y - 1, x }
	};


	//Flags[y][x] = 0;
	// 0 -- empty
	// 1 -- tail
	// 2 -- border cube
	// 3 -- filled

	if (std::find(round.begin(), round.end(), std::vector<int>({ y, x })) != round.end())
	{
		return;
	}
	if (y == 0 || y == gameField->getCubes().size() - 1 || x == 0 || x == gameField->getCubes()[y].size() - 1)
	{
		round.push_back({ y, x });
		return;
	}

	round.push_back({ y, x });
	res.push_back({ y, x });

	for (int i = 0; i < r.size(); i++)
	{
		std::vector<int> p = r[i];

		if (
			p[0] >= 0 && p[0] < gameField->getCubes().size()
			&&
			p[1] >= 0 && p[1] < gameField->getCubes()[p[0]].size()
			&&
			gameField->getCubes()[p[0]][p[1]]->getFlag() == 0
		)
		{
			_Conquer(p[0], p[1]);
		}
	}
}

// TODO: correct messy coordinates
void Player::up()
{
	currDirection = "Left";
}

void Player::down()
{
	currDirection = "Right";
}

void Player::left()
{
	currDirection = "Down";
}

void Player::right()
{
	currDirection = "Up";
}
