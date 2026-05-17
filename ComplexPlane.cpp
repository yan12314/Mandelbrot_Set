#include "ComplexPlane.h"
#include <cmath>
#include <sstream>
#include <complex>

using namespace std;

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
{
	//save window size
	m_pixel_size.x = pixelWidth;
	m_pixel_size.y = pixelHeight;

	//calculate screen ratio
	m_aspectRatio = (float)pixelHeight / pixelWidth;

	//starting point
	m_plane_center.x = 0;
	m_plane_center.y = 0;

	//start complex plane
	m_plane_size.x = BASE_WIDTH;
	m_plane_size.y = BASE_HEIGHT * m_aspectRatio;

	m_zoomCount = 0;

	m_State = State::CALCULATING;

	//prepare VertexArray
	m_vArray.setPrimitiveType(Points);
	m_vArray.resize(pixelWidth * pixelHeight);
}

// draw vertex array
void ComplexPlane::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(m_vArray);
}

void ComplexPlane::updateRender()
{
	if (m_State == State::CALCULATING)
	{
		// go through every pixel
		for (int i = 0; i < m_pixel_size.y; i++)
		{
			for (int j = 0; j < m_pixel_size.x; j++)
			{
				//convert 2D pixel to 1D array index
				int index = j + i * m_pixel_size.x;

				// set pixel position on screen
				m_vArray[index].position = Vector2f((float)j, (float)i);

				Vector2i pixel;
				pixel.x = j;
				pixel.y = i;

				Vector2f coord = mapPixelToCoords(pixel);
				size_t count = countIterations(coord);

				Uint8 r;
				Uint8 g;
				Uint8 b;

				//set pixel color
				iterationsToRGB(count, r, g, b);
				m_vArray[index].color = Color(r, g, b);
			}
		}
		m_State = State::DISPLAYING;
	}
}

void ComplexPlane::zoomIn(Vector2i mousePixel)
{
	//coordinate under mouse before zoom
	Vector2f beforeZoom = mapPixelToCoords(mousePixel);

	m_zoomCount++;

	// make plane smaller
	m_plane_size.x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size.y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);

	// coordinate under mouse after zoom
	Vector2f afterZoom = mapPixelToCoords(mousePixel);

	// move center
	m_plane_center.x += beforeZoom.x - afterZoom.x;
	m_plane_center.y += beforeZoom.y - afterZoom.y;

	m_State = State::CALCULATING;
}

void ComplexPlane::zoomOut(Vector2i mousePixel)
{
	// coordinate under mouse before zoom
	Vector2f beforeZoom = mapPixelToCoords(mousePixel);

	m_zoomCount--;

	// make plane bigger
	m_plane_size.x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size.y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);

	// coordinate under mouse after zoom
	Vector2f afterZoom = mapPixelToCoords(mousePixel);

	// move center
	m_plane_center.x += beforeZoom.x - afterZoom.x;
	m_plane_center.y += beforeZoom.y - afterZoom.y;

	m_State = State::CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel)
{
	// coordinate in the complex plane
	Vector2f coord = mapPixelToCoords(mousePixel);
	m_plane_center = coord;
	m_State = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousePixel)
{
	// convert mouse pixel to complex coordinate
	Vector2f coord = mapPixelToCoords(mousePixel);

	// save mouse coordinate
	m_mouseLocation = coord;
}

void ComplexPlane::loadText(Text& text)
{
	// create text for screen
	stringstream textInfo;

	textInfo << "Mandelbrot Set" << endl;
	textInfo << "Center: (" << m_plane_center.x << ", " << m_plane_center.y << ")" << endl;
	textInfo << "Cursor: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")" << endl;
	textInfo << "Left-click to Zoom in" << endl;
	textInfo << "Right-click to Zoom out" << endl;
	text.setString(textInfo.str());
}

size_t ComplexPlane::countIterations(Vector2f coord)
{
	complex<double> c(coord.x, coord.y);
	complex<double> z(0, 0);

	for (size_t count = 0; count < MAX_ITER; count++)
	{
		// if z is too far it escapes
		if (abs(z) > 2.0)
		{
			return count;
		}

		// Mandelbrot formula
		z = z * z + c;
	}

	return MAX_ITER;
}

void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
	// black inside Mandelbrot
	if (count == MAX_ITER)
	{
		r = 0;
		g = 0;
		b = 0;
	}
	else
	{
		int color = count % 32;

		if (color < 5)
		{
			r = 0;
			g = 0;
			b = 120 + color * 25;
		}
		else if (color < 10)
		{
			r = 0;
			g = (color - 5) * 50;
			b = 255;
		}
		else if (color < 15)
		{
			r = 0;
			g = 255;
			b = 255 - (color - 10) * 50;
		}
		else if (color < 20)
		{
			r = (color - 15) * 50;
			g = 255;
			b = 0;
		}
		else if (color < 25)
		{
			r = 255;
			g = 255 - (color - 20) * 50;
			b = 0;
		}
		else
		{
			r = 255;
			g = 0;
			b = (color - 25) * 35;
		}
	}
}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel)
{
	// complex plane range
	float realMin = m_plane_center.x - m_plane_size.x / 2.0;
	float realMax = m_plane_center.x + m_plane_size.x / 2.0;

	float imagMin = m_plane_center.y - m_plane_size.y / 2.0;
	float imagMax = m_plane_center.y + m_plane_size.y / 2.0;

	// create coordinate
	Vector2f coord;

	// map x pixel to real coordinate
	coord.x = ((float)mousePixel.x - 0) / (m_pixel_size.x - 0) * (realMax - realMin) + realMin;

	// map y pixel to imaginary coordinate
	coord.y = ((float)mousePixel.y - m_pixel_size.y) / (0 - m_pixel_size.y) * (imagMax - imagMin) + imagMin;

	return coord;
}