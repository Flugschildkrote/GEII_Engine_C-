#include "Object.h"

Object::Object(const ObjectCreateInfo &info) : mMaterial(info.material.lock()), mShape(info.shape.lock()), mTransform(std::make_shared<Transform>()),/* mPickingID(0),*/ mIsEnabled(true){
    CHECK_OBJECT(std::cerr << "Warning : Object must be created with a valid shape\n", std::cerr <<  "Warning : Object must be created with a valid shape\n")
}

Object::~Object(void){

}

void Object::setEnabled(bool state){ mIsEnabled = state; }



