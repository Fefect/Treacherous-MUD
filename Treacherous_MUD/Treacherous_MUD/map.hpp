
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
	unsigned int layers;
 
    std::vector<std::vector<Tile>> tiles;
 
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
 
    /* Save map to disk ~ obsolete for now*/
    void save(const std::string& filename);
 
    /* Draw the map */  
    void draw(sf::RenderWindow& window, float dt);

	void loadJSON(std::map<std::string, Tile>& tileAtlas);

	void drawObjects(sf::RenderWindow& window, float dt);
 
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
		this->layers = 0;
	    loadJSON(tileAtlas);
	}
};
 
#endif /* MAP_HPP */
