
#ifndef TILE_HPP
#define TILE_HPP
 
#include <SFML/Graphics.hpp>
#include <vector>
 
#include "animation_handler.hpp"
 
enum class TileType { VOID, GRASS, FOREST, WATER, RESIDENTIAL, COMMERCIAL, INDUSTRIAL, ROAD, STATIC_WATER };
 
std::string tileTypeToStr(TileType type);
 
class Tile
{
    public:
 
    AnimationHandler animHandler;
    sf::Sprite sprite;
 
    TileType tileType;

	int corX;
	int corY;
 
    /* Tile variant, allowing for different looking versions of the
     * same tile */
    int tileVariant;
 
    /* Region IDs of the tile, tiles in the same region are connected.
     * First is for transport */
    unsigned int regions[1];
 
    /* Constructor */
    Tile() { }
    Tile(const unsigned int tileSize, const unsigned int height, sf::Texture& texture,
        const std::vector<Animation>& animations,
        const TileType tileType)
    {
        this->tileType = tileType;
        this->tileVariant = 0;
        this->regions[0] = 0;

		this->corX = 0;
		this->corY = 0;
 
        this->sprite.setOrigin(sf::Vector2f(0.0f, tileSize*(height-1)));
        this->sprite.setTexture(texture);
        this->animHandler.frameSize = sf::IntRect(0, 0, tileSize*2, tileSize*height);
        for(auto animation : animations)
        {
            this->animHandler.addAnim(animation);
        }
        this->animHandler.update(0.0f);
    }
 
    void draw(sf::RenderWindow& window, float dt);
 
    void update();
 
};
 
#endif /* TILE_HPP */
