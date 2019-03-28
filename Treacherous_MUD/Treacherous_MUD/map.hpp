
#ifndef MAP_HPP
#define MAP_HPP
 
#include <SFML/Graphics.hpp>
 
#include <string>
#include <map>
#include <vector>
 
#include "Tile.hpp"
#include "BFS.hpp"

class Map
{
    public:

	BFS bfs;

	int area[64][64];

    unsigned int width;
    unsigned int height;
 
    std::vector<Tile> tiles;
 
    /* Resource map */
    std::vector<int> resources;
 
    unsigned int tileSize;
 
    unsigned int numSelected;
 
    unsigned int numRegions[1];

 
	/* Select the tiles within the bounds */
	void select(sf::Vector2i location, std::vector<TileType> blacklist);
 
	/* Deselect all tiles */

	//clears the path
	void clearPath();

    /* Load map from disk */
    void load(const std::string& filename, unsigned int width, unsigned int height,
        std::map<std::string, Tile>& tileAtlas);
 
    /* Save map to disk */
    void save(const std::string& filename);
 
    /* Draw the map */  
    void draw(sf::RenderWindow& window, float dt);
 
    /* Blank map constructor */
    Map()
    {
	    this->numSelected = 0;
	    this->tileSize = 64;
	    this->width = 0;
	    this->height = 0;
	    this->numRegions[0] = 1;
    }
    /* Load map from file constructor */
	Map(const std::string& filename, unsigned int width, unsigned int height,
	    std::map<std::string, Tile>& tileAtlas)
	{
	    this->numSelected = 0;
	    this->tileSize = 64;
	    load(filename, width, height, tileAtlas);
	}
};
 
#endif /* MAP_HPP */
