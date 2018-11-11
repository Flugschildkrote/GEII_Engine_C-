#include "MainApplication.h"
#include "geException.h"
#include "Render.h"
#include "OGL_Shader.h"
#include "Obj_Loader.h"
#include "GL_ErrorHandler.h"
#include "Scene.h"
#include "Object.h"
#include "Scancodes.h"


MainApplication::MainApplication(const std::string &appName, unsigned int w, unsigned int h) : mName(appName), mWidth(w), mHeight(h), mGLFWwindow(nullptr), mScene(10,10,10,10),
mInputManager(KeyMode::SCANCODE) { }

MainApplication::~MainApplication(void){

}

void MainApplication::clearContent(void){
    mPhongRender.reset(nullptr);
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
    }

    // Chargement d'un fichier obj dans la scene
    Obj_Loader myObjLoader("Data/citroen_ds3/Citroen_DS3.obj", &mScene);

    mCamera.mPos = glm::vec3(0,8,15);
    mCamera.mLookPos = glm::vec3(0.0f, 0.0f, 0.0f);
    mCamera.mType = CAMERA_TYPE_PERSPECTIVE;
    mCamera.mXres = mWidth;
    mCamera.mYres = mHeight;
    mCamera.mZFar = 200.0f;
    mCamera.mZNear = 0.01f;

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

    if(mInputManager.getFlagState(WIND0W_RESIZED_FLAG)){
        glfwGetWindowSize(mGLFWwindow, reinterpret_cast<int*>(&mWidth), reinterpret_cast<int*>(&mHeight));
        glViewport(0, 0, mWidth, mHeight);
        mCamera.mXres = mWidth;
        mCamera.mYres = mHeight;
        mInputManager.resetFlags(WIND0W_RESIZED_FLAG);
    }

}

void MainApplication::tick(void){
    mPhongRender->update(&mCamera);
    mCamera.right(0.032f);
    mScene.getSkybox()->resetTransform(mCamera.mPos);
}

void MainApplication::draw(void){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mPhongRender->draw(&mScene, &mCamera);
    glfwSwapBuffers(mGLFWwindow);
}
