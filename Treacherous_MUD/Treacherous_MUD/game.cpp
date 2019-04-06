
#include <stack>
 
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
 
#include "game.hpp"
#include "game_state.hpp"
#include "texture_manager.hpp"
#include "animation_handler.hpp"
#include "tile.hpp"

Game::Game()
{
    this->loadTextures();
	this->loadTiles();
	this->loadFonts();
	this->loadStylesheets();
	
 
    this->window.create(sf::VideoMode(800, 600), "Treacherous MUD");
    this->window.setFramerateLimit(60);
 
    this->background.setTexture(this->texmgr.getRef("background"));
}

void Game::pushState(GameState* state)
{
    this->states.push(state);
 
    return;
}

void Game::loadTextures()
{
	
	texmgr.placeStaticTexture();
	texmgr.loadTexture("grass",         "media/grass.png");
    texmgr.loadTexture("forest",        "media/forest.png");
    texmgr.loadTexture("water",         "media/water.png");
    texmgr.loadTexture("residential",   "media/residential.png");
    texmgr.loadTexture("commercial",    "media/commercial.png");
    texmgr.loadTexture("industrial",    "media/industrial.png");
    texmgr.loadTexture("road",          "media/road.png");
	texmgr.loadTexture("static_water",	"media/water_static.png");
	texmgr.loadTexture("void", "");

    texmgr.loadTexture("background", "media/background.png");

	texmgr.loadTexture("box_bottom",         "media/box_bottom.png");
	texmgr.loadTexture("box_top",         "media/box_top.png");
}
 
void Game::popState()
{
    delete this->states.top();
    this->states.pop();
 
    return;
}
 
void Game::changeState(GameState* state)
{
    if(!this->states.empty())
        popState();
    pushState(state);
 
    return;
}
 
GameState* Game::peekState()
{
    if(this->states.empty()) return nullptr;
    return this->states.top();
}
 
void Game::gameLoop()
{
    sf::Clock clock;
 
    while(this->window.isOpen())
    {
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();
 
        if(peekState() == nullptr) continue;
        peekState()->handleInput();
        peekState()->update(dt);
        this->window.clear(sf::Color::Black);
        peekState()->draw(dt);
        this->window.display();
    }
}

Game::~Game()
{
    while(!this->states.empty()) popState();
}

void Game::loadSpriteSheet()
{
	
}

void Game::loadTiles()
{
	
    Animation staticAnim(0, 0, 1.0f);

	int maxHeight = 10;
	int maxWidth = 3;

	int maxAmount = maxHeight * maxWidth;

	for(int i = 0; i < maxAmount; i++)
	{
		this->tileAtlas[std::to_string(i)] = Tile(this->tileSize,1,texmgr.getStaticRef(i), {staticAnim}, TileType::STATIC_WATER);
	}

	// this->tileAtlas["static_water"] =
	// 	Tile(this->tileSize, 1, texmgr.getRef("static_water"),
	// 		{staticAnim},
	// 		TileType::STATIC_WATER);
	this->tileAtlas["void"] =
       Tile(this->tileSize, 1, texmgr.getRef("void"),
           { staticAnim },
           TileType::VOID);
 //
	//   this->tileAtlas["box_top"] =
 //        Tile(this->tileSize, 1, texmgr.getRef("box_top"),
 //            { staticAnim },
 //            TileType::GRASS);
 //
	//   this->tileAtlas["box_bottom"] =
 //        Tile(this->tileSize, 1, texmgr.getRef("box_bottom"),
 //            { staticAnim },
 //            TileType::GRASS);
 //
 //    this->tileAtlas["grass"] =
 //        Tile(this->tileSize, 1, texmgr.getRef("grass"),
 //            { staticAnim },
 //            TileType::GRASS);
 //    tileAtlas["forest"] =
 //        Tile(this->tileSize, 1, texmgr.getRef("forest"),
 //            { staticAnim },
 //            TileType::FOREST);  
 //    tileAtlas["water"] =
 //        Tile(this->tileSize, 1, texmgr.getRef("water"),
 //            { Animation(0, 3, 0.5f),
 //            Animation(0, 3, 0.5f),
 //            Animation(0, 3, 0.5f) },
 //            TileType::WATER);
 //    tileAtlas["residential"] =
 //        Tile(this->tileSize, 2, texmgr.getRef("residential"),
 //            { staticAnim, staticAnim, staticAnim,
 //            staticAnim, staticAnim, staticAnim },
 //            TileType::RESIDENTIAL);
 //    tileAtlas["commercial"] =
 //        Tile(this->tileSize, 2, texmgr.getRef("commercial"),
 //            { staticAnim, staticAnim, staticAnim, staticAnim},
 //            TileType::COMMERCIAL);
 //    tileAtlas["industrial"] =
 //        Tile(this->tileSize, 2, texmgr.getRef("industrial"),
 //            { staticAnim, staticAnim, staticAnim,
 //            staticAnim },
 //            TileType::INDUSTRIAL);
 //    tileAtlas["road"] =
 //        Tile(this->tileSize, 1, texmgr.getRef("road"),
 //            { staticAnim, staticAnim, staticAnim,
 //            staticAnim, staticAnim, staticAnim,
 //            staticAnim, staticAnim, staticAnim,
 //            staticAnim, staticAnim },
 //            TileType::ROAD);
	
    return;
}


void Game::loadFonts()
{
    sf::Font font;
    font.loadFromFile("media/font.ttf");
    this->fonts["main_font"] = font;
 
    return;
}
 
void Game::loadStylesheets()
{
    this->stylesheets["button"] = GuiStyle(&this->fonts.at("main_font"), 1,
        sf::Color(0xc6,0xc6,0xc6), sf::Color(0x94,0x94,0x94), sf::Color(0x00,0x00,0x00),
        sf::Color(0x61,0x61,0x61), sf::Color(0x94,0x94,0x94), sf::Color(0x00,0x00,0x00));
    this->stylesheets["text"] = GuiStyle(&this->fonts.at("main_font"), 0,
        sf::Color(0x00,0x00,0x00,0x00), sf::Color(0x00,0x00,0x00), sf::Color(0xff,0xff,0xff),
        sf::Color(0x00,0x00,0x00,0x00), sf::Color(0x00,0x00,0x00), sf::Color(0xff,0x00,0x00));
 
    return;
}


