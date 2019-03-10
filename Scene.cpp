#include "Scene.h"
#include "Material.h"
#include "OGL_Texture.h"
#include "Object.h"
#include "OGL_Shader.h"
#include "geException.h"
#include "Log.h"

Scene::Scene(unsigned int objectCount, const glm::vec3 &ambiantColor) : mSkybox(nullptr), mAmbiantColor(ambiantColor)
{
    mObjects.reserve(objectCount);
}


Scene::~Scene(void)
{
    // les smart pointers sont détruits automatiquement pas besoin d'appeler le freeAll;
}

void Scene::freeAll(void){
    mSkybox.reset();

    unsigned int i(0);
    ENGINE_LOG_INFO("Scene:freeAll(), Trying to free ", mObjects.size()," objects.");
    while(i < mObjects.size()){
        if(!removeObject(i)){
            i++;
        }
    }

    DEBUG_BLOCK(if(mObjects.size()){
                    ENGINE_LOG_WARNING("Scene:freeAll(), ", mObjects.size(), " Objects remaining.");
                }else{
                    ENGINE_LOG_SUCCESS("Scene:freeAll(), ", mObjects.size(), " Objects remaining.");
                }
    )
}

int Scene::addObject(const Object_sptr& object){
    /*ENGINE_DEBUG_CODE(
        for(unsigned int i(0); i < mObjects.size(); i++){
            if(mObjects[i] == object){
                ENGINE_LOG_WARNING("Trying to add an object witch is already in scene. (index ", i ").");
            }
        }
    )*/

    mObjects.push_back(object);
    return mObjects.size()-1;
}

bool Scene::removeObject(unsigned int index){
    CHECK_INDEX(index, mObjects.size(), ENGINE_LOG_ERROR("Scene:removeObject(index), index out of range."));
    mObjects.erase(mObjects.begin()+index);
    return true;
}

Object_sptr Scene::getObject(unsigned int index) const {
    CHECK_INDEX(index, mObjects.size(), ENGINE_LOG_ERROR("Scene:getObject(index), index out of range.\n"));
    return mObjects[index];
}

Object_sptr Scene::getSkybox(void) const { return mSkybox; }

unsigned int Scene::getObjectsCount(void) const { return mObjects.size(); }
bool Scene::hasSkybox(void) const { return static_cast<bool>(mSkybox); }


void Scene::setSkybox(const Object_sptr &skybox){ mSkybox = skybox; }

void Scene::sortTransparentObjects(void){
    unsigned int nbprim= mObjects.size();
    if(!nbprim)
        return;

    unsigned int transparentCount = getTranparentObjectsCount();
    unsigned int lastTransparent = nbprim-1;
    for(unsigned int i(0); i < transparentCount; i++){
        if(!mObjects[i]->getMaterial()->isTransparent()){
            while(!mObjects[lastTransparent]->getMaterial()->isTransparent()){
                lastTransparent--;
            }
            std::swap(mObjects[i], mObjects[lastTransparent]);
        }
    }
}
unsigned int Scene::getTranparentObjectsCount(void) const{
    unsigned int nbprim=mObjects.size();
    if(!nbprim)
        return 0;

    unsigned int nbTransparent = 0;
    for(unsigned int i(0); i < nbprim; i++){
        if(mObjects[i]->getMaterial()->isTransparent()){
            nbTransparent++;
        }
    }
    return nbTransparent;
}

int Scene::addLight(const Light_sptr& light){
    mLights.push_back(light);
    return mLights.size()-1;
}
Light_sptr Scene::getLight(unsigned int index) const{
    CHECK_INDEX(index, mLights.size(), ENGINE_LOG_ERROR("Scene:getLight(index), index out of range.\n"));
    return mLights[index];
}
bool Scene::removeLight(unsigned int index){
    CHECK_INDEX(index, mLights.size(), ENGINE_LOG_ERROR("Scene:getLight(index), index out of range.\n"));
    mLights.erase(mLights.begin()+index);
    return true;
}
unsigned int Scene::getLightCount(void) const { return mLights.size(); }
