
#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP
 
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
 
class TextureManager
{
    private:
 
    /* Array of textures used */
    std::map<std::string, sf::Texture> textures;
	std::map<std::string, sf::Sprite> static_textures;
	std::map<std::string, sf::Texture> texture1;
 
    public:

	sf::Sprite& getStaticRef(const std::string& texture);

	int spriteSheetHeight;
	int spriteSheetWidth;

	void placeStaticTexture();
    /* Add a texture from a file */
    void loadTexture(const std::string& name, const std::string &filename);
 
    /* Translate an id into a reference */
    sf::Texture& getRef(const std::string& texture);
 
    /* Constructor */
    TextureManager()
    {
    }
};
 
#endif /* TEXTURE_MANAGER_HPP */
