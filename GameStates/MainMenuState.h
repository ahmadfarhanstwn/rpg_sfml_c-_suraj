#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include <GameState.h>
#include "EditorState.h"
#include "Buttons.h"

class MainMenuState : public State
{
private:
    void initVariables();
    void initBackgrounds();
    void initKeyBinds();
    void initFonts();
    void initButtons();

    sf::Texture backgroundTexture;
    sf::RectangleShape background;
    sf::Font font;

    std::map<std::string, Buttons*> buttons;

public:
    MainMenuState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states);
    virtual ~MainMenuState();

    //Functions
    void updateInput(const float& dt);

    void updateButtons();
    void update(const float& dt);
    void renderButtons(sf::RenderTarget* target = nullptr);
    void render(sf::RenderTarget* target = nullptr);
};

#endif // MAINMENUSTATE_H
