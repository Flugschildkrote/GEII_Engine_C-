#include "Object.h"

namespace GEII
{
    Object::Object(const Shape_sptr &shape, const Material_sptr& material, const Transform_sptr& transform)
        : mShape(shape), mMaterial(material), mTransform(transform), mIsEnabled(true)
    {
        if(!mTransform){
            mTransform = std::make_shared<Transform>();
        }
    }

    Object::~Object(void){

    }

    void Object::setEnabled(bool state){ mIsEnabled = state; }
}


