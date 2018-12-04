#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#include "geHeader.h"
#include "GLFW/glfw3.h"
#include "Scene.h"
#include "Camera.h"
#include "InputManager.h"
#include "RenderPicking.h"
#include "Transform.h"

#define SWAP_INTERVAL 0



class OGL_FrameBuffer;

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
    Camera mCamera;
    InputManager mInputManager;
    std::unique_ptr<RenderPhong> mPhongRender;
    std::unique_ptr<RenderPicking> mRenderPicking;
    std::unique_ptr<RenderShadowMapping> mShadowMapping;
    std::vector<std::unique_ptr<OGL_FrameBuffer> > mFrameBuffers;
    std::vector<Light_sptr> mLights;
   // std::vector<Texture_sptr> mShadowMaps;
    Object_sptr mConeObject;

    Transform_sptr mTransform;
};

#endif // MAINAPPLICATION_H
