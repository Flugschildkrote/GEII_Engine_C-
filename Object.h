#ifndef OBJECT_H
#define OBJECT_H

#include "geHeader.h"
#include "Transform.h"

namespace GEII
{
    class Object{
    public:
        Object(const Shape_sptr &shape, const Material_sptr& material, const Transform_sptr& transform = Transform_sptr(nullptr));
        virtual ~Object(void);

        inline Material_sptr getMaterial(void) const { return mMaterial; }
        inline Shape_sptr getShape(void) const { return mShape; }
        inline Transform_sptr getTransform(void) const { return mTransform; }
        inline bool isEnabled(void) const { return mIsEnabled; }

        void setEnabled(bool state);
        void setTransform(const Transform_sptr &transform){mTransform = transform;}
    protected:
        Shape_sptr mShape;
        Material_sptr mMaterial;
        Transform_sptr mTransform;
        bool mIsEnabled;
    };
}
#endif // OBJECT_H
