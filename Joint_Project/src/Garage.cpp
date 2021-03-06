#include <Garage.h>

Garage::Garage(sf::Font font, sf::Font currencyFont, GameState * gameState, Player *player) :
	m_textFont(font),
	m_currencyFont(currencyFont)
{
	m_gameState = gameState;
	m_player = player;

	// set the integers for keeping track of currently highlighted options to 0 so they start off on the "first" in each list
	m_currentCar = 0;
	m_currentStat = 1; // the DJ Dilemma
	m_textFont = font;

	// generate and place the images of each upgradeable car as well as the curent stats of the cars, with each upgrade bath and its current value set at the bottom of the screen
	for (int i = 0; i < 3; i++)
	{
		// load in bus textures form the resource manager and set its position and scale to be slightly reduced
		m_carOptions[i] = ResourceManager::instance().m_holder["Bus" + std::to_string(i)];
		m_cars[i].setTexture(m_carOptions[i]);
		m_cars[i].setPosition(50, 100 + (i * 100));
		m_cars[i].setScale(.75,.75);
		m_cars[i].setColor(m_dim);

		// loading textures from resource manager for upgrade sprites
		m_upgradeText[i] = ResourceManager::instance().m_holder["Upgrade" + std::to_string(i)];
		m_uprgradeSpr[i].setTexture(m_upgradeText[i]);
		m_uprgradeSpr[i].setOrigin(m_upgradeText[i].getSize().x / 2, m_upgradeText[i].getSize().y / 2);
		m_uprgradeSpr[i].setPosition(225 + (i * 225), 500);
		m_uprgradeSpr[i].setScale(.1, .1);

		// generating bars located below upgrde sprites to track the "progression" of the upgrades
		m_upgradeBars[i] = new Bar(sf::Vector2f(225 + (i * 225), 550), 25);

		//stat value sliders to keep display the current stats of each car as well as a label for each
		m_statValues[i] = new Slider(&sf::Vector2f(450, 150 + (i * 75)), 11);
		m_statLabel[i] = new Label(&m_strings[i], &m_textFont, &sf::Vector2f(350, 160 + (i * 75)), 15, sf::Color(140, 80, 160));
		m_statLabel[i]->loseFocus();
	}

	m_currency = new Label(&m_strings[3], &m_currencyFont, &sf::Vector2f(50, 20), 15, sf::Color(255, 205, 0));
	m_cost = new Label(&m_strings[4], &m_currencyFont, &sf::Vector2f(450, 580), 15, sf::Color(255, 205, 0));

	m_cars[m_currentCar].setColor(m_highlight); // sets the defa+ult hightlighted car

	updateUI();

	m_upgrdCost = m_player->m_playerCar[m_currentCar]->m_upgradeLevels[m_currentStat] + 1;
}

Garage::~Garage()
{
}

void Garage::update(Xbox360Controller & controller, sf::Time dt)
{
	std::stringstream ssCurr;
	ssCurr << m_strings[3] << m_player->m_currency;

	std::stringstream ssCost;
	ssCost << m_strings[4] << m_upgrdCost;

	m_currency->updateText(ssCurr);
	m_cost->updateText(ssCost);

	checkSelection(controller.m_currentState, controller); // check for navigation i.e. up down selection process and highlighting effects
	checkButton(controller.m_currentState, controller); // check for button pressing, which will select car/stat to upgrade
}

void Garage::render(sf::RenderWindow &window)
{
	window.draw(m_backgroundSprite);
	m_currency->render(window);
	if (m_upgradeBars[m_currentStat]->returnNode() != 3)
	{
		m_cost->render(window);
	}

	for (int i = 0; i < 3; i++)
	{
		// render each element
		window.draw(m_cars[i]);
		window.draw(m_uprgradeSpr[i]);
		m_statValues[i]->render(window);
		m_statLabel[i]->render(window);
		m_upgradeBars[i]->render(window);
	}
}


