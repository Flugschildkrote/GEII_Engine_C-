#ifndef LIGHT_H
#define LIGHT_H

#include "geHeader.h"
#include "Transform.h"

struct Light{
    glm::vec3 ambiantColor;
    glm::vec3 power;
    LightType type;
    float angle;
    Transform_sptr transform = std::make_shared<Transform>();
    Texture_sptr shadowMap;
};

#endif // LIGHT_H
