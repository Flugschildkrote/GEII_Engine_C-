#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "geHeader.h"

enum class SpaceReference{
    WORLD,
    LOCAL
};

constexpr glm::vec3 AXIS_FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
constexpr glm::vec3 AXIS_BACK = -AXIS_FRONT;
constexpr glm::vec3 AXIS_LEFT = glm::vec3(-1.0f, 0.0f, 0.0f);
constexpr glm::vec3 AXIS_RIGHT = -AXIS_LEFT;
constexpr glm::vec3 AXIS_UP = glm::vec3(0.0f, 1.0f, 0.0f);
constexpr glm::vec3 AXIS_DOWN = -AXIS_UP;

class Transform
{
public:
    Transform(void);
    virtual ~Transform(void);

    void translate(const glm::vec3 &translation, SpaceReference spaceRef = SpaceReference::LOCAL);
    void rotate(float angle, const glm::vec3 &axis, SpaceReference spaceRef = SpaceReference::LOCAL);

    glm::vec3 getWorldAxis(const glm::vec3 &localAxis) const { return mTransformations*glm::vec4(localAxis, 0.0f); };
    glm::vec3 getPosition(void) const { return glm::vec3(mTransformations*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)); }
   // bool getTrackingState(void) const { return mTrackingEnabled; }
    glm::mat4 getTransformations(void) const { return mTransformations; }

    void setPosition(const glm::vec3 &pos);
    void setTransformations(const glm::mat4 &transformations){ mTransformations = transformations; }
private:
    void updateTracking(void);

    glm::mat4 mTransformations;

    // Tracking informations
    /*bool mTrackingEnabled;
    glm::vec3 mTrackingPoint;*/
};

using Transform_sptr = std::shared_ptr<Transform>;
using Transform_wptr = std::weak_ptr<Transform>;
using Transform_uptr = std::unique_ptr<Transform>;

#endif // TRANSFORM_H
