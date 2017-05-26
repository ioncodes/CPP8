#include "SFML/Window.hpp"
#include <SFML/Graphics.hpp>
#include "Chip8.h"
#include <chrono>
#include <thread>
#include <iostream>

int main(int argc, char* argv[])
{
	Chip8 chip8 = Chip8();
	chip8.LoadRom(argv[1]);

	sf::RenderWindow window(sf::VideoMode(1024, 512), "Chip8");

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		chip8.Cycle();

		std::vector<sf::Sprite> sprites;

		for (int x = 0; x < 64; x++)
		{
			for (int y = 0; y < 32; y++)
			{
				if (chip8.screen()[x][y] == 1)
				{
					sf::Texture texture;
					texture.create(16, 16);
					sf::Sprite sprite(texture);
					sprite.setPosition(x * 16, y * 16);
					sprite.setColor(sf::Color::White);
					sprites.push_back(sprite);
				}
			}
		}
		window.clear(sf::Color::Black);
		for (int i = 0; i < sprites.size(); i++)
		{
			window.draw(sprites.at(i));
		}
		window.display();
		std::this_thread::sleep_for(std::chrono::microseconds(1200));
	}


	return 0;
}
