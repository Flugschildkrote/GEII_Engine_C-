#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>

#include "geHeader.h"

using namespace GEII;

using Material_SceneID = unsigned int;
class Scene{
public:
    Scene(unsigned int objectCount, const glm::vec3 &ambiantColor);
    virtual ~Scene(void);

    void freeAll(void);

    Object_sptr getObject(unsigned int index) const;
    Object_sptr getByPickingID(const PickingID_t &id) const;
    Object_sptr getSkybox(void) const;

    unsigned int getObjectsCount(void) const;
    bool hasSkybox(void) const;

    int addObject(const Object_sptr& object);
    bool removeObject(unsigned int index);

    int addLight(const Light_sptr& light);
    Light_sptr getLight(unsigned int index) const;
    bool removeLight(unsigned int index);
    unsigned int getLightCount(void) const;

    void setSkybox(const Object_sptr &skybox);

    void sortTransparentObjects(void);
    unsigned int getTranparentObjectsCount(void) const;
    glm::vec3 getAmbiantColor(void) const { return mAmbiantColor; }

  ///  void SceneSwapObjects(unsigned int p0, unsigned int p1);
  ///  unsigned int SceneGetObjectTriangleNumber(unsigned int p);

private:
    std::vector<Object_sptr> mObjects;
    std::vector<Light_sptr> mLights;
    Object_sptr mSkybox;
    glm::vec3 mAmbiantColor;
};

#endif // SCENE_H
