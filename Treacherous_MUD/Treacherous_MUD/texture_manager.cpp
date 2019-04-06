
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
 
#include "texture_manager.hpp"
 
void TextureManager::loadTexture(const std::string& name, const std::string& filename) {
	/* Create the texture in the std::map, then load the texture */
	textures.emplace(name, sf::Texture());
	this->textures[name].loadFromFile(filename);

	return;
}

void TextureManager::placeStaticTexture()
{
	sf::Texture textureMap;
	texture1.emplace("tilemap",sf::Texture());
	this->texture1["tilemap"].loadFromFile("media/spritesheetMUD.png");
	this->spriteSheetHeight = this->texture1["tilemap"].getSize().y / 64;
	this->spriteSheetWidth = this->texture1["tilemap"].getSize().x / 128;
	for(int i = 0; i < this->spriteSheetHeight; i++)
	{
		for(int b = 0; b < this->spriteSheetWidth; b++)
		{
			sf::Sprite sprite;
			sprite.setTexture(this->texture1["tilemap"]);
			sprite.setTextureRect(sf::IntRect(b*128,i*64,128,64));
			this->static_textures.emplace_back(sprite);
		}
	}
}

sf::Sprite& TextureManager::getStaticRef(const int id)
{
	return this->static_textures.at(id);
}
 
sf::Texture& TextureManager::getRef(const std::string& texture)
{
    return this->textures.at(texture);
}
