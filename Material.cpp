#include "Material.h"

Material::Material(const MaterialCreateInfo &info) : mAmbiantColor_Kd(info.ambiantColor_Kd), mSpecularColor_Ks(info.specularColor_Ks),
mSpecularExponent_Ns(info.specularExponent_Ns), mTransparency_Alpha(info.transparency_Alpha),mLightSensitive(info.lightSensitive), mTexture(info.texture){

}

Material::~Material(void){

}

bool Material::isTransparent(void) const { return mTransparency_Alpha < 1.0f; }
const std::string Material::toString(void) const{
    return std::string("");
}
