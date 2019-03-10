#ifndef LIGHT_H
#define LIGHT_H

/*!
* \file Light.h
* \author Alexandre Brunet
* \date 18/02/2019
*/

#include "geHeader.h"
#include "Transform.h"
#include "OGL_FrameBuffer.h"

#define LIGHT_FBO_SIZE 4096

namespace GEII
{
    struct Light{
        Light(void) : frameBuffer(LIGHT_FBO_SIZE,LIGHT_FBO_SIZE) { }
        glm::vec3 ambiantColor;
        glm::vec3 power;
        LightType type;
        float angle;
        Transform_sptr transform = std::make_shared<Transform>();
        Texture_sptr shadowMap;
        ShadowMap_FrameBuffer frameBuffer;// = ShadowMap_FrameBuffer(LIGHT_FBO_SIZE, LIGHT_FBO_SIZE);
    };
}

#endif // LIGHT_H
