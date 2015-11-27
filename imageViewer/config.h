#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>

struct Config {
	sf::RenderWindow window;
	float size = 1.0;
	std::string addresPlusMinus = "image/plmi.png";
	std::string addresLeftRight = "image/lr.png";
	sf::Image image;
	sf::Texture plusT;
	sf::Sprite plus;
	sf::Texture minusT;
	sf::Sprite minus;
	sf::Texture leftMoveT;
	sf::Sprite leftMove;
	sf::Texture rightMoveT;
	sf::Sprite rightMove;

	sf::Image img;
	sf::Texture imgTexture;
	sf::Sprite imgSprite;
	std::vector<std::string>::iterator itImg;
	bool isNeedNewImg = true;
	bool isNeedRedrawImg = true;
	bool isMousePressedInImg = false;
	bool mouseMove = false;
	bool moveImg = false;
	int deltaXMouseImg = 0;
	int deltaYMouseImg = 0;
	int imgPositionX;
	bool isHaveImg = true;
	int imgPositionY;
};