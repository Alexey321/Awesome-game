#pragma once


#include <memory>
#include <string>

#include <SFML\Graphics\RectangleShape.hpp>

class GameField;

class Player : public sf::RectangleShape
{
public:
	Player(const int width, const int height, const sf::Vector2i& pos);

	void tick();

	void setPos(const sf::Vector2i& pos);

	void up();
	void down();
	void left();
	void right();

	void beginPlay();

	void setGameField(std::shared_ptr<GameField> gameField);

private:
	void Conquer();
	void _Conquer(int y, int x);
	void checkLives();
	void updatePosition();

	void goUp();
	void goDown();
	void goLeft();
	void goRight();

	void resetCommonBalls();
	void checkHeartHit(const sf::Vector2f& newLocation);
	bool isPosValid();
	bool isMovingBackward();
	///** Begin: movement events */
	/*
	void onMoveBackward(const sf::Vector2i & pos);
	void onMoveForward(const sf::Vector2i & pos);
	*/
	////** End: movement events */
	void tryToConquer();
	void processConqueredRegion();
	void processConqueredCells();
	void checkForNextLevel();
	void unmarkTailMarkedCells();
	void releaceAuxiliaryVariablesForConqueredRegionCalculation();
	void freeGameField();
	void resetLevelStateToDefault();
	void enharderGame();
	void spawnOneBall();

	std::shared_ptr<GameField> gameField;

	int lives = 10;

	sf::Vector2i pos;
	sf::Vector2i startPos;

	std::string currDirection = "None"; // TODO: replace with enum class

	int amountOfConqueredCells = 0;

	int level = 1;

	float speed = 0.2f; // TODO : adjust speed

	int amountOfCells = 0;

	/// auxiliary arrays for detection a conqure region 
	std::vector<std::vector<int>> res;
	std::vector<std::vector<int>> round;
};

