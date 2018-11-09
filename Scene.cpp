#include "Scene.h"
#include "Material.h"
#include "OGL_Texture.h"
#include "Object.h"
#include "OGL_Shader.h"
#include "geException.h"

Scene::Scene(unsigned int shapesCount, unsigned int materialsCount, unsigned int texturesCount, unsigned int objectCount) : mSkybox(nullptr)
{
    mShapes.reserve(shapesCount);
    mMaterials.reserve(materialsCount);
    mTextures.reserve(texturesCount);
    mObjects.reserve(objectCount);
}


Scene::~Scene(void)
{
    // les smart pointers sont détruits automatiquement pas besoin d'appeler le freeAll;
}

void Scene::freeAll(void){
    mSkybox.reset();

    unsigned int i(0);
    DEBUG_LOG(std::cerr,"Scene:freeAll(), Trying to free "+ std::to_string(mObjects.size())+" objects.");
    while(i < mObjects.size()){
        if(!removeObject(i)){
            i++;
        }
    }
    DEBUG_LOG(std::cerr,"Scene:freeAll(), "+ std::to_string(mObjects.size())+" Objects remaining.");

    DEBUG_LOG(std::cerr,"Scene:freeAll(), Trying to free "+ std::to_string(mShapes.size())+" shapes.");
    i = 0;
    while(i < mShapes.size()){
        if(!removeShape(i)){
            i++;
        }
    }
    DEBUG_LOG(std::cerr,"Scene:freeAll(), "+ std::to_string(mShapes.size())+" Shapes remaining.");

    DEBUG_LOG(std::cerr,"Scene:freeAll(), Trying to free "+ std::to_string(mMaterials.size())+" materials.");
    i = 0;
    while(i < mMaterials.size()){
        if(!removeMaterial(i)){
            i++;
        }
    }
    DEBUG_LOG(std::cerr,"Scene:freeAll(), "+ std::to_string(mMaterials.size())+" materials remaining.");

    DEBUG_LOG(std::cerr,"Scene:freeAll(), Trying to free "+ std::to_string(mTextures.size())+" textures.");
    i = 0;
    while(i < mTextures.size()){
        if(!removeTexture(i)){
            i++;
        }
    }
    DEBUG_LOG(std::cerr,"Scene:freeAll(), "+ std::to_string(mTextures.size())+" textures remaining.");
}

bool Scene::removeShape(unsigned int index){
    CHECK_INDEX(index, mShapes.size(), std::cerr << "Warning : Scene:removeShape(index), index out of range.\n");
    if(mShapes[index].unique()){
        mShapes.erase(mShapes.begin()+index);
        return true;
    }
    DEBUG_LOG(std::cerr, "Scene:removeShape(), Failed to remove shape " + std::to_string(index) + ", element is used elsewhere.");
    return false;
}

bool Scene::removeMaterial(unsigned int index){
    CHECK_INDEX(index, mMaterials.size(), std::cerr << "Warning : Scene:removeMaterial(index), index out of range.\n");
    if(mMaterials[index].unique()){
        mMaterials.erase(mMaterials.begin()+index);
        return true;
    }
    DEBUG_LOG(std::cerr, "Scene:removeMaterial(), Failed to remove material " + std::to_string(index) + ", element is used elsewhere.");
    return false;
}

bool Scene::removeTexture(unsigned int index){
    CHECK_INDEX(index, mTextures.size(), std::cerr << "Warning : Scene:removeTexture(index), index out of range.\n");
    if(mTextures[index].unique()){
        mTextures.erase(mTextures.begin()+index);
        return true;
    }
    DEBUG_LOG(std::cerr, "Scene:removeTextures(), Failed to remove texture " + std::to_string(index) + ", element is used elsewhere.");
    return false;
}

bool Scene::removeObject(unsigned int index){
    CHECK_INDEX(index, mObjects.size(), std::cerr << "Warning : Scene:removeObject(index), index out of range.\n");
    if(mObjects[index].unique()){
        mObjects.erase(mObjects.begin()+index);
        return true;
    }
    DEBUG_LOG(std::cerr, "Scene:removeObject(), Failed to remove object " + std::to_string(index) + ", element is used elsewhere.");
    return false;
}

Shape_sptr Scene::getNewShape(const ShapeCreateInfo &info, unsigned int* id){
    Shape_sptr shape = std::make_shared<Shape>(info);

    CHECK_POINTER(id, std::cerr << "Warning : Scene:getNewShape(info, id), id should be a valid pointer.\n")
    (*id) = mShapes.size();

    mShapes.push_back(shape);
    return shape;
}
Material_sptr Scene::getNewMaterial(const MaterialCreateInfo &info, unsigned int* id){
    Material_sptr material = std::make_shared<Material>(info);

    CHECK_POINTER(id,std::cerr << "Warning : Scene:getNewMaterial(info, id), id should be a valid pointer.\n")
    (*id) = mMaterials.size();

    mMaterials.push_back(material);
    return material;
}
Texture_sptr Scene::getNewTexture(const OGL_TextureCreateInfo &info, unsigned int* id){
    Texture_sptr texture = std::make_shared<OGL_Texture>(info);

    CHECK_POINTER(id,std::cerr << "Warning : Scene:getNewTexture(info, id), id should be a valid pointer.\n")
    (*id) = mTextures.size();

    mTextures.push_back(texture);
    return texture;
}
Object_sptr Scene::getNewObject(const ObjectCreateInfo& info, unsigned int* id){
    Object_sptr object = std::make_shared<Object>(info);

    CHECK_POINTER(id,std::cerr << "Warning : Scene:getNewObject(info, id), should a valid pointer.\n")
    (*id) = mObjects.size();

    mObjects.push_back(object);
    return object;
}

Shape_sptr Scene::getShape(unsigned int index) const{
    CHECK_INDEX(index, mShapes.size(), std::cerr << "Warning : Scene:getShape(index), index out of range.\n");
    return mShapes[index];
}
Material_sptr Scene::getMaterial(unsigned int index) const{
    CHECK_INDEX(index, mMaterials.size(), std::cerr << "Warning : Scene:getMaterial(index), index out of range.\n");
    return mMaterials[index];
}
Texture_sptr Scene::getTexture(unsigned int index) const {
    CHECK_INDEX(index, mTextures.size(), std::cerr << "Warning : Scene:getTexture(index), index out of range.\n");
    return mTextures[index];
}
Object_sptr Scene::getObject(unsigned int index) const {
    CHECK_INDEX(index, mObjects.size(), std::cerr << "Warning : Scene:getObject(index), index out of range.\n");
    return mObjects[index];
}
Object_sptr Scene::getSkybox(void) const { return mSkybox; }

unsigned int Scene::getShapesCount(void) const { return mShapes.size(); }
unsigned int Scene::getMaterialsCount(void) const { return mMaterials.size(); }
unsigned int Scene::getTexturesCount(void) const { return mTextures.size(); }
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
