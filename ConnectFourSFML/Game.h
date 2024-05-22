#pragma once
#include "SFML/Graphics.hpp"
#include <vector>

class Game
{
public:
	void run();
	virtual ~Game();

private:
	sf::RenderWindow* window;
	sf::Texture* t[3];
	sf::Sprite* bg;
	sf::Sprite* fg;
	sf::Sprite* red;
	sf::Sprite* yellow;
	sf::Sprite* ghost;
	sf::RectangleShape* highlight;

	unsigned short int grid[6][7];
	bool turn;
	bool highlighted;

	void init();
	void updateEvent();
	void update();
	void render();
};

