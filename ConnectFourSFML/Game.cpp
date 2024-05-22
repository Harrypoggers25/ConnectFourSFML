#include "Game.h"
#include <iostream>

#define OFFSET_X 50
#define OFFSET_Y 200

void Game::run()
{
	this->init();
	while (this->window->isOpen()) {
		this->updateEvent();
		this->update();
		this->render();
	}
}

Game::~Game()
{
	delete this->window;

	for (size_t i = 0; i < 3; i++) delete this->t[i];

	delete this->bg;
	delete this->fg;
	delete this->red;
	delete this->yellow;
}

void Game::init()
{
	this->window = new sf::RenderWindow(
		sf::VideoMode(800, 800), 
		"Connect Four SFML", 
		sf::Style::Titlebar | sf::Style::Close
	);

	for (size_t i = 0; i < 3; i++) this->t[i] = new sf::Texture();

	this->t[0]->loadFromFile("Images/bg.png");
	this->bg = new sf::Sprite(*this->t[0]);
	this->bg->setPosition(OFFSET_X, OFFSET_Y);

	this->t[1]->loadFromFile("Images/fg.png");
	this->fg = new sf::Sprite(*this->t[1]);
	this->fg->setPosition(OFFSET_X, OFFSET_Y);

	this->t[2]->loadFromFile("Images/chips.png");

	this->red = new sf::Sprite(*this->t[2]);
	this->red->setTextureRect(sf::IntRect(0, 0, 100, 100));

	this->yellow = new sf::Sprite(*this->t[2]);
	this->yellow->setTextureRect(sf::IntRect(100, 0, 100, 100));

	this->ghost = new sf::Sprite(*this->t[2]);
	this->ghost->setTextureRect(sf::IntRect(0, 0, 100, 100));

	this->highlight = new sf::RectangleShape(sf::Vector2f(100.f, 600.f));
	this->highlight->setFillColor(sf::Color(255, 255, 255, 50));

	this->turn = false; // False = Red; True = Yellow
	this->highlighted = false;
}

void Game::updateEvent()
{
	sf::Event event;
	while (this->window->pollEvent(event)) {
		if (event.type == sf::Event::Closed) this->window->close();
		if (event.type == sf::Event::MouseMoved) {
			sf::Vector2i mousePos = sf::Mouse::getPosition(*this->window);
			if (mousePos.x >= OFFSET_X && mousePos.x < 700 + OFFSET_X && 
				mousePos.y >= OFFSET_Y && mousePos.y < 600 + OFFSET_Y) {
				float x = (float)((mousePos.x - OFFSET_X) / 100 * 100 + OFFSET_X);
				this->highlighted = true;

				this->highlight->setPosition(x, (float)OFFSET_Y);
				this->ghost->setPosition(x, (float)(OFFSET_Y - 100));
			}
			else this->highlighted = false;
		}
		if (event.type == sf::Event::MouseLeft) this->highlighted = false;
		if (event.type == sf::Event::MouseButtonPressed) {
			sf::Vector2i mousePos = sf::Mouse::getPosition(*this->window);
			if (mousePos.x >= OFFSET_X && mousePos.x < 700 + OFFSET_X &&
				mousePos.y >= OFFSET_Y && mousePos.y < 600 + OFFSET_Y) {
				unsigned short int col = (mousePos.x - OFFSET_X) / 100;
				
				//bool oldTurn = turn;
				for (int i = 0; i < 6; i++) {
					if (grid[i][col] == 0) {
						grid[i][col] = turn == false ? 1 : 2;
						turn = !turn;
						this->ghost->setTextureRect(sf::IntRect(turn == false ? 0 : 100, 0, 100, 100));
						break;
					}
				}
				//if (oldTurn == turn)

				auto checkWin = [](const unsigned short int grid[6][7]) {
					auto checkLine = [](const std::string& str) {
						return str.find("1111") != std::string::npos || str.find("2222") != std::string::npos;
					};

					// Check vertical
					for (size_t j = 0; j < 7; j++) {
						if (grid[0][j] == 0) continue;	// Row empty
						std::string str =
							std::to_string(grid[0][j]) +
							std::to_string(grid[1][j]) +
							std::to_string(grid[2][j]) +
							std::to_string(grid[3][j]) +
							std::to_string(grid[4][j]) +
							std::to_string(grid[5][j]);
						if (str.find("000") != std::string::npos) continue; // Impossible win
						if (checkLine(str)) return true;
					}

					// Check horizontal
					for (size_t i = 0; i < 6; i++) {
						std::string str =
							std::to_string(grid[i][0]) +
							std::to_string(grid[i][1]) +
							std::to_string(grid[i][2]) +
							std::to_string(grid[i][3]) +
							std::to_string(grid[i][4]) +
							std::to_string(grid[i][5]) +
							std::to_string(grid[i][6]);
						if (str.find("0000") != std::string::npos) break; // Impossible win
						if (checkLine(str)) return true;
					}

					// Check diagonals
					// Down-right diagonals
					for (int i = 0; i <= 2; ++i) {
						for (int j = 0; j <= 3; ++j) {
							std::string str;
							for (int k = 0; k < 4; ++k) {
								str += std::to_string(grid[i + k][j + k]);
							}
							if (checkLine(str)) {
								return true;
							}
						}
					}

					// Up-right diagonals
					for (int i = 3; i < 6; ++i) {
						for (int j = 0; j <= 3; ++j) {
							std::string str;
							for (int k = 0; k < 4; ++k) {
								str += std::to_string(grid[i - k][j + k]);
							}
							if (checkLine(str)) {
								return true;
							}
						}
					}

					return false;
				};
				
				if (checkWin(this->grid)) std::cout << "You win !" << std::endl;
			}
		}
	}
}

void Game::update()
{
}

void Game::render()
{
	this->window->clear();
	
	this->window->draw(*this->bg);
	
	for (size_t i = 0; i < 6; i++) {
		for (size_t j = 0; j < 7; j++) {
			size_t y = 5 - i;
			if (this->grid[i][j] == 1) {
				this->red->setPosition(j * 100 + OFFSET_X, y * 100 + OFFSET_Y);
				this->window->draw(*this->red);
			}
			else if (this->grid[i][j] == 2) {
				this->yellow->setPosition(j * 100 + OFFSET_X, y * 100 + OFFSET_Y);
				this->window->draw(*this->yellow);
			}
		}
	}

	this->window->draw(*this->fg);
	if (this->highlighted) {
		this->window->draw(*this->ghost);
		this->window->draw(*this->highlight);
	}

	this->window->display();
}
