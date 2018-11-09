#include "Object.h"

Object::Object(const ObjectCreateInfo &info) : mMaterial(info.material.lock()), mShape(info.shape.lock()), mTransformations(info.transformations), mIsEnabled(true){
    CHECK_OBJECT(std::cerr << "Warning : Object must be created with a valid shape\n", std::cerr <<  "Warning : Object must be created with a valid shape\n")
}

Object::~Object(void){

}

void Object::setEnabled(bool state){ mIsEnabled = state; }
void Object::resetTransform(const glm::vec3 &pos){
    mTransformations = glm::translate(glm::mat4(1.0), pos);
}


