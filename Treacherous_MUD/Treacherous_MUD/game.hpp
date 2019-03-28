#ifndef GAME_HPP
#define GAME_HPP
 
#include <stack>
#include <SFML/Graphics.hpp>
#include "texture_manager.hpp"
#include "Tile.hpp"

class GameState;
 
class Game
{
    public:

	const static int tileSize = 64;
 
	std::stack<GameState*> states;
	std::map<std::string, Tile> tileAtlas;
 
    sf::RenderWindow window;
    TextureManager texmgr;
    sf::Sprite background;
 
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
};
 
#endif /* GAME_HPP */
