#ifndef RENDERPICKING_H
#define RENDERPICKING_H

#include "geHeader.h"
#include "Render.h"

#define RENDER_PICKING_SKYBOX 0xFFFFFF
#define RENDER_PICKING_SKYBOX_COLOR glm::vec3(1.0f, 1.0f, 1.0f)

class RenderPicking : public Render
{
public:
    RenderPicking(void);
    virtual ~RenderPicking(void);

    virtual void draw(Scene *scene, Camera* camera) const;
    PickingID_t getClickedObject(int x, int y);
protected:
    virtual void initUniforms(void);

    GLint mU_mvp;
    GLint mU_colorPicking;
};

#endif // RENDERPICKING_H
