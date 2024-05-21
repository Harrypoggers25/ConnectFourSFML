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

	for (size_t i = 0; i < 4; i++) delete this->t[i];

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

	for (size_t i = 0; i < 4; i++) this->t[i] = new sf::Texture();

	this->t[0]->loadFromFile("Images/bg.png");
	this->bg = new sf::Sprite(*this->t[0]);
	this->bg->setPosition(OFFSET_X, OFFSET_Y);

	this->t[1]->loadFromFile("Images/fg.png");
	this->fg = new sf::Sprite(*this->t[1]);
	this->fg->setPosition(OFFSET_X, OFFSET_Y);

	this->t[2]->loadFromFile("Images/red.png");
	this->red = new sf::Sprite(*this->t[2]);

	this->t[3]->loadFromFile("Images/yellow.png");
	this->yellow = new sf::Sprite(*this->t[3]);

	this->highlight = new sf::RectangleShape(sf::Vector2f(0, 0));
	this->highlight->setFillColor(sf::Color(255, 255, 255, 50));

	this->turn = false; // False = Red; True = Yellow
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
				this->highlight->setSize(sf::Vector2f(100.f, 600.f));
				this->highlight->setPosition((float)((mousePos.x - OFFSET_X) / 100 * 100 + OFFSET_X), OFFSET_Y);
			}
			else this->highlight->setSize(sf::Vector2f(0, 0));
		}
		if (event.type == sf::Event::MouseLeft) this->highlight->setSize(sf::Vector2f(0, 0));
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
						break;
					}
				}
				//if (oldTurn == turn)
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
	this->window->draw(*this->highlight);

	this->window->display();
}
