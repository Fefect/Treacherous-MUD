
#include <SFML/Graphics.hpp>
 
#include "game_state.hpp"
#include "game_state_editor.hpp"
#include <iostream>

void GameStateEditor::draw(const float dt)
{
    this->game->window.clear(sf::Color::Black);
 
    this->game->window.setView(this->guiView);
    this->game->window.draw(this->game->background);
 
    this->game->window.setView(this->gameView);

	map.draw(this->game->window, dt);
	map.drawObjects(this->game->window, dt);

	this->game->window.setView(this->guiView);
    for(auto gui : this->guiSystem) this->game->window.draw(gui.second);
 
    return;
}
 
void GameStateEditor::update(const float dt)
{
    return;
}
 
void GameStateEditor::handleInput()
{
    sf::Event event;
 
    sf::Vector2f guiPos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->guiView);
    sf::Vector2f gamePos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->gameView);
 
    while(this->game->window.pollEvent(event))
    {
        switch(event.type)
        {
            /* Close the window */
            case sf::Event::Closed:
            {
                this->game->window.close();
                break;
            }
            /* Resize the window */
            case sf::Event::Resized:
            {
                gameView.setSize(event.size.width, event.size.height);
				gameView.zoom(zoomLevel);
                this->game->background.setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0, 0), this->guiView));
                this->game->background.setScale(
                    float(event.size.width) / float(this->game->background.getTexture()->getSize().x),
                    float(event.size.height) / float(this->game->background.getTexture()->getSize().y));
                break;
            }
		    case sf::Event::MouseMoved:
			{
		    /* Pan the camera */
		    if(this->actionState == ActionState::PANNING)
		    {
		        sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition(this->game->window) - this->panningAnchor);
		        gameView.move(-1.0f * pos * this->zoomLevel);
		        panningAnchor = sf::Mouse::getPosition(this->game->window);
		    }
		    /* Select tiles */
		    break;
		}
		   case sf::Event::MouseButtonPressed:
{
    /* Start panning */
    if(event.mouseButton.button == sf::Mouse::Middle)
    {
		this->guiSystem.at("rightClickMenu").hide();
        this->guiSystem.at("selectionCostText").hide();
        if(this->actionState != ActionState::PANNING)
        {
            this->actionState = ActionState::PANNING;
            this->panningAnchor = sf::Mouse::getPosition(this->game->window);
        }
    }
    else if(event.mouseButton.button == sf::Mouse::Left)
    {
        this->actionState = ActionState::SELECTING;
        sf::Vector2f pos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->gameView);
        selectionStart.x = pos.y / (this->map.tileSize) + pos.x / (2*this->map.tileSize) - this->map.width * 0.5 - 0.5;
        selectionStart.y = pos.y / (this->map.tileSize) - pos.x / (2*this->map.tileSize) + this->map.width * 0.5 + 0.5;
		if(this->guiSystem.at("rightClickMenu").visible)
		{
			std::string msg = this->guiSystem.at("rightClickMenu").activate(guiPos);
			if(msg == "cmd_examine")
			{
				std::cout << "Clicked the examine button \n";

				this->guiSystem.at("rightClickMenu").hide();
			}
		}
		else if(this->currentTile->tileType == TileType::GRASS)
	    {
	       this->map.select(selectionStart, {this->currentTile->tileType, TileType::WATER});
	    }
    }
    else if(event.mouseButton.button == sf::Mouse::Right)
    {
        /* Stop selecting */
        if(this->actionState == ActionState::SELECTING)
        {
            this->actionState = ActionState::NONE;
        }
		if(!this->guiSystem.at("rightClickMenu").visible)
		{
			 /* Open the tile select menu. */
        sf::Vector2f pos = guiPos;
 
        if(pos.x > this->game->window.getSize().x - this->guiSystem.at("rightClickMenu").getSize().x)
        {
            pos -= sf::Vector2f(this->guiSystem.at("rightClickMenu").getSize().x, 0);
        }
        if(pos.y > this->game->window.getSize().y - this->guiSystem.at("rightClickMenu").getSize().y)
        {
            pos -= sf::Vector2f(0, this->guiSystem.at("rightClickMenu").getSize().y);
        }
        this->guiSystem.at("rightClickMenu").setPosition(pos);
        this->guiSystem.at("rightClickMenu").show();
		}
		else
		{
			this->guiSystem.at("rightClickMenu").hide();
		}
    }
    break;
}
case sf::Event::MouseButtonReleased:
{
    /* Stop panning */
    if(event.mouseButton.button == sf::Mouse::Middle)
    {
        this->actionState = ActionState::NONE;
    }
    break;
}
    /* Zoom the view */
    case sf::Event::MouseWheelMoved:
    {
        if(event.mouseWheel.delta < 0)
        {
            gameView.zoom(2.0f);
            zoomLevel *= 2.0f;
        }
        else
        {
            gameView.zoom(0.5f);
            zoomLevel *= 0.5f;
        }
        break;
	 }
	
    default: break;
        }
    }
}
 
GameStateEditor::GameStateEditor(Game* game)
{
    this->game = game;
    sf::Vector2f pos = sf::Vector2f(this->game->window.getSize());
    this->guiView.setSize(pos);
    this->gameView.setSize(pos);
    pos *= 0.5f;
    this->guiView.setCenter(pos);
    this->gameView.setCenter(pos);
 
    map = Map("city_map1.dat", 64, 64, game->tileAtlas);
 
    this->zoomLevel = 1.5f;
 
    /* Centre the camera on the map */
    sf::Vector2f centre(this->map.width, this->map.height*0.5);
    centre *= float(this->map.tileSize);
    gameView.setCenter(centre);
 
    this->selectionStart = sf::Vector2i(0, 0);
    this->selectionEnd = sf::Vector2i(0, 0);
 
    this->currentTile = &this->game->tileAtlas.at("1");
    this->actionState = ActionState::NONE;

		/* Create gui elements. */
	this->guiSystem.emplace("rightClickMenu", Gui(sf::Vector2f(196, 16), 2, false, this->game->stylesheets.at("button"),
	    {
			std::make_pair("Examine", "cmd_examine"),
	    }));
	 
	this->guiSystem.emplace("selectionCostText", Gui(sf::Vector2f(196, 16), 0, false, this->game->stylesheets.at("text"),
	    { std::make_pair("", "") }));

		this->guiSystem.emplace("inventory", Gui(sf::Vector2f(300,150), 2, true,
	                                       this->game->stylesheets.at("button"),
	    {
	        std::make_pair("",          "player_inventory")
	    }));

	this->guiSystem.at("inventory").setPosition(sf::Vector2f((this->game->window.getSize().x - 300) , this->game->window.getSize().y - 150));

	this->guiSystem.at("inventory").show();

	this->guiSystem.emplace("inventory_slot_1", Gui(sf::Vector2f(30,30), 2, true,
                                       this->game->stylesheets.at("button"),
    {
        std::make_pair("",          "player_inventory_slot_1")
    }));

	this->guiSystem.at("inventory_slot_1").setPosition(sf::Vector2f((this->game->window.getSize().x - 295) , this->game->window.getSize().y - 145));

	this->guiSystem.at("inventory_slot_1").show();

	this->guiSystem.emplace("infoBar", Gui(sf::Vector2f(this->game->window.getSize().x / 5, 16), 2, true,
	                                       this->game->stylesheets.at("button"),
	    {
	        std::make_pair("health",          "player_health")
	    }));
	this->guiSystem.at("infoBar").setPosition(sf::Vector2f(0, this->game->window.getSize().y - 16));
	this->guiSystem.at("infoBar").show();
}
