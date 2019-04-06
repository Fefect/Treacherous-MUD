
    #include <SFML/Graphics.hpp>
     
    #include "animation_handler.hpp"
    #include "Tile.hpp"
     
    void Tile::draw(sf::RenderWindow& window, float dt)
    {
        /* Change the sprite to reflect the tile variant */
        this->animHandler.changeAnim(this->tileVariant);
     
        /* Update the animation */
        this->animHandler.update(dt);
     
        /* Update the sprite */
       // this->sprite.setTextureRect(this->animHandler.bounds);
     
        /* Draw the tile */
        window.draw(this->sprite);
     
        return;
    }
     
    void Tile::update()
    {
        
    }
     
    std::string tileTypeToStr(TileType type)
    {
        switch(type)
        {
            default:
            case TileType::VOID:            return "Void";
            case TileType::GRASS:           return "Flatten";
            case TileType::FOREST:          return "Forest";
            case TileType::WATER:           return "Water";
            case TileType::RESIDENTIAL:     return "Residential Zone";
            case TileType::COMMERCIAL:      return "Commercial Zone";
            case TileType::INDUSTRIAL:      return "Industrial Zone";
			case TileType::STATIC_WATER:	return "Static water";
        }
    }
