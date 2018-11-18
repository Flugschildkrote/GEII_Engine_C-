#include "Transform.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/vector_angle.hpp"

Transform::Transform() : mTransformations(1.0f)/*, mTrackingEnabled(false), mTrackingPoint(glm::vec3(0.0f, 0.0f, 0.0f))*/{

}

Transform::~Transform(){
    //dtor
}


void Transform::translate(const glm::vec3 &translation, SpaceReference spaceRef){
    if(spaceRef == SpaceReference::LOCAL){
        mTransformations = glm::translate(mTransformations, translation);
    }else if(spaceRef == SpaceReference::WORLD){
        mTransformations = glm::translate(glm::mat4(1.0f), translation)*mTransformations;
    }

    //updateTracking();
}

void Transform::rotate(float angle, const glm::vec3 &axis, SpaceReference spaceRef){
    /*if(mTrackingEnabled){
        DEBUG_BLOCK(std::cerr << "Warning : Transform::rotate : Impossible to rotate a transform while tracking is enabled.");
        return;
    }*/

    // Le tracking n'est pas activé
    if(spaceRef == SpaceReference::LOCAL){
        mTransformations = glm::rotate(mTransformations, angle, axis);
    }else if(spaceRef == SpaceReference::WORLD){
        glm::mat4 inverseMatrix = glm::inverse(mTransformations);
        glm::vec3 axisWorld = inverseMatrix*glm::vec4(axis, 0.0f);
        mTransformations = glm::rotate(mTransformations, angle, axisWorld);
    }
}

void Transform::setPosition(const glm::vec3 &pos){
    mTransformations[3] = glm::vec4(pos, 1.0f);
}


/*void Transform::setTrackPoint(const glm::vec3 &point, bool enableTracking){
    mTrackingPoint = point;
    setTracking(enableTracking);
}*/

/*void Transform::setTracking(bool trackingState){
    mTrackingEnabled = trackingState;

  //  updateTracking();
}*/

/*void Transform::updateTracking(void){
    if(!mTrackingEnabled) { return; };
    if(getPosition() == mTrackingPoint){ return; }
    //(Forward sur -Z)
    mTransformations = glm::lookAt(getPosition(), mTrackingPoint, glm::vec3(0.0f, 1.0f, 0.0f));
    /*glm::vec3 lookDir = glm::normalize(mTrackingPoint-getPosition()); // OK
    glm::vec3 lookDirBis = glm::normalize(glm::vec3(lookDir.x, 0.0f, lookDir.z));
    glm::vec3 forwardDir = glm::normalize(mTransformations*glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); // OK
    //std::cout << geToString(lookDirBis) << std::endl;
    glm::vec3 upAxis = glm::normalize(mTransformations*glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    float angleX = glm::orientedAngle(lookDirBis, forwardDir, upAxis);
   // std::cerr << "Angle x : " << glm::degrees(angleX) << std::endl;
    rotate(-angleX, upAxis, SpaceReference::WORLD);

    forwardDir = glm::normalize(mTransformations*glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); // OK
    glm::vec3 rightAxis = glm::normalize(mTransformations*glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    float angleY = glm::orientedAngle(lookDir, forwardDir, rightAxis);
    std::cerr << "Angle y : " << glm::degrees(angleY) << std::endl;
    //rotate(-angleY, rightAxis, SpaceReference::WORLD);*/
    // Le tracking est activé
//}
