#ifndef ENDSCREEN_H
#define ENDSCREEN_H

#include <SFML\Graphics.hpp>
#include <iostream>
#include "Xbox360Controller.h"
#include "ResourceManager.h"
#include "GameState.h"
#include "Widget.h"

class EndScreen
{
public: 
	// Function to construct the end screen
	EndScreen(sf::Font font, GameState *gameState);

	// Function to de-construct the end screen
	~EndScreen();

	// Function to update the end screen
	void update(Xbox360Controller & controller, sf::Time dt);

	// Function to draw everything in the end screen
	void render(sf::RenderWindow & window);

	void checkButton(GamePadState m_state, Xbox360Controller m_controller);	// Function to check if the selected button is clicked

private: 
	GameState * m_gameState;
	Button * m_backButton;
	std::string m_backString = "Back";
	sf::Font m_font;
	sf::View m_view;

};
#endif // !ENDSCREEN_H
