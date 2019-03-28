
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
 
sf::Texture& TextureManager::getRef(const std::string& texture)
{
    return this->textures.at(texture);
}
