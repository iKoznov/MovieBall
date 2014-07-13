#include "MovieBallSFML.hpp"

#include <boost/format.hpp>
#include <thread>
#include <chrono>

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
		
		auto idleTime = TimePerFrame - timeSinceLastUpdate;
		if (idleTime > sf::Time::Zero)
			std::this_thread::sleep_for(std::chrono::milliseconds(idleTime.asMilliseconds()));
		
		while (timeSinceLastUpdate >= TimePerFrame) {
			timeSinceLastUpdate -= TimePerFrame;
			processEvents();
			update();
		}
		
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
	
	mPlayer.move ( 1000.f * movement * TimePerFrame.asSeconds() );
}

void Game::render()
{
	mWindow.clear();
	mWindow.draw ( mPlayer );
	mWindow.display();
}

