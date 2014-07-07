#include "engine/HelloIOS.h"
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

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

Game::Game()
: mWindow ( sf::VideoMode ( 640, 480 ), "SFML Application" )
, mPlayer()
, mIsMovingUp(false), mIsMovingDown(false), mIsMovingLeft(false), mIsMovingRight(false)
{
	mPlayer.setRadius ( 40.f );
	mPlayer.setPosition ( 100.f, 100.f );
	mPlayer.setFillColor ( sf::Color::Cyan );
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	
	while ( mWindow.isOpen() )
	{
		processEvents();
		timeSinceLastUpdate += clock.restart();
		
		while (timeSinceLastUpdate > TimePerFrame) {
			timeSinceLastUpdate -= TimePerFrame;
			processEvents();
			update();
		}
		
		processEvents();
		update();
		render();
	}
}

void Game::processEvents()
{
	sf::Event event;
	while ( mWindow.pollEvent ( event ) ) {
		switch ( event.type ) {
			case sf::Event::KeyPressed:
				handlePlayerInput ( event.key.code, true );
				break;
			case sf::Event::KeyReleased:
				handlePlayerInput ( event.key.code, false );
				break;
			case sf::Event::Closed:
				mWindow.close();
				break;
		}
	}
}

void Game::handlePlayerInput ( sf::Keyboard::Key key, bool isPressed )
{
	if ( key == sf::Keyboard::W )
		mIsMovingUp = isPressed;
	else if ( key == sf::Keyboard::S )
		mIsMovingDown = isPressed;
	else if ( key == sf::Keyboard::A )
		mIsMovingLeft = isPressed;
	else if ( key == sf::Keyboard::D )
		mIsMovingRight = isPressed;
}

void Game::update()
{
	sf::Vector2f movement ( 0.f, 0.f );
	if ( mIsMovingUp )
		movement.y -= 1.f;
	if ( mIsMovingDown )
		movement.y += 1.f;
	if ( mIsMovingLeft )
		movement.x -= 1.f;
	if ( mIsMovingRight )
		movement.x += 1.f;
	
	mPlayer.move ( 100.f * movement * TimePerFrame.asSeconds() );
}

void Game::render()
{
	mWindow.clear();
	mWindow.draw ( mPlayer );
	mWindow.display();
}

int main(int argc, char *argv[])
{
	hello();
	
	Game game;
	game.run();
	
	// create the window
	sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, sf::ContextSettings(32));
	
	// load resources, initialize the OpenGL states, ...
	
	// run the main loop
    bool running = true;
    while (running)
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // end the program
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }
		
        // clear the buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
        // draw...
		
        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }
	
    // release resources...
	
	return 0;
}
