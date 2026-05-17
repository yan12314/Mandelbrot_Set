#include <SFML/Graphics.hpp>
#include "ComplexPlane.h"

using namespace sf;
using namespace std;

int main()
{
	// screen size
	int pixelWidth = VideoMode::getDesktopMode().width / 4;
	int pixelHeight = VideoMode::getDesktopMode().height / 4;

	// create window
	RenderWindow window(VideoMode(pixelWidth, pixelHeight), "Mandelbrot Set");

	// complex plane object
	ComplexPlane plane(pixelWidth, pixelHeight);

	Font font;
	font.loadFromFile("tuffy.ttf");

	// create text
	Text text;
	text.setFont(font);
	text.setCharacterSize(22);
	text.setFillColor(Color::White);

	while (window.isOpen())
	{
		Event event;

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}
			if (event.type == Event::MouseButtonPressed)
			{
				Vector2i mousePixel;
				mousePixel.x = event.mouseButton.x;
				mousePixel.y = event.mouseButton.y;

				// left click zoom in
				if (event.mouseButton.button == Mouse::Left)
				{
					plane.zoomIn(mousePixel);
				}

				//right click zoom out
				if (event.mouseButton.button == Mouse::Right)
				{
					plane.zoomOut(mousePixel);
				}
			}

			if (event.type == Event::MouseMoved)
			{
				Vector2i mousePixel;
				mousePixel.x = event.mouseMove.x;
				mousePixel.y = event.mouseMove.y;

				plane.setMouseLocation(mousePixel);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// update scene
		plane.updateRender();
		plane.loadText(text);

		// draw scene
		window.clear();
		window.draw(plane);
		window.draw(text);
		window.display();
	}
	
	return 0;

}