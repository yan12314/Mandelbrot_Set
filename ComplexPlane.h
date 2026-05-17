#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;

const unsigned int MAX_ITER = 64;
const float BASE_WIDTH = 4.0;
const float BASE_HEIGHT = 4.0;
const float BASE_ZOOM = 0.5;

enum class State { CALCULATING, DISPLAYING };

class ComplexPlane : public Drawable
{
private:
    VertexArray m_vArray;
    State m_State;
    Vector2f m_mouseLocation;
    Vector2i m_pixel_size;
    Vector2f m_plane_center;
    Vector2f m_plane_size;
    int m_zoomCount;
    float m_aspectRatio;

    void draw(RenderTarget& target, RenderStates states) const;
    size_t countIterations(Vector2f coord);
    void iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b);
    Vector2f mapPixelToCoords(Vector2i mousePixel);
public:
    ComplexPlane(int pixelWidth, int pixelHeight);
    void zoomIn(Vector2i mousePixel);
    void zoomOut(Vector2i mousePixel);
    void setCenter(Vector2i mousePixel);
    void setMouseLocation(Vector2i mousePixel);
    void loadText(Text& text);
    void updateRender();
};