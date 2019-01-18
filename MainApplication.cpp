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
#include "Light.h"

Shape_sptr CameraShape;
OGL_ShaderProgram_sptr WireShader;
Object_sptr testCube;

MainApplication::MainApplication(const std::string &appName, unsigned int w, unsigned int h) : mName(appName), mWidth(w), mHeight(h), mGLFWwindow(nullptr), mScene(10,10,10,10),
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
        tmpLights[0]->ambiantColor = glm::vec3(0.5f, 0.5f, 0.5f);
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

        { // Frame Buffer
            unsigned int id;
            int fboSize = 2024;
            OGL_TextureCreateInfo texInfo = {};
            texInfo.fromFile_nFromBuffer = false;
            texInfo.sourceFile = "Data/Textures/earth_1024.bmp";
            texInfo.genMipMaps = false;
            texInfo.width = fboSize;
            texInfo.height = fboSize;
            texInfo.wrapS = GL_CLAMP_TO_BORDER;
            texInfo.wrapT = GL_CLAMP_TO_BORDER;
            texInfo.format = GL_DEPTH_COMPONENT;
            texInfo.pixelType = GL_FLOAT;
            Texture_sptr depthTexture1 = mScene.getNewTexture(texInfo, &id);
           /* texInfo.magFilter = GL_LINEAR;
            texInfo.minFilter = GL_LINEAR;*/
           // texInfo.borderColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
            Texture_sptr depthTexture2 = mScene.getNewTexture(texInfo, &id);

            AttachmentData attachmentData = {};
            attachmentData.attachment = GL_DEPTH_ATTACHMENT;
            attachmentData.textureID = depthTexture1->getID();

            FrameBuffer_CreateInfo frameBufferInfo = {};
            frameBufferInfo.flags = FRAMEBUFFER_DISABLE_READ | FRAMEBUFFER_DISABLE_WRITE;
            frameBufferInfo.attachments.push_back(attachmentData);
            frameBufferInfo.width = fboSize;
            frameBufferInfo.height = fboSize;
            mFrameBuffers.push_back(std::make_unique<OGL_FrameBuffer>(frameBufferInfo));

            attachmentData.textureID = depthTexture2->getID();
            frameBufferInfo.attachments[0] = attachmentData;
            mFrameBuffers.push_back(std::make_unique<OGL_FrameBuffer>(frameBufferInfo));

            tmpLights[0]->shadowMap = depthTexture1;
            tmpLights[1]->shadowMap = depthTexture2;
            //mShadowMaps.push_back(depthTexture1);
            //mShadowMaps.push_back(depthTexture2);

            /** Carré pour afficher la texture de profondeur **/
            MaterialCreateInfo matInfo = {};
            matInfo.texture = depthTexture2;
            matInfo.lightSensitive = false;
            Material_sptr objectMaterial = mScene.getNewMaterial(matInfo, &id);
            matInfo.texture = depthTexture1;
            Material_sptr objectMaterial2 = mScene.getNewMaterial(matInfo, &id);

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
            object->getTransform()->rotate(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            object->getTransform()->translate(glm::vec3(-4.999, 5.0f, -2.1), SpaceReference::WORLD);
            objectInfo.material = objectMaterial2;
            object = mScene.getNewObject(objectInfo, &id);
            object->getTransform()->rotate(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            object->getTransform()->translate(glm::vec3(-4.999, 5.0f, 2.1), SpaceReference::WORLD);
        }
    }

    { // Création du cube de test
        unsigned int id;
        ShapeCreateInfo shapeInfo = {};
        shapeInfo.half_x = 2;
        shapeInfo.half_y = 2;
        shapeInfo.half_z = 2;
        shapeInfo.sourceType = ShapeSource::BOX;
        Shape_sptr shape = mScene.getNewShape(shapeInfo, &id);

        OGL_TextureCreateInfo textureInfo;
        textureInfo.fromFile_nFromBuffer = true;
        textureInfo.sourceFile = "Data/Textures/normal1.png";
        textureInfo.magFilter = GL_LINEAR;
        Texture_sptr normalMap= mScene.getNewTexture(textureInfo, &id);

        textureInfo.sourceFile = "Data/Textures/Rock_028_SD/Rock_028_COLOR.jpg";
        Texture_sptr diffuseMap = mScene.getNewTexture(textureInfo, &id);

        MaterialCreateInfo materialInfo = {};
        materialInfo.ambiantColor_Kd = glm::vec3(0.7f, 0.7f, 0.7f);
        materialInfo.lightSensitive = true;
        materialInfo.specularColor_Ks = glm::vec3(0.5f, 0.5f, 0.5f);
        materialInfo.specularExponent_Ns = 5.0f;
        //materialInfo.texture = diffuseMap;
        materialInfo.normalMap = normalMap;
        Material_sptr material = mScene.getNewMaterial(materialInfo, &id);

        ObjectCreateInfo objectInfo = {};
        objectInfo.material = material;
        objectInfo.shape = shape;
        testCube = mScene.getNewObject(objectInfo, &id);
    }

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
    mConeObject = mScene.getObject(1);
    mTransform = mConeObject->getTransform();
   // mLights[1]->transform = mTransform;

    {
        MaterialCreateInfo matInfo = {};
        matInfo.lightSensitive = false;
        matInfo.ambiantColor_Kd = glm::vec3(1.0f, 0.5f, 0.5f);
        unsigned int id;
        Material_sptr myMaterial  = mScene.getNewMaterial(matInfo, &id);
        ShapeCreateInfo shapeInfo = {};
        shapeInfo.sourceType = ShapeSource::BOX;
        shapeInfo.half_x = 0.02f;
        shapeInfo.half_y = 0.02f;
        shapeInfo.half_z = 0.02f;
        Shape_sptr shape = mScene.getNewShape(shapeInfo, &id);
        ObjectCreateInfo objecInfo = {};
        objecInfo.material = myMaterial;
        objecInfo.shape = shape;
        Object_sptr object = mScene.getNewObject(objecInfo, &id);
        //mLights[1]->transform = mCamera.mTransform;
        object->setTransform(mLights[1]->transform);
    }

    {
        ShapeCreateInfo createInfo = {};
        createInfo.sourceType = ShapeSource::BOX;
        CameraShape = std::make_shared<Shape>(createInfo);
        CameraShape->loadCamera(mLights[1]->angle, 1.0f, 0.1f, 100.0f);
        ShaderCreateInfo vShaderInfo = {};
        vShaderInfo.fromFile_nFromMemory = true;
        vShaderInfo.sourceFile = "Data/Shaders/Simple.vert";
        ShaderCreateInfo fShaderInfo = {};
        fShaderInfo.fromFile_nFromMemory = true;
        fShaderInfo.sourceFile = "Data/Shaders/Simple.frag";
        OGL_ShaderProgramCreateInfo programInfo = {};
        programInfo.fragInfo = &fShaderInfo;
        programInfo.vertInfo = &vShaderInfo;
        WireShader = std::make_shared<OGL_ShaderProgram>(programInfo);
    }


    //myObjLoader.loadFile("Data/citroen_ds3/Citroen_DS3.obj", &mScene);



   // mCamera.mPos = glm::vec3(0,8,15);
    //mCamera.mLookPos = glm::vec3(0.0f, 0.0f, 0.0f);
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

    mCamera.mTransform->translate(glm::vec3(0.0f, 10.0f, -10.0f));


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
            mCamera.mTransform->rotate((float)-mInputManager.getMouseMoveY()*cameraRotSpeed, AXIS_RIGHT);
            mCamera.mTransform->rotate((float)-mInputManager.getMouseMoveX()*cameraRotSpeed, AXIS_UP, SpaceReference::WORLD);
        }else{
            testCube->getTransform()->rotate((float)-mInputManager.getMouseMoveY()*cameraRotSpeed, AXIS_RIGHT);
            testCube->getTransform()->rotate((float)-mInputManager.getMouseMoveX()*cameraRotSpeed, AXIS_UP, SpaceReference::WORLD);
            mLights[1]->transform->rotate((float)-mInputManager.getMouseMoveY()*cameraRotSpeed, AXIS_RIGHT);
            mLights[1]->transform->rotate((float)-mInputManager.getMouseMoveX()*cameraRotSpeed, AXIS_UP, SpaceReference::WORLD);
        }

        mInputManager.resetFlags(MOUSE_MOVED_FLAG);
    }

    bool ctrl = mInputManager.getKeyModifier(GLFW_MOD_CONTROL);
    float cameraSpeed = 0.02f;
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
                mCamera.mTransform->translate(AXIS_RIGHT*cameraSpeed);
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
                mCamera.mTransform->translate(AXIS_LEFT*cameraSpeed);
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
                mCamera.mTransform->translate(AXIS_FRONT*cameraSpeed);
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
                mCamera.mTransform->translate(AXIS_BACK*cameraSpeed);
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
        mCamera.mXres = mWidth;
        mCamera.mYres = mHeight;
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
    mScene.getSkybox()->getTransform()->setPosition(mCamera.mTransform->getPosition());
}

