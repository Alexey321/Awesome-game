#pragma once

#include "Player.h"
#include "Heart.h"
#include "Prickle.h"
#include "CommonBall.h"
#include "Cube.h"

#include <vector>
#include <memory>

#include <SFML\Graphics\RectangleShape.hpp>

class Player;
class CommonBall;

class GameField : public sf::RectangleShape
{
public:
	GameField(const int width, const int height);

	void tick();

	std::shared_ptr<Player> getPlayer() const;
	std::vector<std::shared_ptr<CommonBall>>& getCommonBalls();
	std::vector<std::shared_ptr<Heart>>& getHearts();
	std::vector<std::shared_ptr<Prickle>>& getPrickles();
	std::vector<std::vector<std::shared_ptr<Cube>>>& getCubes();
	std::vector<sf::Vector2i>& getTail();

	int getBallsCount() const;

	int getWidth() const;
	int getHeight() const;
	int getBorderWidth() const;

	float getPercentageOfFieldToWin() const;

	void setPercentageOfConquered(const float percentageOfConquered);

	std::shared_ptr<CommonBall> spawnBall(const int y, const int x);

	void beginPlay();

	void setGameField(std::shared_ptr<GameField> gameField);

private:
	void initStyles(const int width, const int height);
	void fillUpWithCubes();
	void initFlags();
	void spawnInitialBalls();

	int ballsCount = 2;
	float BallVelocity = 0.5;

	int width;
	int height;
	int borderWidth = 20;

	std::shared_ptr<Player> player;
	std::vector<std::shared_ptr<CommonBall>> commonBalls;
	std::vector<std::shared_ptr<Heart>> hearts;
	std::vector<std::shared_ptr<Prickle>> prickles;

	std::vector<std::vector<std::shared_ptr<Cube>>> cubes;

	std::vector<sf::Vector2i> tail;

	float percentageOfFieldToWin = 0.55; // [0 - 1]

	float percentageOfConquered = 0; // [0 - 100]

	std::shared_ptr<GameField> gameField;
};

