#include "MainApplication.h"
#include "geException.h"
#include "Render.h"
#include "OGL_Shader.h"
#include "Obj_Loader.h"
#include "GL_ErrorHandler.h"
#include "Scene.h"
#include "Object.h"
#include "Scancodes.h"
#include "OGL_FrameBuffer.h"
#include "OGL_Texture.h"


MainApplication::MainApplication(const std::string &appName, unsigned int w, unsigned int h) : mName(appName), mWidth(w), mHeight(h), mGLFWwindow(nullptr), mScene(10,10,10,10),
mInputManager(KeyMode::SCANCODE) { }

MainApplication::~MainApplication(void){

}

void MainApplication::clearContent(void){
    mPhongRender.reset(nullptr);
    mRenderPicking.reset(nullptr);
    mFrameBuffer.reset(nullptr);

    mScene.freeAll();
}

bool MainApplication::setup(void){
    if(!initGL()){
        close();
        return false;
    }
    initGLStates();
    initInput();
    loadResources();

    return true;
}

bool MainApplication::initGL(void){
    if(!glfwInit()){
        std::cerr << "Failed to init GLFW." << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    mGLFWwindow = glfwCreateWindow(mWidth, mHeight, mName.c_str(), nullptr, nullptr);
    if(!mGLFWwindow){
        std::cerr << "Failed to create GLFW Window." << std::endl;
        return false;
    }

    glfwMakeContextCurrent(mGLFWwindow);

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        std::cerr << "Failed to init glut." << std::endl;
        return false;
    }

    glDebugMessageCallback(geErrorCallback, nullptr);
    glfwSwapInterval(SWAP_INTERVAL);
    return true;
}

bool MainApplication::initGLStates(){
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glViewport(0,0,mWidth,mHeight);
    return true;
}

void MainApplication::initInput(void){
    glfwSetWindowUserPointer(mGLFWwindow, &mInputManager);
    glfwSetKeyCallback(mGLFWwindow, InputManager::keyCallback);
    glfwSetMouseButtonCallback(mGLFWwindow, InputManager::mouseButtonCallback);
    glfwSetCursorPosCallback(mGLFWwindow, InputManager::mouseMovedCallback);
    glfwSetWindowSizeCallback(mGLFWwindow, InputManager::windowResizedCallback);
}

bool MainApplication::loadResources(void){

    { //##########[CREATION DU RENDER]##########
        mPhongRender = std::make_unique<RenderPhong>();
        mRenderPicking = std::make_unique<RenderPicking>();
    }

    // Chargement d'un fichier obj dans la scene
    Obj_Loader myObjLoader("Data/citroen_ds3/Citroen_DS3.obj", &mScene);

    mCamera.mPos = glm::vec3(0,8,15);
    mCamera.mLookPos = glm::vec3(0.0f, 0.0f, 0.0f);
    mCamera.mType = CAMERA_TYPE_PERSPECTIVE;
    mCamera.mXres = mWidth;
    mCamera.mYres = mHeight;
    mCamera.mZFar = 100.0f;
    mCamera.mZNear = 0.1f;

    { // Skybox
        unsigned int id;
        OGL_TextureCreateInfo texInfo = {};
        texInfo.fromFile_nFromBuffer = true;
        texInfo.sourceFile = "Data/Textures/Skybox2.jpg";
       // texInfo.sourceFile = "Data/Textures/earth.bmp";

        texInfo.wrapS = GL_CLAMP_TO_EDGE;
        texInfo.wrapT = GL_CLAMP_TO_EDGE;
        texInfo.magFilter = GL_LINEAR;
        texInfo.genMipMaps = false;
        Texture_sptr skyboxTexture = mScene.getNewTexture(texInfo, &id);

        MaterialCreateInfo matInfo = {};
        matInfo.texture = skyboxTexture;
        Material_sptr skyboxMaterial = mScene.getNewMaterial(matInfo, &id);

        ShapeCreateInfo shapeInfo = {};
        shapeInfo.sourceType = ShapeSource::SKYBOX;
        shapeInfo.half_x = 1.0f;
        shapeInfo.half_y = 1.0f;;
        shapeInfo.half_z = 1.0f;
        Shape_wptr skyboxShape = mScene.getNewShape(shapeInfo, &id);

        ObjectCreateInfo skyboxInfo = {};
        skyboxInfo.material = skyboxMaterial;
        skyboxInfo.shape = skyboxShape;
        Object_sptr skyboxObject = mScene.getNewObject(skyboxInfo, &id);
        mScene.setSkybox(skyboxObject);
    }

     { // Frame Buffer
        unsigned char pixels[12] = { 255, 255,255, 255, 0, 255, 0, 255, 255, 255, 0, 255};
        unsigned int id;
        OGL_TextureCreateInfo texInfo = {};
        texInfo.fromFile_nFromBuffer = false;
        texInfo.sourceFile = "Data/Textures/earth_1024.bmp";
        texInfo.genMipMaps = false;
        texInfo.width = 512;
        texInfo.height = 512;
        texInfo.format = GL_RGB;
        Texture_sptr colorTexture = mScene.getNewTexture(texInfo, &id);

        AttachmentData attachmentData = {};
        attachmentData.attachment = GL_COLOR_ATTACHMENT0;
        attachmentData.textureID = colorTexture->getID();

        FrameBuffer_CreateInfo frameBufferInfo = {};
        //frameBufferInfo.flags = FRAMEBUFFER_DISABLE_READ | FRAMEBUFFER_DISABLE_WRITE;
        frameBufferInfo.attachments.push_back(attachmentData);

        texInfo.format = GL_DEPTH_COMPONENT;
        Texture_sptr depthTexture = mScene.getNewTexture(texInfo, &id);
        attachmentData.attachment = GL_DEPTH_ATTACHMENT;
        attachmentData.textureID = depthTexture->getID();
        frameBufferInfo.attachments.push_back(attachmentData);
        frameBufferInfo.width = 512;
        frameBufferInfo.height = 512;

        mFrameBuffer = std::make_unique<OGL_FrameBuffer>(frameBufferInfo);

        MaterialCreateInfo matInfo = {};
        matInfo.texture = colorTexture;
        matInfo.lightSensitive = false;
        Material_sptr objectMaterial = mScene.getNewMaterial(matInfo, &id);

        ShapeCreateInfo shapeInfo = {};
        shapeInfo.sourceType = ShapeSource::SQUARE;
        shapeInfo.half_x = 2.0f;
        shapeInfo.half_y = 2.0f;
        shapeInfo.half_z = 1.0f;
        Shape_wptr objectShape = mScene.getNewShape(shapeInfo, &id);

        ObjectCreateInfo objectInfo = {};
        objectInfo.material = objectMaterial;
        objectInfo.shape = objectShape;
        Object_sptr object = mScene.getNewObject(objectInfo, &id);
        object->resetTransform(glm::vec3(6,0,0));
    }

    mScene.sortTransparentObjects();
    return true;
}