void MainApplication::draw(void){
    if(mWidth == 0 || mHeight == 0)
        return;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(unsigned int i(0); i < mFrameBuffers.size(); i++){
        mFrameBuffers[i]->bind(true);
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, 0, mFrameBuffers[i]->getWidth(), mFrameBuffers[i]->getHeight());
        mCamera.mXres = mFrameBuffers[i]->getWidth();
        mCamera.mYres = mFrameBuffers[i]->getHeight();

        mShadowMapping->draw(&mScene, mLights[i].get());
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, mWidth, mHeight);
    mCamera.mXres = mWidth;
    mCamera.mYres = mHeight;

    mPhongRender->draw(&mScene, &mCamera, mLights);
   glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    WireShader->bind(true);
        glm::mat4 mvp = LIGHT_SPOT_PROJECTION(mLights[1]->angle)*glm::lookAt(mLights[1]->transform->getPosition(),mLights[1]->transform->getPosition()+mLights[1]->transform->getWorldAxis(AXIS_FRONT),
                                                mLights[1]->transform->getWorldAxis(AXIS_UP))*mLights[1]->transform->getTransformations();
        mvp = mCamera.getProjectionMatrix()*mCamera.getViewMatrix()*mLights[1]->transform->getTransformations();
        WireShader->setUniform(WireShader->getUniformLocation("mvp"), mvp);
        CameraShape->draw();
    WireShader->bind(false);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glfwSwapBuffers(mGLFWwindow);
}
