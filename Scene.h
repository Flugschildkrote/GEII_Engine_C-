#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>

#include "Shape.h"
#include "geHeader.h"

using Material_SceneID = unsigned int;
class Scene{
public:
    Scene(unsigned int shapesCount, unsigned int materialsCount, unsigned int texturesCount, unsigned int objectCount);
    virtual ~Scene(void);
    void freeAll(void);

    Shape_sptr getNewShape(const ShapeCreateInfo &info, unsigned int* id = nullptr);
    Material_sptr getNewMaterial(const MaterialCreateInfo &info, unsigned int* id = nullptr);
    Texture_sptr getNewTexture(const OGL_TextureCreateInfo &info, unsigned int* id = nullptr);
    Object_sptr getNewObject(const ObjectCreateInfo &info, unsigned int* id = nullptr);

    Shape_sptr getShape(unsigned int index) const;
    Material_sptr getMaterial(unsigned int index) const;
    Texture_sptr getTexture(unsigned int index) const;
    Object_sptr getObject(unsigned int index) const;
    Object_sptr getSkybox(void) const;

    unsigned int getShapesCount(void) const;
    unsigned int getMaterialsCount(void) const;
    unsigned int getTexturesCount(void) const;
    unsigned int getObjectsCount(void) const;
    bool hasSkybox(void) const;

    bool removeShape(unsigned int index);
    bool removeMaterial(unsigned int index);
    bool removeTexture(unsigned int index);
    bool removeObject(unsigned int index);

    void setSkybox(const Object_sptr &skybox);

    void sortTransparentObjects(void);
    unsigned int getTranparentObjectsCount(void) const;

  ///  void SceneSwapObjects(unsigned int p0, unsigned int p1);
  ///  unsigned int SceneGetObjectTriangleNumber(unsigned int p);

private:
    std::vector<Shape_sptr> mShapes;
    std::vector<Material_sptr> mMaterials;
    std::vector<Texture_sptr> mTextures;
    std::vector<Object_sptr> mObjects;
    Object_sptr mSkybox;
};

#endif // SCENE_H
