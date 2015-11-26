#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <Windows.h>
#include <list>
#include <atlbase.h>
#include "config.h"

std::list<std::string> readFiles(std::string & imageDirectoryAddres) {
	std::wstring imageDirectoryAddresInWstring(imageDirectoryAddres.begin(), imageDirectoryAddres.end());
	std::list<std::string> nameImages;
	//char searchPath[200];
	std::wstring searchPath = imageDirectoryAddresInWstring + _T("\\*.*");
	//sprintf(searchPath, "%s/*.*", imageDirectoryAddres.c_str());
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


void eventProcessing(Config & conf, std::list<std::string> & nameImage) {
	sf::Event event;
	while (conf.window.pollEvent(event)) {
		if (event.type == sf::Event::Closed ||
			(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
			conf.window.close();
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left && conf.itImg != nameImage.begin()) {
			conf.itImg--;
			conf.isNeedNewImg = true;
			conf.isNeedRedrawImg = true;
		}
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right && conf.itImg != --nameImage.end()) {
			conf.itImg++;
			conf.isNeedNewImg = true;
			conf.isNeedRedrawImg = true;
		}
		if (event.type == sf::Event::Resized || event.type == sf::Event::GainedFocus) {
			conf.isNeedRedrawImg = true;
			conf.window.setView(sf::View(sf::FloatRect(0.0f, 0.0f, (float)conf.window.getSize().x, (float)conf.window.getSize().y)));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)) {
			conf.minus.setColor(sf::Color::White);
			if (conf.size < 5 && conf.size >= 1) {
				conf.size = conf.size + (float)(0.5);
				if (conf.size == 5) {
					conf.plus.setColor(sf::Color::Transparent);
				}
			}
			if (conf.size < 1) {
				conf.size = conf.size + (float)(0.1);
			}
			conf.imgSprite.setScale(conf.size, conf.size);
			conf.isNeedRedrawImg = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)) {
			conf.plus.setColor(sf::Color::White);
			if (conf.size >(0.2) && conf.size <= 1) {
				conf.size = conf.size - (float)(0.1);
				if (conf.size < (0.2)) {
					conf.minus.setColor(sf::Color::Transparent);
				}
			}
			if (conf.size > 1) {
				conf.size = conf.size - (float)(0.5);
			}
			conf.imgSprite.setScale(conf.size, conf.size);
			conf.isNeedRedrawImg = true;
		}
		if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left) {
			sf::RectangleShape areaImg;
			areaImg.setTextureRect(sf::IntRect((int)(conf.imgSprite.getPosition().x), (int)(conf.imgSprite.getPosition().y), conf.imgSprite.getTextureRect().width * (int)(conf.size), conf.imgSprite.getTextureRect().height * (int)(conf.size)));
			if (areaImg.getTextureRect().contains(sf::Mouse::getPosition(conf.window))) {
				conf.isMousePressedInImg = true;
				conf.deltaXMouseImg = sf::Mouse::getPosition(conf.window).x - (int)(conf.imgSprite.getPosition().x);
				conf.deltaYMouseImg = sf::Mouse::getPosition(conf.window).y - (int)(conf.imgSprite.getPosition().y);
			}
			areaImg.setTextureRect(sf::IntRect((int)(conf.leftMove.getPosition().x), (int)(conf.leftMove.getPosition().y), conf.leftMove.getTextureRect().width, conf.leftMove.getTextureRect().height));
			if (areaImg.getTextureRect().contains(sf::Mouse::getPosition(conf.window)) && conf.itImg != nameImage.begin()) {
				conf.itImg--;
				conf.isNeedNewImg = true;
				conf.isNeedRedrawImg = true;
			}
			areaImg.setTextureRect(sf::IntRect((int)(conf.rightMove.getPosition().x), (int)(conf.rightMove.getPosition().y), conf.rightMove.getTextureRect().width, conf.rightMove.getTextureRect().height));
			if (areaImg.getTextureRect().contains(sf::Mouse::getPosition(conf.window)) && conf.itImg != --nameImage.end()) {
				conf.itImg++;
				conf.isNeedNewImg = true;
				conf.isNeedRedrawImg = true;
			}
			areaImg.setTextureRect(sf::IntRect((int)(conf.plus.getPosition().x), (int)(conf.plus.getPosition().y), conf.plus.getTextureRect().width, conf.plus.getTextureRect().height));
			if (areaImg.getTextureRect().contains(sf::Mouse::getPosition(conf.window))) {
				conf.minus.setColor(sf::Color::White);
				if (conf.size < 5 && conf.size >= 1) {
					conf.size = conf.size + (float)(0.5);
					if (conf.size == 5) {
						conf.plus.setColor(sf::Color::Transparent);
					}
				}
				if (conf.size < 1) {
					conf.size = conf.size + (float)(0.1);
				}
				conf.imgSprite.setScale(conf.size, conf.size);
				conf.isNeedRedrawImg = true;
			}
			areaImg.setTextureRect(sf::IntRect((int)(conf.minus.getPosition().x), (int)(conf.minus.getPosition().y), conf.minus.getTextureRect().width, conf.minus.getTextureRect().height));
			if (areaImg.getTextureRect().contains(sf::Mouse::getPosition(conf.window))) {
				conf.plus.setColor(sf::Color::White);
				if (conf.size >(0.2) && conf.size <= 1) {
					conf.size = conf.size - (float)(0.1);
					std::cout << conf.size;
					if (conf.size < (0.2)) {
						conf.minus.setColor(sf::Color::Transparent);
					}
				}
				if (conf.size > 1) {
					conf.size = conf.size - (float)(0.5);
				}
				conf.imgSprite.setScale(conf.size, conf.size);
				conf.isNeedRedrawImg = true;
			}
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


void drawImage(Config & conf, std::string & imageDirectoryAddresInString) {
	if (conf.isNeedNewImg) {
		if (conf.img.loadFromFile(imageDirectoryAddresInString + "\\" + *conf.itImg)) {
			conf.window.setTitle(imageDirectoryAddresInString + "\\" + *conf.itImg);
			conf.imgTexture.loadFromImage(conf.img);
			conf.imgSprite.setTexture(conf.imgTexture, 1);
			conf.isNeedNewImg = false;
		} 
		else {
			conf.isNeedNewImg = false;
			conf.window.setTitle("File can't load.");
		}
	}
	if (conf.isNeedRedrawImg) {
		conf.window.clear(sf::Color(255, 255, 255));
		if (!conf.moveImg) {
			conf.imgPositionX = (int)((conf.window.getSize().x / 2) - (conf.imgSprite.getTextureRect().width * conf.imgSprite.getScale().x / 2));
			conf.imgPositionY = (int)((conf.window.getSize().y / 2) - (conf.imgSprite.getTextureRect().height * conf.imgSprite.getScale().y / 2));
		} 
		else {
			if ((sf::Mouse::getPosition(conf.window).x - conf.deltaXMouseImg) > (int)(conf.window.getSize().x / 2)) {
				conf.imgPositionX = (int)(conf.window.getSize().x / 2);
			}
			else if ((sf::Mouse::getPosition(conf.window).x - conf.deltaXMouseImg + conf.imgSprite.getTextureRect().width * conf.imgSprite.getScale().x) < (int)(conf.window.getSize().x / 2)) {
				conf.imgPositionX = (int)(conf.window.getSize().x / 2 - conf.imgSprite.getTextureRect().width * conf.imgSprite.getScale().x);
			}
			else {
				conf.imgPositionX = sf::Mouse::getPosition(conf.window).x - conf.deltaXMouseImg;
			}
			if ((sf::Mouse::getPosition(conf.window).y - conf.deltaYMouseImg) > (int)(conf.window.getSize().y / 2)) {
				conf.imgPositionY = (int)(conf.window.getSize().y / 2);
			} 
			else if ((sf::Mouse::getPosition(conf.window).y - conf.deltaYMouseImg + conf.imgSprite.getTextureRect().height * conf.imgSprite.getScale().y) < (int)(conf.window.getSize().y / 2)) {
				conf.imgPositionY = (int)(conf.window.getSize().y / 2 - conf.imgSprite.getTextureRect().height  * conf.imgSprite.getScale().y);
			}
			else {
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
}


void runViewer(Config & conf, std::list<std::string> & nameImage, std::string & imageDirectoryAddresInString) {
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
		system("pause");
	}
}


int main()
{
	Config conf;
	std::string imageDirectoryAddres;
	//imageDirectoryAddres = "D:\\imageViewer";
	std::cin >> imageDirectoryAddres;
	std::list<std::string> nameImagesInList = readFiles(imageDirectoryAddres);
	runViewer(conf, nameImagesInList, imageDirectoryAddres);
	
    return 0;
}

