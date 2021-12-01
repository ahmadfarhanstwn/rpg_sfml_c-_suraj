#include "State.h"

State::State(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states)
{
    this->states = states;
    this->window = window;
    this->supportedKeys = supportedKeys;
    this->quit = false;
}

State::~State()
{
    //dtor
}

const bool& State::getQuit() const
{
    return this->quit;
}

void State::updateMousePosition()
{
    this->mousePosScreen = sf::Mouse::getPosition();
    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    this->mousePosView = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));
}

void State::endState()
{
    this->quit = true;
}