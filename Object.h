#ifndef OBJECT_H
#define OBJECT_H

#include "geHeader.h"

#ifdef DEBUG
#define CHECK_OBJECT(error_shape, error_material) if(!mShape){error_shape;} if(!mMaterial){error_material;}
#else
#define CHECK_OBJECT(error_shape, error_material)
#endif // DEBUG

class Object{
public:
    Object(const ObjectCreateInfo &info);
    virtual ~Object(void);

    inline Material_sptr getMaterial(void) const { return mMaterial; }
    inline Shape_sptr getShape(void) const { return mShape; }
    inline glm::mat4 getTransformations(void) const { return mTransformations; }
    inline bool isEnabled(void) const { return mIsEnabled; }

    void setEnabled(bool state);
    void resetTransform(const glm::vec3 &pos);
protected:
    Material_sptr mMaterial;
    Shape_sptr mShape;
    glm::mat4 mTransformations;
    bool mIsEnabled;
};

#endif // OBJECT_H
