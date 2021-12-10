#include "stdafx.h"
#include "EditorState.h"

//Init Functions
void EditorState::initVariables()
{
    this->textureRect = sf::IntRect(0,0, static_cast<int>(this->stateData->gridSize), static_cast<int>(this->stateData->gridSize));
}

void EditorState::initBackgrounds()
{

}

void EditorState::initKeyBinds()
{
    std::ifstream ifs("Config/editorkeybinds.ini");

    if (ifs.is_open())
    {
        std::string key = "";
        std::string suppKey = "";
        while (ifs >> key >> suppKey)
        {
            this->keyBinds[key] = this->supportedKeys->at(suppKey);
        }
    }
}

void EditorState::initFonts()
{
    if (!this->font.loadFromFile("Fonts/Dosis-Light.ttf"))
    {
        throw("ERROR::EDITORSTATE::COULDN'T LOAD FONT(Dosis-Light.ttf)");
    }
}

void EditorState::initText()
{
    this->cursorText.setFont(this->font);
    this->cursorText.setFillColor(sf::Color::White);
    this->cursorText.setCharacterSize(12);
    this->cursorText.setPosition(this->mousePosView.x, this->mousePosView.y - 50.f);
}

void EditorState::initPauseMenu()
{
    this->pMenu = new PauseMenu(*this->window, this->font);

    this->pMenu->addButtons("SAVE", 150.f, "Save Map");
    this->pMenu->addButtons("QUIT", 225.f, "Exit Map");
}

void EditorState::initButtons()
{

}

void EditorState::initGui()
{
    //Sidebar
    this->sidebar.setSize(sf::Vector2f(30.f, static_cast<float>(this->stateData->graphicsSettings->resolution.height)));
    this->sidebar.setFillColor(sf::Color(50,50,50,100));
    this->sidebar.setOutlineColor(sf::Color(200,200,200,150));
    this->sidebar.setOutlineThickness(1.f);

    this->selectorRect.setSize(sf::Vector2f(this->stateData->gridSize, this->stateData->gridSize));
    this->selectorRect.setFillColor(sf::Color(255,255,255,150));
    this->selectorRect.setOutlineThickness(1.f);
    this->selectorRect.setOutlineColor(sf::Color::Green);

    this->selectorRect.setTexture(this->tileMap->getTileSheet());
    this->selectorRect.setTextureRect(this->textureRect);

    //Init Text Selector
    this->textureSelector = new gui::TextureSelector(20.f, 20.f, 800.f, 200.f,
                                                     this->stateData->gridSize ,this->tileMap->getTileSheet(),
                                                     this->font, "X");
}

void EditorState::initTileMap()
{
    this->tileMap = new TileMap(this->stateData->gridSize, 10,10, "Resources/Images/Tiles/tilesheet1.png");
}

EditorState::EditorState(StateData* state_data) :
    State(state_data)
{
    this->initVariables();
    this->initBackgrounds();
    this->initFonts();
    this->initText();
    this->initKeyBinds();
    this->initPauseMenu();
    this->initButtons();
    this->initTileMap();
    this->initGui();
}

EditorState::~EditorState()
{
    for (auto it = this->buttons.begin(); it != buttons.end(); it++)
        delete it->second;
    delete this->pMenu;
    delete this->tileMap;
    delete this->textureSelector;
}

void EditorState::updateInput(const float& dt)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keyBinds.at("Escape"))) && this->getKeytime())
    {
        if (!this->paused)
            this->pauseState();
        else
            this->unpauseState();
    }
}

void EditorState::updateEditorInput(const float& dt)
{
    // Add a tile
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->getKeytime())
    {
        if (!this->sidebar.getGlobalBounds().contains(sf::Vector2f(this->mousePosWindow)))
        {
            if (!this->textureSelector->getActive())
            {
                this->tileMap->addTile(this->mousePosGrid.x, this->mousePosGrid.y, 0, this->textureRect);
            }
            else
            {
                this->textureRect = this->textureSelector->getTextureRect();
            }
        }
    }
    // Remove a tile
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && this->getKeytime())
    {
        if (!this->textureSelector->getActive() && !this->sidebar.getGlobalBounds().contains(sf::Vector2f(this->mousePosWindow)))
            this->tileMap->removeTile(this->mousePosGrid.x, this->mousePosGrid.y, 0);
    }
}

void EditorState::updateButtons()
{
    /**CHECK UPDATE FOR ALL BUTTONS AND CHECK IF A BUTTON IS PRESSED**/
    for(auto &it : this->buttons)
    {
        it.second->update(this->mousePosView);
    }
}

void EditorState::updateGui(const float& dt)
{
    if (!this->textureSelector->getActive())
    {
        this->selectorRect.setTextureRect(this->textureRect);
        this->selectorRect.setPosition(this->mousePosGrid.x * this->stateData->gridSize,
                                       this->mousePosGrid.y * this->stateData->gridSize);
    }
    this->cursorText.setPosition(this->mousePosView.x, this->mousePosView.y - 100.f);
    std::stringstream ss;
    ss << this->mousePosView.x << "  " << this->mousePosView.y <<
    "\n" << this->mousePosGrid.x << "  " << this->mousePosGrid.y <<
    "\n" << this->textureRect.left << "  " << this->textureRect.top;
    this->cursorText.setString(ss.str());
    this->textureSelector->update(this->mousePosWindow, dt);
}

void EditorState::updatePauseMenuButtons()
{
    if (this->pMenu->isPressed("SAVE") && this->getKeytime())
    {
        this->tileMap->saveToFile("test.slmp");
    }
    if (this->pMenu->isPressed("QUIT") && this->getKeytime())
    {
        this->endState();
    }
}

void EditorState::update(const float& dt)
{
    this->updateMousePosition();
    this->updateKeytime(dt);
    this->updateInput(dt);

    if (!this->paused) //unpaused
    {
        this->updateButtons();
        this->updateGui(dt);
        this->updateEditorInput(dt);
    }
    else //paused
    {
        this->pMenu->update(this->mousePosView);
        this->updatePauseMenuButtons();
    }
}

void EditorState::renderButtons(sf::RenderTarget& target)
{
    for(auto &it : this->buttons)
    {
        it.second->render(target);
    }
}

void EditorState::renderGui(sf::RenderTarget& target)
{
    if (!this->textureSelector->getActive())
        target.draw(this->selectorRect);
    this->textureSelector->render(target);
    target.draw(this->cursorText);
    target.draw(this->sidebar);
}

void EditorState::render(sf::RenderTarget* target)
{
    if (!target)
        target = this->window;

    this->tileMap->render(*target);

    this->renderButtons(*target);
    this->renderGui(*target);

    //render pause menu
    if (this->paused)
    {
        this->pMenu->render(*target);
    }
}
