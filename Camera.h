#ifndef CAMERA_H
#define CAMERA_H

#include "geHeader.h"
#include "Transform.h"

enum CameraType : uint8_t{
    CAMERA_TYPE_ORTHOGRAPHIC = 0,
    CAMERA_TYPE_PERSPECTIVE = 1
};

class Camera
{
public:
    Camera(void);
    virtual ~Camera(void);

    CameraType mType;
    float mFov, mZNear, mZFar;
    unsigned int mXres, mYres;
    Transform_sptr mTransform;
   // glm::vec3 mPos, mLookPos, mUpAxis;

    glm::mat4 getProjectionMatrix(void) const;
    glm::mat4 getViewMatrix(void) const;
    float getRatio(void) const;

  /*  void forward(float dist);
    void backward(float dist);
    void up(float dist);
    void down(float dist);
    void left(float dist);
    void right(float dist);*/

    //glm::vec3 getLookDir(void) const;
protected:
};

#endif // CAMERA_H
