#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#include "geHeader.h"
#include "GLFW/glfw3.h"
#include "Scene.h"
#include "Camera.h"
#include "InputManager.h"
#include "RenderPicking.h"
#include "Transform.h"
//#include "OGL_FrameBuffer.h"

#define SWAP_INTERVAL 0

namespace GEII
{
    class OGL_FrameBuffer;
}

class MainApplication
{
public:
    MainApplication(const std::string &appName, unsigned int w, unsigned int h);
    virtual ~MainApplication(void);

    void run(void);

private:
    bool setup(void);
    bool initGL(void);
    bool initGLStates(void);
    void initInput(void);
    bool loadResources(void);
    void clearContent(void);
    void close(void);
    void processInput(void);

    void tick(void);
    void draw(void);
    std::string mName;
    unsigned int mWidth, mHeight;
    GLFWwindow* mGLFWwindow;
    Scene mScene;
    GEII::Camera mCamera;
    InputManager mInputManager;
    std::unique_ptr<GEII::RenderPhong> mPhongRender;
    std::unique_ptr<GEII::RenderPicking> mRenderPicking;
    std::unique_ptr<GEII::RenderShadowMapping> mShadowMapping;
    std::vector<std::shared_ptr<GEII::OGL_FrameBuffer> > mFrameBuffers;
    std::vector<Light_sptr> mLights;
   // std::vector<Texture_sptr> mShadowMaps;
    Object_sptr mConeObject;

    Transform_sptr mTransform;
};

#endif // MAINAPPLICATION_H