void MainApplication::close(void){
    clearContent();
    DEBUG_LOG(std::cerr, "MainApplication:close(), content destroyed.");

    glfwDestroyWindow(mGLFWwindow);
    DEBUG_LOG(std::cerr, "MainApplication:close(), Window \""+mName+"\" destroyed");

    glfwTerminate();
    DEBUG_LOG(std::cerr, "MainApplication:close(), GLFW has terminated.");
}

void MainApplication::run(void){
    try{
        if(!setup()){
            return;
        }

        while(!glfwWindowShouldClose(mGLFWwindow)){
            processInput();
            tick();
            draw();
            glfwPollEvents();
        }
    }catch(const geException &e){
        std::cerr << e.what() << std::endl;
    }
    close();
}

void MainApplication::processInput(void){
    if(mInputManager.getKey(GLFW_KEY_Q)){
        std::cerr << "Key A pressed" << std::endl;

    }

    if(mInputManager.getKeyModifier(GLFW_MOD_ALT)){
        std::cerr << "ALT ENABLED" << std::endl;
    }

    if(mInputManager.getFlagState(MOUSE_BUTTON_CLICKED_FLAG)){
        if(mInputManager.getMouseButtonClicked(GLFW_MOUSE_BUTTON_1)){
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            mRenderPicking->draw(&mScene, &mCamera);
            PickingID_t id = mRenderPicking->getClickedObject(mInputManager.getMouseX(), mHeight-mInputManager.getMouseY());

            if(id == RENDER_PICKING_SKYBOX)
                std::cerr << "Clicked on the Skybox" << std::endl;
            else{
                mScene.getObject(id)->setEnabled(false);
            }
        }
        mInputManager.resetFlags(MOUSE_BUTTON_CLICKED_FLAG);
    }


    if(mInputManager.getFlagState(WIND0W_RESIZED_FLAG)){
        glfwGetWindowSize(mGLFWwindow, reinterpret_cast<int*>(&mWidth), reinterpret_cast<int*>(&mHeight));
        glViewport(0, 0, mWidth, mHeight);
        mCamera.mXres = mWidth;
        mCamera.mYres = mHeight;
        mInputManager.resetFlags(WIND0W_RESIZED_FLAG);
    }
}

void MainApplication::tick(void){
    mCamera.right(0.032f);
    mPhongRender->update(&mCamera);
    mScene.getSkybox()->resetTransform(mCamera.mPos);
}

void MainApplication::draw(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mFrameBuffer->bind(true);
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, 0, mFrameBuffer->getWidth(), mFrameBuffer->getHeight());
        mCamera.mXres = mFrameBuffer->getWidth();
        mCamera.mYres = mFrameBuffer->getHeight();

        mPhongRender->draw(&mScene, &mCamera);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, mWidth, mHeight);
    mCamera.mXres = mWidth;
    mCamera.mYres = mHeight;
    mPhongRender->draw(&mScene, &mCamera);
    glfwSwapBuffers(mGLFWwindow);
}
