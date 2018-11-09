#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#include "geHeader.h"
#include "GLFW/glfw3.h"
#include "Scene.h"
#include "Camera.h"

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
    bool loadResources(void);
    void clearContent(void);
    void close(void);

    void tick(void);
    void draw(void);

    std::string mName;
    unsigned int mWidth, mHeight;
    GLFWwindow* mGLFWwindow;
    Scene mScene;
    Camera mCamera;
    std::unique_ptr<RenderPhong> mPhongRender;
    //std::unique_ptr<RenderPicking> mPickingRender;
};

#endif // MAINAPPLICATION_H
