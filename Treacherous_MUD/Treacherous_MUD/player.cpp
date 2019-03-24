#include <iostream>
#include "utils.cpp"
#include"SFML\Graphics.hpp"
#include"SFML\Window.hpp"
#include"SFML\System.hpp"

class Player 
{ 
    public: 
		Position pos;
	sf::RectangleShape selection;
		
      
    //Default Constructor 
    Player() 
    {
	    std::cout << "Default Constructor called" << std::endl;  
        pos.x = 0;
		pos.y = 0;
		selection.setSize(sf::Vector2f(32,32));
		selection.setOutlineThickness(2);
		selection.setOutlineColor(sf::Color::Magenta);
		selection.setFillColor(sf::Color::Transparent);
		selection.setPosition(pos.x * 32, pos.y * 32);
    } 
      
    //Parametrized Constructor 
    Player(int x, int y) 
    {
	    std::cout << "Parametrized Constructor called" << std::endl; 
        pos.x = x;
		pos.y = y;
    } 
}; 