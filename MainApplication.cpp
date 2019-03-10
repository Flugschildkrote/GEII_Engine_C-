#include "MainApplication.h"
#include "geException.h"
#include "Render.h"
#include "OGL_Shader.h"
#include "Obj_Loader.h"
#include "GL_ErrorHandler.h"
#include "Scene.h"
#include "Object.h"
#include "Scancodes.h"
#include "Texture2D.h"
#include "Light.h"
#include "Shapes.h"
#include "Material.h"
#include "OGL_FrameBuffer.h"

using namespace GEII;

Shape_sptr CameraShape;
OGL_ShaderProgram_sptr WireShader;
Object_sptr testCube;

MainApplication::MainApplication(const std::string &appName, unsigned int w, unsigned int h) : mName(appName), mWidth(w), mHeight(h), mGLFWwindow(nullptr), mScene(10, glm::vec3(1.0f, 1.0f, 1.0f)),
mInputManager(KeyMode::KEYCODE) { }


MainApplication::~MainApplication(void){

}

void MainApplication::clearContent(void){
    mPhongRender.reset(nullptr);
    mRenderPicking.reset(nullptr);
    mConeObject.reset();
    mFrameBuffers.clear();
    mLights.clear();
    //mShadowMaps.clear();

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
    #ifdef DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    #else
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_FALSE);
    #endif
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
   // mLight.dir = glm::vec3(-1.0f, -1.0f, -1.0f);

     /** LIGHTS **/
    {
        /** SUN*/
        Light_sptr tmpLights[2];
        tmpLights[0] = std::make_shared<Light>();
        tmpLights[0]->ambiantColor = glm::vec3(1.2f, 1.2f, 1.2f);
        //tmpLights[0]->ambiantColor = glm::vec3(0.1f, 0.8f, 0.1f);
        tmpLights[0]->power = glm::vec3(1.0f, 1.0f, 1.0f);
        //tmpLights[0]->power = glm::vec3(0.0f, 0.0f, 0.0f);

        tmpLights[0]->transform->translate(glm::vec3(10.0f, 10.0f, 10.0f));
        tmpLights[0]->transform->rotate(glm::radians(45.0f), AXIS_UP, SpaceReference::LOCAL);
        tmpLights[0]->transform->rotate(glm::radians(-45.0f), AXIS_RIGHT, SpaceReference::LOCAL);
        tmpLights[0]->type = LIGHT_SUN;

        /** SPOT **/
        tmpLights[1] = std::make_shared<Light>();
        //tmpLights[1]->ambiantColor = glm::vec3(0.8f, 0.1f, 0.1f);
        tmpLights[1]->ambiantColor = glm::vec3(0.5f, 0.5f, 0.5f);
        tmpLights[1]->power = glm::vec3(0.0f, 0.0f, 0.0f);
        tmpLights[1]->transform->translate(glm::vec3(1.0f, 1.0f, 1.0f));
        tmpLights[1]->transform->rotate(glm::radians(45.0f), AXIS_UP, SpaceReference::LOCAL);
        tmpLights[1]->transform->rotate(glm::radians(-45.0f), AXIS_RIGHT, SpaceReference::LOCAL);
        tmpLights[1]->angle = 45.0f;
        tmpLights[1]->type = LIGHT_SPOT;

        mLights.push_back(tmpLights[0]);
        mLights.push_back(tmpLights[1]);
        mScene.addLight(tmpLights[0]);
        mScene.addLight(tmpLights[1]);

        { // Frame Buffer
            int fboSize = 4096;


            std::shared_ptr<ShadowMap_FrameBuffer> fbo1 = std::make_shared<ShadowMap_FrameBuffer>(fboSize, fboSize);
            std::shared_ptr<ShadowMap_FrameBuffer> fbo2 = std::make_shared<ShadowMap_FrameBuffer>(fboSize, fboSize);

            tmpLights[0]->shadowMap = fbo1->getDepthTexture();
            tmpLights[1]->shadowMap = fbo2->getDepthTexture();

            mFrameBuffers.push_back(fbo1);
            mFrameBuffers.push_back(fbo2);

            /*attachmentData.textureID = depthTexture2->getID();
            frameBufferInfo.attachments[0] = attachmentData;
            mFrameBuffers.push_back(std::make_unique<OGL_FrameBuffer>(frameBufferInfo));*/

            /*tmpLights[0]->shadowMap = depthTexture1;
            tmpLights[1]->shadowMap = depthTexture2;*/


            /** Carré pour afficher la texture de profondeur **/
            Material_sptr objectMaterial = std::make_shared<Material>(fbo1->getDepthTexture(), false);
            Material_sptr objectMaterial2 = std::make_shared<Material>(fbo2->getDepthTexture(), false);

           // Shape_wptr objectShape = mScene.getNewShape(shapeInfo, &id);
           #define USE_LIGH_DEBUG_SHAPE 0
           #if USE_LIGH_DEBUG_SHAPE == 1
                Shape_sptr objectShape = std::make_shared<Square>(2.0f, 2.0f);
                Object_sptr object = std::make_shared<Object>(objectShape, objectMaterial);
                mScene.addObject(object);
                object->getTransform()->rotate(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                object->getTransform()->translate(glm::vec3(-4.999, 5.0f, -2.1), SpaceReference::WORLD);

                object = std::make_shared<Object>(objectShape, objectMaterial2);
                object->getTransform()->rotate(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                object->getTransform()->translate(glm::vec3(-4.999, 5.0f, 2.1), SpaceReference::WORLD);
                mScene.addObject(object);
            #endif // USE_LIGH_DEBUG_SHAPE


        }
    }

    #define TEST_CUBE 0
    #if TEST_CUBE == 1
    { // Création du cube de test
        Shape_sptr shape = std::make_shared<Box>(2.0f, BoxUVPattern::FullFaces);
        Texture_sptr normalMap= std::make_shared<Texture2D>("Data/Textures/normal1.png", true);
        Texture_sptr diffuseMap = std::make_shared<Texture2D>("Data/Textures/Rock_028_SD/Rock_028_COLOR.jpg");
        Material_sptr material = std::make_shared<Material>(glm::vec3(0.7, 0.7, 0.7), glm::vec3(0.5, 0.5, 0.5), 5.0f, 1.0f, Texture_sptr(nullptr), normalMap,  true);

        testCube = std::make_shared<Object>(shape, material);
        mScene.addObject(testCube);

    }
    #endif // TEST_CUBE

    { //##########[CREATION DU RENDER]##########
        mPhongRender = std::make_unique<RenderPhong>();
        mRenderPicking = std::make_unique<RenderPicking>();
        mShadowMapping = std::make_unique<RenderShadowMapping>();
    }

    // Chargement d'un fichier obj dans la scene
  // Obj_Loader myObjLoader("Data/citroen_ds3/Citroen_DS3.obj", &mScene);
   // Obj_Loader myObjLoader("Data/Test_Obj/cat/cat.obj", &mScene);
    //myObjLoader.loadFile("Data/Test_Obj/cat/cat.obj", &mScene);


    //Obj_Loader myObjLoader("Data/Test_Obj/ShadowTest/ShadowTest2.obj", &mScene);
  //  Obj_Loader myObjLoader("Data/Test_Obj/cat/cat.obj", &mScene);
   // myObjLoader.loadFile("Data/Test_Obj/cat/cat.obj", &mScene);
  //  myObjLoader.loadFile("Data/citroen_ds3/Citroen_DS3.obj", &mScene);

   // mLights[1]->transform = mTransform;

    {
        Material_sptr myMaterial  = std::make_shared<Material>();
        myMaterial->setLightSensitive(false);
        myMaterial->setDiffuseColor(glm::vec3(1.0f, 0.5f, 0.5f));
        //Shape_sptr shape = mScene.getNewShape(shapeInfo, &id);
        Shape_sptr shape = std::make_shared<Box>(0.02f);
        Object_sptr object = std::make_shared<Object>(shape, myMaterial);
        mScene.addObject(object);
        //mLights[1]->transform = mCamera.mTransform;
        object->setTransform(mLights[1]->transform);
        mConeObject = mScene.getObject(0);
        mTransform = mConeObject->getTransform();
    }

    {
        /*ShapeCreateInfo createInfo = {};
        createInfo.sourceType = ShapeSource::BOX;*/
       // CameraShape = std::make_shared<Shape>(createInfo);
        //CameraShape->loadCamera(mLights[1]->angle, 1.0f, 0.1f, 100.0f);
       /* ShaderCreateInfo vShaderInfo = {};
        vShaderInfo.fromFile_nFromMemory = true;
        vShaderInfo.sourceFile = "Data/Shaders/Simple.vert";
        ShaderCreateInfo fShaderInfo = {};
        fShaderInfo.fromFile_nFromMemory = true;
        fShaderInfo.sourceFile = "Data/Shaders/Simple.frag";
        OGL_ShaderProgramCreateInfo programInfo = {};
        programInfo.fragInfo = &fShaderInfo;
        programInfo.vertInfo = &vShaderInfo;*/
        WireShader = std::make_shared<OGL_ShaderProgram>("Data/Shaders/Simple.vert", "Data/Shaders/Simple.frag");
    }

    {
        Obj_Loader loader("Data/the-mill/the-mill2.obj", &mScene);
        std::cerr << mScene.getObjectsCount() << std::endl;

        //Obj_Loader loader2("Data/citroen_ds3/Citroen_DS3.obj", &mScene);
    }
    //myObjLoader.loadFile("Data/citroen_ds3/Citroen_DS3.obj", &mScene);



   // mCamera.mPos = glm::vec3(0,8,15);
    //mCamera.mLookPos = glm::vec3(0.0f, 0.0f, 0.0f);
    mCamera.setType(CAMERA_TYPE_PERSPECTIVE);
    mCamera.setXRes(mWidth);
    mCamera.setXRes(mHeight);
    mCamera.setFar(100.0f);
    mCamera.setNear(0.1f);

    { // Skybox
        Texture_sptr skyboxTexture = std::make_shared<Texture2D>("Data/Textures/Skybox2.jpg");
        Material_sptr skyboxMaterial = std::make_shared<Material>(skyboxTexture, false);

        Shape_sptr skyboxShape = std::make_shared<Box>(1.0f, BoxUVPattern::Skybox);

        Object_sptr skyboxObject = std::make_shared<Object>(skyboxShape, skyboxMaterial);
        mScene.setSkybox(skyboxObject);
    }

    mCamera.getTransform()->translate(glm::vec3(0.0f, 10.0f, -10.0f));


  /*  ShapeCreateInfo shapeInfo = {};
    unsigned int id;
    shapeInfo.height = 2.0f;
    shapeInfo.radius = 1.0f;
    shapeInfo.slice = 50;
    shapeInfo.stack = 50;
    shapeInfo.half_x = 0.5f;
    shapeInfo.half_y = 0.5f;
    shapeInfo.half_z = 0.5f;
    shapeInfo.sourceType = ShapeSource::BOX;
    Shape_sptr coneShape = mScene.getNewShape(shapeInfo, &id);

    MaterialCreateInfo matInfo = {};
    matInfo.ambiantColor_Kd = glm::vec3(1.0f, 0.0f, 1.0f);
    Material_sptr material = mScene.getNewMaterial(matInfo, &id);

    ObjectCreateInfo objectInfo = {};
    objectInfo.material = material;
    objectInfo.shape = coneShape;
    mConeObject = mScene.getNewObject(objectInfo, &id);*/
   // mTransform.setTracking(true);
    //Mtl_Loader loader("Data/citroen_ds3/Citroen_DS3.mtl");

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

    float cameraRotSpeed = 0.005f;
    bool alt = mInputManager.getKeyModifier(GLFW_MOD_ALT);

    if(mInputManager.getFlagState(MOUSE_MOVED_FLAG)){
        if(!alt){
            mCamera.getTransform()->rotate((float)-mInputManager.getMouseMoveY()*cameraRotSpeed, AXIS_RIGHT);
            mCamera.getTransform()->rotate((float)-mInputManager.getMouseMoveX()*cameraRotSpeed, AXIS_UP, SpaceReference::WORLD);
        }else{
            #if TEST_CUBE == 1
                testCube->getTransform()->rotate((float)-mInputManager.getMouseMoveY()*cameraRotSpeed, AXIS_RIGHT);
                testCube->getTransform()->rotate((float)-mInputManager.getMouseMoveX()*cameraRotSpeed, AXIS_UP, SpaceReference::WORLD);
            #endif // TEST_CUBE

            mLights[1]->transform->rotate((float)-mInputManager.getMouseMoveY()*cameraRotSpeed, AXIS_RIGHT);
            mLights[1]->transform->rotate((float)-mInputManager.getMouseMoveX()*cameraRotSpeed, AXIS_UP, SpaceReference::WORLD);
        }

        mInputManager.resetFlags(MOUSE_MOVED_FLAG);
    }

    bool ctrl = mInputManager.getKeyModifier(GLFW_MOD_CONTROL);
    float cameraSpeed = 0.1f;
    float lightSpeed = 0.005f;
    if(mInputManager.getKey(GLFW_KEY_1)){
        glfwSetInputMode(mGLFWwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }if(mInputManager.getKey(GLFW_KEY_2)){
        glfwSetInputMode(mGLFWwindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    if(mInputManager.getKey(GLFW_KEY_F5)){
        glfwSetWindowMonitor(mGLFWwindow, glfwGetPrimaryMonitor(), 0, 0, 1920, 1080, 0);
        glfwSwapInterval(SWAP_INTERVAL);
    }/*if(mInputManager.getKey(GLFW_KEY_F6)){
        glfwSetWindowMonitor(mGLFWwindow, nullptr, 0, 0, 800, 600, 60);
        glfwSwapInterval(SWAP_INTERVAL);
    }*/

    if(mInputManager.getKey(GLFW_KEY_RIGHT)){
        if(!ctrl){
            if(!alt){
                mCamera.getTransform()->translate(AXIS_RIGHT*cameraSpeed);
            }else{
                mLights[1]->transform->translate(AXIS_RIGHT*lightSpeed);
            }
        }else{
            mLights[0]->transform->setTransformations(glm::rotate(glm::mat4(1.0f), -lightSpeed, glm::vec3(0.0f, 1.0f, 0.0f))*mLights[0]->transform->getTransformations());
        }
    }
    if(mInputManager.getKey(GLFW_KEY_LEFT)){
        if(!ctrl){
            if(!alt){
                mCamera.getTransform()->translate(AXIS_LEFT*cameraSpeed);
            }else{
                mLights[1]->transform->translate(AXIS_LEFT*lightSpeed);
            }
        }else{
            mLights[0]->transform->setTransformations(glm::rotate(glm::mat4(1.0f), lightSpeed, glm::vec3(0.0f, 1.0f, 0.0f))*mLights[0]->transform->getTransformations());
        }
    }
    if(mInputManager.getKey(GLFW_KEY_UP)){
        if(!ctrl){
            if(!alt){
                mCamera.getTransform()->translate(AXIS_FRONT*cameraSpeed);
            }else{
                mLights[1]->transform->translate(AXIS_FRONT*lightSpeed);
            }
        }else{
            mLights[0]->transform->setTransformations(glm::rotate(glm::mat4(1.0f), -lightSpeed, mLights[0]->transform->getWorldAxis(AXIS_RIGHT))*mLights[0]->transform->getTransformations());
        }
    }
    if(mInputManager.getKey(GLFW_KEY_DOWN)){
        if(!ctrl){
            if(!alt){
                mCamera.getTransform()->translate(AXIS_BACK*cameraSpeed);
            }else{
                mLights[1]->transform->translate(AXIS_BACK*lightSpeed);
            }
        }else{
            mLights[0]->transform->setTransformations(glm::rotate(glm::mat4(1.0f), lightSpeed, mLights[0]->transform->getWorldAxis(AXIS_RIGHT))*mLights[0]->transform->getTransformations());
        }
    }

    if(mInputManager.getFlagState(WIND0W_RESIZED_FLAG)){
        glfwGetWindowSize(mGLFWwindow, reinterpret_cast<int*>(&mWidth), reinterpret_cast<int*>(&mHeight));
        glViewport(0, 0, mWidth, mHeight);
        mCamera.setXRes(mWidth);
        mCamera.setYRes(mHeight);
        mInputManager.resetFlags(WIND0W_RESIZED_FLAG);
    }

    float moveSpeed = 0.02f;
    float rotSpeed = 0.005f;

    SpaceReference spaceReference = SpaceReference::LOCAL;
    if(alt){ spaceReference = SpaceReference::WORLD; }
    // UP
    if(mInputManager.getKey(GLFW_KEY_KP_ADD)){
        if(!ctrl){ mTransform->translate(glm::vec3(0.0f, moveSpeed, 0.0f), spaceReference); }
        else{ mTransform->rotate(rotSpeed, glm::vec3(0.0f, 1.0f, 0.0f), spaceReference); }
    //DOWN
    }if(mInputManager.getKey(GLFW_KEY_KP_SUBTRACT)){
        if(!ctrl){  mTransform->translate(glm::vec3(0.0f, -moveSpeed, 0.0f), spaceReference); }
        else{ mTransform->rotate(-rotSpeed, glm::vec3(0.0f, 1.0f, 0.0f), spaceReference); }
    }
    if(mInputManager.getKey(GLFW_KEY_KP_4)){
        if(!ctrl){ mTransform->translate(glm::vec3(-moveSpeed,0.0f, 0.0f), spaceReference); }
        else{ mTransform->rotate(-rotSpeed, glm::vec3(1.0f, 0.0f, 0.0f), spaceReference); }
    }
    if(mInputManager.getKey(GLFW_KEY_KP_6)){
        if(!ctrl){ mTransform->translate(glm::vec3(moveSpeed, 0.0f, 0.0f), spaceReference); }
        else{ mTransform->rotate(rotSpeed, glm::vec3(1.0f, 0.0f, 0.0f), spaceReference); }
    }
    if(mInputManager.getKey(GLFW_KEY_KP_8)){
        if(!ctrl){ mTransform->translate(AXIS_FRONT*moveSpeed, spaceReference); }
        else{ mTransform->rotate(rotSpeed, glm::vec3(0.0f, 0.0f, 1.0f), spaceReference); }
    }
    if(mInputManager.getKey(GLFW_KEY_KP_5)){
        if(!ctrl){ mTransform->translate(AXIS_BACK*moveSpeed, spaceReference);
        }else{ mTransform->rotate(-rotSpeed, glm::vec3(0.0f, 0.0f, 1.0f), spaceReference); }
    }
}

void MainApplication::tick(void){
    mScene.getSkybox()->getTransform()->setPosition(mCamera.getTransform()->getPosition());
}

void MainApplication::draw(void){
    if(mWidth == 0 || mHeight == 0)
        return;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   // for(unsigned int i(0); i < mFrameBuffers.size(); i++){
    //    mFrameBuffers[i]->bind();
    for(unsigned int i(0); i < mScene.getLightCount(); i++){
        Light_sptr light = mScene.getLight(i);
        OGL_FrameBuffer* fbo = &light->frameBuffer;
        fbo->bind();

        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

       /* glViewport(0, 0, mFrameBuffers[i]->getWidth(), mFrameBuffers[i]->getHeight());
        mCamera.setXRes(mFrameBuffers[i]->getWidth());
        mCamera.setYRes(mFrameBuffers[i]->getHeight());*/

        glViewport(0, 0, fbo->getWidth(), fbo->getHeight());
        mCamera.setXRes(fbo->getWidth());
        mCamera.setYRes(fbo->getHeight());

        mShadowMapping->draw(&mScene, light.get());
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, mWidth, mHeight);
    mCamera.setXRes(mWidth);
    mCamera.setYRes(mHeight);

    mPhongRender->draw(&mScene, &mCamera);
   glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    WireShader->bind();
        glm::mat4 mvp = LIGHT_SPOT_PROJECTION(mLights[1]->angle)*glm::lookAt(mLights[1]->transform->getPosition(),mLights[1]->transform->getPosition()+mLights[1]->transform->getWorldAxis(AXIS_FRONT),
                                                mLights[1]->transform->getWorldAxis(AXIS_UP))*mLights[1]->transform->getTransformations();
        mvp = mCamera.getProjectionMatrix()*mCamera.getViewMatrix()*mLights[1]->transform->getTransformations();
        WireShader->setUniform(WireShader->getUniformLocation("mvp"), mvp);
        //CameraShape->draw();
    WireShader->unbind();
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glfwSwapBuffers(mGLFWwindow);
}
