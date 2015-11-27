#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <Windows.h>
#include <list>
#include <atlbase.h>
#include "config.h"

std::vector<std::string> readFiles(std::string & imageDirectoryAddres) {
	std::wstring imageDirectoryAddresInWstring(imageDirectoryAddres.begin(), imageDirectoryAddres.end());
	std::vector<std::string> nameImages;
	std::wstring searchPath = imageDirectoryAddresInWstring + _T("\\*.*");
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(searchPath.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				std::wstring nameInWstring(fd.cFileName);
				std::string nameInString(nameInWstring.begin(), nameInWstring.end());
				nameImages.push_back(nameInString);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return nameImages;
}


void redrawingImage(Config & conf) {
	conf.window.clear(sf::Color(255, 255, 255));
	if (!conf.moveImg) {
		conf.imgPositionX = (int)((conf.window.getSize().x / 2) - (conf.imgSprite.getTextureRect().width * conf.imgSprite.getScale().x / 2));
		conf.imgPositionY = (int)((conf.window.getSize().y / 2) - (conf.imgSprite.getTextureRect().height * conf.imgSprite.getScale().y / 2));
	} else {
		if ((sf::Mouse::getPosition(conf.window).x - conf.deltaXMouseImg) > (int)(conf.window.getSize().x / 2)) {
			conf.imgPositionX = (int)(conf.window.getSize().x / 2);
		} else if ((sf::Mouse::getPosition(conf.window).x - conf.deltaXMouseImg + conf.imgSprite.getTextureRect().width * conf.imgSprite.getScale().x) < (int)(conf.window.getSize().x / 2)) {
			conf.imgPositionX = (int)(conf.window.getSize().x / 2 - conf.imgSprite.getTextureRect().width * conf.imgSprite.getScale().x);
		} else {
			conf.imgPositionX = sf::Mouse::getPosition(conf.window).x - conf.deltaXMouseImg;
		}
		if ((sf::Mouse::getPosition(conf.window).y - conf.deltaYMouseImg) > (int)(conf.window.getSize().y / 2)) {
			conf.imgPositionY = (int)(conf.window.getSize().y / 2);
		} else if ((sf::Mouse::getPosition(conf.window).y - conf.deltaYMouseImg + conf.imgSprite.getTextureRect().height * conf.imgSprite.getScale().y) < (int)(conf.window.getSize().y / 2)) {
			conf.imgPositionY = (int)(conf.window.getSize().y / 2 - conf.imgSprite.getTextureRect().height  * conf.imgSprite.getScale().y);
		} else {
			conf.imgPositionY = sf::Mouse::getPosition(conf.window).y - conf.deltaYMouseImg;
		}

	}
	conf.imgSprite.setPosition((float)conf.imgPositionX, (float)conf.imgPositionY);
	conf.isNeedRedrawImg = false;
	conf.window.draw(conf.imgSprite);
	conf.window.draw(conf.leftMove);
	conf.window.draw(conf.rightMove);
	conf.window.draw(conf.plus);
	conf.window.draw(conf.minus);
	conf.window.display();
}


void drawNewImage(Config & conf, std::string & imageDirectoryAddresInString) {
	if (conf.img.loadFromFile(imageDirectoryAddresInString + "\\" + *conf.itImg)) {
		conf.window.setTitle(imageDirectoryAddresInString + "\\" + *conf.itImg);
		conf.imgTexture.loadFromImage(conf.img);
		conf.imgSprite.setTexture(conf.imgTexture, 1);
		conf.isNeedNewImg = false;
	} else {
		conf.isNeedNewImg = false;
		conf.isNeedRedrawImg = false;
		conf.window.setTitle("File can't load.");
	}
}


void drawImage(Config & conf, std::string & imageDirectoryAddresInString) {
	if (conf.isNeedNewImg) {
		drawNewImage(conf, imageDirectoryAddresInString);
	}
	if (conf.isNeedRedrawImg) {
		redrawingImage(conf);
	}
}


bool isSpritesIntersectedWithMous(sf::Sprite & sprite, Config & conf) {
	int posMousX = sf::Mouse::getPosition(conf.window).x;
	int posMousY = sf::Mouse::getPosition(conf.window).y;
	if ((((posMousX > sprite.getPosition().x) && (posMousX < sprite.getPosition().x + abs(sprite.getTextureRect().width)))
		&& ((posMousY > sprite.getPosition().y) && (posMousY < sprite.getPosition().y + abs(sprite.getTextureRect().height))))) {
		return true;
	}
	return false;
}


void previousImage(Config & conf) {
	conf.itImg--;
	conf.isNeedNewImg = true;
	conf.isNeedRedrawImg = true;
}


void nextImage(Config & conf) {
	conf.itImg++;
	conf.isNeedNewImg = true;
	conf.isNeedRedrawImg = true;
}


void zoomUp(Config & conf) {
	conf.minus.setColor(sf::Color::White);
	if (conf.size < 5 && conf.size >= 1) {
		conf.size = conf.size + 0.5f;
		if (conf.size == 5) {
			conf.plus.setColor(sf::Color::Transparent);
		}
	}
	if (conf.size < 1) {
		conf.size = conf.size + 0.1f;
	}
	conf.imgSprite.setScale(conf.size, conf.size);
	conf.isNeedRedrawImg = true;
}


void zoomDown(Config & conf) {
	conf.plus.setColor(sf::Color::White);
	if (conf.size >(0.2) && conf.size <= 1) {
		conf.size = conf.size - 0.1f;
		if (conf.size < (0.2)) {
			conf.minus.setColor(sf::Color::Transparent);
		}
	}
	if (conf.size > 1) {
		conf.size = conf.size - 0.5f;
	}
	conf.imgSprite.setScale(conf.size, conf.size);
	conf.isNeedRedrawImg = true;
}


void eventProcessing(Config & conf, std::vector<std::string> & nameImage) {
	sf::Event event;
	while (conf.window.pollEvent(event)) {
		if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
			conf.window.close();
		if (event.type == sf::Event::Resized || event.type == sf::Event::GainedFocus) {
			conf.isNeedRedrawImg = true;
			conf.window.setView(sf::View(sf::FloatRect(0.0f, 0.0f, (float)conf.window.getSize().x, (float)conf.window.getSize().y)));
		}

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left && conf.itImg != nameImage.begin())
			previousImage(conf);
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right && conf.itImg != --nameImage.end())
			nextImage(conf);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up))
			zoomUp(conf);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down))
			zoomDown(conf);

		if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left) {
			if (isSpritesIntersectedWithMous(conf.imgSprite, conf)) {
				conf.isMousePressedInImg = true;
				conf.deltaXMouseImg = sf::Mouse::getPosition(conf.window).x - (int)(conf.imgSprite.getPosition().x);
				conf.deltaYMouseImg = sf::Mouse::getPosition(conf.window).y - (int)(conf.imgSprite.getPosition().y);
			}
			if (isSpritesIntersectedWithMous(conf.leftMove, conf) && conf.itImg != nameImage.begin()) 
				previousImage(conf);
			if (isSpritesIntersectedWithMous(conf.rightMove, conf) && conf.itImg != --nameImage.end()) 
				nextImage(conf);
			if (isSpritesIntersectedWithMous(conf.plus, conf)) 
				zoomUp(conf);
			if (isSpritesIntersectedWithMous(conf.minus, conf))
				zoomDown(conf);
		}
		if (conf.isMousePressedInImg) {
			if (event.type == sf::Event::MouseMoved) {
				conf.mouseMove = true;
				conf.moveImg = true;
				conf.isNeedRedrawImg = true;
			}
		}
		if (event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left) {
			conf.isMousePressedInImg = false;
			conf.mouseMove = false;
			conf.isNeedRedrawImg = false;
			conf.moveImg = false;
			conf.deltaXMouseImg = 0;
			conf.deltaYMouseImg = 0;
		}
	}
}


