#include "stdafx.h"

#include "Game.h"
#include "GameField.h"
#include "Player.h"
#include "Heart.h"
#include "Prickle.h"
#include "CommonBall.h"
#include "Cube.h"
#include "HUD.h"

#include <vector>
#include <memory>
#include <iostream>

#include <SFML\Graphics.hpp>

Game::Game()
{
	hud = std::make_unique<HUD>(width, height);
	window = std::make_unique<sf::RenderWindow>(sf::VideoMode(640, 440 + hud->getHeight()), "Xonix");
	gameField = std::make_shared<GameField>(width, height);
	window->setFramerateLimit(frameRate);

	actorsPostConstructor();
}

void Game::actorsPostConstructor()
{
	gameField->setGameField(gameField);
	gameField->beginPlay();
	gameField->getPlayer()->setGameField(gameField);
	gameField->getPlayer()->beginPlay();
}

int Game::run()
{
	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
			processEvent(event);
		// TODO: implement multithreading
		update();
		render();
	}
	return 1;
	
}

void Game::processEvent(const sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::KeyPressed:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			gameField->getPlayer()->up();
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			gameField->getPlayer()->down();
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			gameField->getPlayer()->left();
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			gameField->getPlayer()->right();
		break;
	case sf::Event::Closed:
		window->close();
		break;
	default:
		break;
	}
}

void Game::update()
{
	gameField->tick();
	for (const auto& cubesArray : gameField->getCubes())
		for (const auto& cube : cubesArray)
			cube->tick();
	for (const auto& commonBall : gameField->getCommonBalls())
		commonBall->tick();
	for (const auto& heart : gameField->getHearts())
		heart->tick();
	for (const auto& prickle : gameField->getPrickles())
		prickle->tick();
	gameField->getPlayer()->tick();
	hud->tick();
}

void Game::render()
{
	window->clear();
	window->draw(*gameField);
	for (const auto& cubesArray : gameField->getCubes())
		for (const auto& cube : cubesArray)
			window->draw(*cube);
	for (const auto& commonBall : gameField->getCommonBalls())
		window->draw(*commonBall);
	for (const auto& heart : gameField->getHearts())
		window->draw(*heart);
	for (const auto& prickle : gameField->getPrickles())
		window->draw(*prickle);
	window->draw(*gameField->getPlayer());
	window->draw(*hud);
	window->display();
}