// Function to check which button is selected
void Garage::checkSelection(GamePadState m_state, Xbox360Controller m_controller)
{
	// if down pressed go down the list of cars/stats and keep track of current car/stat thats highlighted, focusing/unfocusing them in the process
	if (m_controller.m_currentState.LeftThumbStick.y > 50 && m_controller.m_previousState.LeftThumbStick.y < 50)
	{
		if (m_statLabel[m_currentStat]->hasFocus())
		{
			m_statLabel[m_currentStat]->loseFocus();
			m_currentStat++;
			if (m_currentStat > 2)
			{
				m_currentStat = 2;
			}
			m_statLabel[m_currentStat]->getFocus();

			m_upgrdCost = m_player->m_playerCar[m_currentCar]->m_upgradeLevels[m_currentStat] + 1;
		}

		else
		{
			// highlights/dims cars depending on movement of controller stick,  increments the integer that tracks the currently selected car
			m_cars[m_currentCar].setColor(m_dim);
			m_currentCar++;
			if (m_currentCar > 2)
			{
				m_currentCar = 2;
			}
			m_cars[m_currentCar].setColor(m_highlight);

			updateUI();
		}
	}

	// if up pressed go up the list of cars/stats and keep track of current car/stat thats highlighted, focusing/unfocusing them in the process
	if (m_controller.m_currentState.LeftThumbStick.y < -50 && m_controller.m_previousState.LeftThumbStick.y > -50)
	{
		if (m_statLabel[m_currentStat]->hasFocus())
		{
			m_statLabel[m_currentStat]->loseFocus();
			m_currentStat--;
			if (m_currentStat < 0)
			{
				m_currentStat = 0;
			}
			m_statLabel[m_currentStat]->getFocus();

			m_upgrdCost = m_player->m_playerCar[m_currentCar]->m_upgradeLevels[m_currentStat] + 1;
		}

		else
		{
			// highlights/dims cars depending on movement of controller stick, decrements the integer that tracks the currently selected car
			m_cars[m_currentCar].setColor(m_dim);
			m_currentCar--;
			if (m_currentCar < 0)
			{
				m_currentCar = 0;
			}
			m_cars[m_currentCar].setColor(m_highlight);

			updateUI();
		}
	}
}

// Function to check if the selected button has been pressed
void Garage::checkButton(GamePadState m_gamePadState, Xbox360Controller m_controller)
{
	if (m_controller.m_currentState.B && !m_controller.m_previousState.B)
	{
		// "B" button logic, will return to the previously selected car
		if (m_statLabel[m_currentStat]->hasFocus())
		{
			m_statLabel[m_currentStat]->loseFocus();
			m_cars[m_currentCar].setColor(m_highlight);
			m_currentStat = 1;
		}

		else
		{
			// return to the main menu screen
			*m_gameState = GameState::MENU_STATE;
		}
	}
	
	if (m_controller.m_currentState.A && !m_controller.m_previousState.A)
	{
		// when the "A" button is pressed and the "cursor" is not currenlty on a stat label/bar switch to the stat selection list
		if (!m_statLabel[m_currentStat]->hasFocus())
		{
			m_statLabel[m_currentStat]->getFocus();
		}

		else
		{
			// when the "A" button is pressed whilst on the stat list, upgrade the currently selected stat
			if (m_player->m_playerCar[m_currentCar]->m_upgradeLevels[m_currentStat] != 3 && m_player->m_currency >= m_upgrdCost)
			{
				m_player->m_currency = m_player->m_currency - m_upgrdCost;
				m_player->m_playerCar[m_currentCar]->updateStat(m_strings[m_currentStat]);
				m_player->m_playerCar[m_currentCar]->m_upgradeLevels[m_currentStat]++;
				m_upgradeBars[m_currentStat]->increment();
				m_statValues[m_currentStat]->incrementSlider();
				m_upgrdCost = m_player->m_playerCar[m_currentCar]->m_upgradeLevels[m_currentStat] + 1;
			}
		}
	}
}

void Garage::updateUI()
{
	for (int i = 0; i < 3; i ++)
	{
		m_upgradeBars[i]->reset();
		m_upgradeBars[i]->assignValues(m_player->m_playerCar[m_currentCar]->m_upgradeLevels[i]);
	}

	m_statValues[0]->setCurrentSegment(m_player->m_playerCar[m_currentCar]->returnDecel());
	m_statValues[1]->setCurrentSegment(m_player->m_playerCar[m_currentCar]->returnHandle());
	m_statValues[2]->setCurrentSegment(m_player->m_playerCar[m_currentCar]->returnAcel());
}