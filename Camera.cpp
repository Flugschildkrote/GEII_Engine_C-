#include "Camera.h"
#include "Transform.h"

namespace GEII
{
    Camera::Camera(void) : mType(CAMERA_TYPE_PERSPECTIVE), mFov(70.0f), mZNear(0.1f), mZFar(10.0f), mXres(1.0f), mYres(1.0f)
     /*mPos(0.0f,0.0f,0.0f), mLookPos(0.0f,1.0f,0.0f), mUpAxis(0.0f,1.0f,0.0f)*/ ,mTransform(std::make_shared<Transform>()){
        //ctor
    }

    Camera::~Camera(void)
    {
        //dtor
    }

    glm::mat4 Camera::getProjectionMatrix(void) const {
        switch(mType){
        case CAMERA_TYPE_PERSPECTIVE :
            return glm::perspective(mFov, getRatio(), mZNear, mZFar);
        case CAMERA_TYPE_ORTHOGRAPHIC :
            return glm::ortho(0.0f, static_cast<float>(mXres), 0.0f, static_cast<float>(mYres), mZNear, mZFar);
        default :
            break;
        }
        return glm::mat4(1.0f);
    }

    glm::mat4 Camera::getViewMatrix(void) const {
        glm::vec3 pos, lookDir, upAxis;
        pos = mTransform->getPosition();
        lookDir = mTransform->getWorldAxis(AXIS_FRONT);
        upAxis = mTransform->getWorldAxis(AXIS_UP);

        return glm::lookAt(pos, pos+lookDir, upAxis);
    }


    void Camera::setType(CameraType type) { mType  = type; }
    void Camera::setFov(float fov)        { mFov   = fov; }
    void Camera::setNear(float near2)      { mZNear = near2; } // Erreur de compilation avec variable "near"
    void Camera::setFar(float far2)        { mZFar = far2; }   // Erreur de compilation avec variable "far"
    void Camera::setXRes(unsigned int xRes) { mXres = xRes; }
    void Camera::setYRes(unsigned int yRes) { mYres = yRes; }
    void Camera::setTransform(const Transform_sptr &transform_sptr) { mTransform = transform_sptr; }
}
