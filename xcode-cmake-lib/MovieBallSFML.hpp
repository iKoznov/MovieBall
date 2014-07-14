#pragma once

#if GLEW_FOUND
#include <GL/glew.h>
#endif

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

class Game {
	
public:
	Game();
	void run();
	
private:
	void processEvents();
	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
	void update();
	void render();
	
private:;
	const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);
	sf::RenderWindow mWindow;
	sf::CircleShape mPlayer;
	bool mIsMovingUp, mIsMovingDown, mIsMovingLeft, mIsMovingRight;
	
};
