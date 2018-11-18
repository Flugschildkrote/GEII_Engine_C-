#ifndef OBJECT_H
#define OBJECT_H

#include "geHeader.h"
#include "Transform.h"

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
    inline Transform_sptr getTransform(void) const { return mTransform; }
    inline bool isEnabled(void) const { return mIsEnabled; }
  //  inline PickingID_t getPickingID(void) const { return mPickingID; }

    void setEnabled(bool state);
  //  void setPickingID(const PickingID_t &id);
    void setTransform(const Transform_sptr &transform){mTransform = transform;}
protected:
    Material_sptr mMaterial;
    Shape_sptr mShape;
    Transform_sptr mTransform;
  //  PickingID_t mPickingID;
    bool mIsEnabled;
};

#endif // OBJECT_H
