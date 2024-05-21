#include "Game.h"

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

	this->t[1]->loadFromFile("Images/fg.png");
	this->fg = new sf::Sprite(*this->t[1]);

	this->t[2]->loadFromFile("Images/red.png");
	this->red = new sf::Sprite(*this->t[2]);

	this->t[3]->loadFromFile("Images/yellow.png");
	this->yellow = new sf::Sprite(*this->t[3]);
}

void Game::updateEvent()
{
	sf::Event event;
	while (this->window->pollEvent(event)) {
		if (event.type == sf::Event::Closed) this->window->close();
	}
}

void Game::update()
{
}

void Game::render()
{
	this->window->clear();
	
	this->window->draw(*this->bg);
	this->window->draw(*this->fg);

	this->window->display();
}