void createArrowAndPM(Config & conf) {
	conf.image.loadFromFile(conf.addresLeftRight);
	conf.leftMoveT.loadFromImage(conf.image);
	conf.leftMove.setTexture(conf.leftMoveT);
	conf.leftMove.setTextureRect(sf::IntRect(0, 0, 50, 50));
	conf.leftMove.setPosition(0, (float)(conf.window.getSize().y / 2) - (conf.leftMove.getTextureRect().height / 2));
	conf.rightMove.setTexture(conf.leftMoveT);
	conf.rightMove.setTextureRect(sf::IntRect(50, 0, 50, 50));
	conf.rightMove.setPosition((float)(conf.window.getSize().x - conf.rightMove.getTextureRect().width), (float)((conf.window.getSize().y / 2) - (conf.rightMove.getTextureRect().height / 2)));

	conf.image.loadFromFile(conf.addresPlusMinus);
	conf.plusT.loadFromImage(conf.image);
	conf.plus.setTexture(conf.plusT);
	conf.plus.setTextureRect(sf::IntRect(0, 0, 50, 55));
	conf.plus.setColor(sf::Color::White);
	conf.plus.setPosition((float)(conf.window.getSize().x - conf.plus.getTextureRect().width * 2), (float)(conf.window.getSize().y - conf.plus.getTextureRect().height));
	conf.minusT.loadFromImage(conf.image);
	conf.minus.setTexture(conf.plusT);
	conf.minus.setTextureRect(sf::IntRect(0, 55, 50, 55));
	conf.minus.setColor(sf::Color::White);
	conf.minus.setPosition((float)(conf.window.getSize().x - conf.minus.getTextureRect().width), (float)(conf.window.getSize().y - conf.minus.getTextureRect().height));

}


void runViewer(Config & conf, std::vector<std::string> & nameImage, std::string & imageDirectoryAddresInString) {
	conf.window.create(sf::VideoMode(800, 600), "Viewer");	
	conf.itImg = nameImage.begin();
	createArrowAndPM(conf);

	for (conf.itImg; conf.itImg != nameImage.end(); conf.itImg++) {
		if (conf.img.loadFromFile(imageDirectoryAddresInString + "\\" + *conf.itImg)) {
			conf.isHaveImg = true;
			break;
		}
		else {
			conf.isHaveImg = false;
		}
	}

	while (conf.window.isOpen()) {
		if (!conf.isHaveImg) {
			conf.window.close();
			break;
		}		
		eventProcessing(conf, nameImage);
		drawImage(conf, imageDirectoryAddresInString);
		
	}
	if (!conf.isHaveImg) {
		std::cout << std::endl << "No images." << std::endl;
	}
}


int main()
{
	Config conf;
	std::string imageDirectoryAddres;
	imageDirectoryAddres = "D:\\imageViewer";
	//std::cin >> imageDirectoryAddres;
	std::vector<std::string> nameImagesInList = readFiles(imageDirectoryAddres);
	runViewer(conf, nameImagesInList, imageDirectoryAddres);	
    return 0;
	std::vector<std::string> v;
}