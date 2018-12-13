#pragma once

#include <SFML\Graphics\RectangleShape.hpp>

class Cube : public sf::RectangleShape
{
public:
	Cube(const sf::Vector2f& pos, const int width, const int height);

	void tick();

	void beginPlay();

	void setFlag(const int flag);
	int getFlag() const;
	
	void setActorHiddenInGame(const bool bHiden = true);
	void setActorEnableCollision(const bool bEnableCollision = true);

	bool getActorEnableCollision() const;

	const sf::Vector2f& getActorLocation() const;

	void addTag(const std::string& tag);
	void removeSingleTag(const std::string& tag);

private:
	int flag; // TODO: replace with enum class

	sf::Vector2f actorLocation;

	sf::Uint8 bHiden = 0;
	bool bEnableCollision = false;

	std::vector<std::string> tags;
};

