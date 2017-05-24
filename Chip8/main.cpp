#include "SFML/Window.hpp"
#include "Chip8.h"

int main()
{
	Chip8 chip8 = Chip8();

	sf::Window window(sf::VideoMode(800, 600), "Chip8");

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}
	return 0;
}
