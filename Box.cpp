#include "Box.h"

namespace GEII
{
    Box::Box(float width, float height, float depth, BoxUVPattern uvPattern) : Shape(GL_TRIANGLES), mWidth(width), mHeight(height), mDepth(depth), mUVPattern(uvPattern)
    {
        ENGINE_LOG_INFO("Creating Cube : (", mWidth, ", ", mHeight, ", ", mDepth, ").");
        float x = width;
        float y = height;
        float z = depth;

        std::vector<float> verticeList = {
            /* Face Avant*/   -x,-y, z,      x,-y, z,       x, y, z,          -x, y, z,
            /* Face Droite*/   x,-y, z,      x,-y,-z,       x, y,-z,           x, y, z,
            /* Face Arrière*/  x,-y,-z,     -x,-y,-z,      -x, y,-z,           x, y,-z,
            /* Face Gauche*/  -x,-y,-z,     -x,-y, z,      -x, y, z,          -x, y,-z,
            /* Face Up*/      -x, y, z,      x, y, z,       x, y,-z,          -x, y,-z,
            /* Face Bas*/      x,-y, z,     -x,-y, z,      -x,-y,-z,           x,-y,-z
        };

        std::vector<float> normalsList = {
            /* Face Avant*/    0, 0, 1,      0, 0, 1,       0, 0, 1,           0, 0, 1,
            /* Face Droite*/   1, 0, 0,      1, 0, 0,       1, 0, 0,           1, 0, 0,
            /* Face Arrière*/  0, 0,-1,      0, 0,-1,       0, 0,-1,           0, 0,-1,
            /* Face Gauche*/  -1, 0, 0,     -1, 0, 0,      -1, 0, 0,          -1, 0, 0,
            /* Face Up*/       0, 1, 0,      0, 1, 0,       0, 1, 0,           0, 1, 0,
            /* Face Bas*/      0,-1, 0,      0,-1, 0,       0,-1, 0,           0,-1, 0
        };

        std::vector<float> textureUVList(48);
        switch(uvPattern)
        {
            case BoxUVPattern::FullFaces :
                textureUVList = {
                    /* Face Avant*/    0, 0,     1, 0,     1, 1,     0, 1,
                    /* Face Droite*/   0, 0,     1, 0,     1, 1,     0, 1,
                    /* Face Arrière*/  0, 0,     1, 0,     1, 1,     0, 1,
                    /* Face Gauche*/   0, 0,     1, 0,     1, 1,     0, 1,
                    /* Face Up*/       0, 0,     1, 0,     1, 1,     0, 1,
                    /* Face Bas*/      0, 0,     1, 0,     1, 1,     0, 1
                };
                break;
            case BoxUVPattern::Skybox :
                {
                    constexpr float bias = 0.001f;
                    textureUVList = {
                        /* Face Avant*/    1.0f,(1.0f/3.0f)+bias,   0.75f, (1.0f/3.0f)+bias,    0.75f,(2.0f/3.0f)-bias,     1.0f,(2.0f/3.0f)-bias,
                        /* Face Droite*/   0.75f,(1.0f/3.0f)+bias,  0.5f,(1.0f/3.0f)+bias,      0.5f, (2.0f/3.0f)-bias,     0.75f, (2.0f/3.0f)-bias,
                        /* Face Arrière*/  0.5f,(1.0f/3.0f),        0.25f, (1.0f/3.0f),         0.25f,(2.0f/3.0f),          0.5f,(2.0f/3.0f),
                        /* Face Gauche*/   0.25f,(1.0f/3.0f)+bias,  0.0f, (1.0f/3.0f)+bias,     0.0f, (2.0f/3.0f)-bias,     0.25f,(2.0f/3.0f)-bias,
                        /* Face Up*/       0.25f+bias,1.0f,         0.5f-bias,1.0f,             0.5f-bias, (2.0f/3.0f),     0.25f+bias, (2.0f/3.0f),
                        /* Face Bas*/      0.5f -bias,0.0f,         0.25f+bias, 0.0f,           0.25f+bias, (1.0f/3.0f),    0.5f-bias,(1.0f/3.0f),
                    };
                }
                break;
            default :
                break;
        }


        std::vector<unsigned int> indicesList = {
            1, 0, 3,     3, 2, 1,
            5, 4, 7,     7, 6, 5,
            9, 8,11,    11,10, 9,
           13,12,15,    15,14,13,
           17,16,19,    19,18,17,
           21,20,23,    23,22,21,
        };

        load(verticeList, textureUVList, normalsList, indicesList);
        ENGINE_LOG_SUCCESS("Cube : (", mWidth, ", ", mHeight, ", ", mDepth, ") created.");
    }

    Box::Box(float bSize, BoxUVPattern uvPattern) : Box(bSize, bSize, bSize, uvPattern) { }

    Box::~Box(void) { }
}
