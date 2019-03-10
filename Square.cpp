#include "Square.h"

namespace GEII
{
    Square::Square(float width, float height) : Shape(GL_TRIANGLES), mWidth(width), mHeight(height)
    {
        float x = width;
        float y = height;
        constexpr float z = 0.0f;

        std::vector<float> verticeList = {      -x,-y,z,            x,-y,z,             x,y,z,              -x,y,z};
        std::vector<float> normalsList = {      0.0f,0.0f,1.0f,     0.0f,0.0f,1.0f,     0.0f,0.0f,1.0f,     0.0f,0.0f,1.0f };
        std::vector<float> textureUVList = {    0,0,                1,0,                1,1,                0,1 };
        std::vector<unsigned int> indicesList = { 1,0,3,     3,2,1 };

        load(verticeList, textureUVList, normalsList, indicesList);
    }

    Square::~Square(void){ }
}
