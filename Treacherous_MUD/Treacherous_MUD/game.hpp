#ifndef GAME_HPP
#define GAME_HPP
 
#include <stack>
#include <SFML/Graphics.hpp>
#include "texture_manager.hpp"
#include "tile_data.hpp"
#include "gui.hpp"

class GameState;
 
class Game
{
    public:

	const static int tileSize = 64;
 
	std::stack<GameState*> states;
	std::map<std::string, Tile> tileAtlas;
	std::map<std::string, GuiStyle> stylesheets;
	std::map<std::string, sf::Font> fonts;
 
    sf::RenderWindow window;
    TextureManager texmgr;
    sf::Sprite background;


	void loadSpriteSheet();
    void pushState(GameState* state);
    void popState();
    void changeState(GameState* state);
    GameState* peekState();
 
    void gameLoop();
 
    Game();
    ~Game();
	private:
	void loadTextures();
	void loadTiles();
	void loadStylesheets();
	void loadFonts();
};
 
#endif /* GAME_HPP */
