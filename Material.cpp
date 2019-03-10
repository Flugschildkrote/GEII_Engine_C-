#include "Material.h"

namespace GEII
{
    Material::Material(void) : mDiffuseColor(1.0f, 1.0f, 1.0f), mSpecularColor(1.0f, 1.0f, 1.0f), mSpecularExponent(1.0f), mOpacity(1.0f), mLightSensitive(true),
          mDiffuseTexture(nullptr), mNormalMap(nullptr)
    {

    }

    Material::Material(const glm::vec3 &diffuseColor, const glm::vec3 &specularColor, float specularExponent, float opacity,
             const Texture_sptr& diffuseTexture, const Texture_sptr& normalMap, bool lightSensitive)
        : mDiffuseColor(diffuseColor), mSpecularColor(specularColor), mSpecularExponent(specularExponent), mOpacity(opacity), mLightSensitive(lightSensitive),
          mDiffuseTexture(diffuseTexture), mNormalMap(normalMap)
    {

    }

    Material::Material(const Texture_sptr& diffuseTexture, bool lightSensitive)
        : mDiffuseColor(1.0f, 1.0f, 1.0f), mSpecularColor(1.0f, 1.0f, 1.0f), mSpecularExponent(1.0f), mOpacity(1.0f), mLightSensitive(lightSensitive),
          mDiffuseTexture(diffuseTexture), mNormalMap(nullptr)
    {

    }

    Material::~Material(void)
    {

    }

    bool Material::isTransparent(void) const {
        return mOpacity < 1.0f;
    }
}
