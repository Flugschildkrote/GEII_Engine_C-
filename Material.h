#ifndef MATERIAL_H
#define MATERIAL_H

#include "geHeader.h"

using Texture_SceneID = uint32_t;

class Material
{
public:
    Material(const MaterialCreateInfo &info);
    virtual ~Material(void);

    bool isTransparent(void) const;
    const std::string toString(void) const;

    glm::vec3 mAmbiantColor_Kd;
    glm::vec3 mSpecularColor_Ks;
    float mSpecularExponent_Ns;
    float mTransparency_Alpha;
    bool mLightSensitive;
    Texture_sptr mTexture;
    Texture_sptr mNormalMap;
};

#endif // MATERIAL_H
