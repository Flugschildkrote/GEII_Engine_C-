#include "Render.h"
#include "geException.h"
#include "OGL_Shader.h"
#include "Scene.h"
#include "Object.h"
#include "Material.h"
#include "OGL_Texture.h"
#include "Shape.h"
#include "Camera.h"

/**************[RENDER]***************************************/
Render::Render(void) : mShaderProgram(nullptr){

}

Render::~Render(void) { }
GLint Render::getUniform(const std::string &uniformName, OGL_ShaderProgram* shader) const{
    GLint result = shader->getUniformLocation(uniformName);
    if(result == GE_UNKNOWN_UNIFORM){
        throw geException("Failed to find uniform \""+uniformName+"\"");
    }
    return result;
}



/**************[RENDER_PHONG]***************************************/
RenderPhong::RenderPhong(void) : Render(){

   // mShadowMappingRender = std::make_unique<RenderShadowMapping>();
    // Standard draw shader
    ShaderCreateInfo vertexInfo = {};
    vertexInfo.fromFile_nFromMemory = true;
    vertexInfo.sourceFile = "Data/Shaders/Phong.vert";

    ShaderCreateInfo fragInfo = {};
    fragInfo.fromFile_nFromMemory = true;
    fragInfo.sourceFile = "Data/Shaders/Phong.frag";

    OGL_ShaderProgramCreateInfo shaderInfo = {};
    shaderInfo.fragInfo = &fragInfo;
    shaderInfo.vertInfo = &vertexInfo;

    mShaderProgram = std::make_unique<OGL_ShaderProgram>(shaderInfo);

    vertexInfo.sourceFile = "Data/Shaders/Skybox.vert";
    fragInfo.sourceFile = "Data/Shaders/Skybox.frag";
    mSkyboxShader = std::make_unique<OGL_ShaderProgram>(shaderInfo);

    initUniforms();
}

RenderPhong::~RenderPhong() { }

void RenderPhong::initUniforms(void){
    mShaderProgram->bind(true);
    OGL_ShaderProgram *shader = mShaderProgram.get();
    /**[MATERIAL]**/
    mU_MatKd = getUniform("color_Kd", shader);
    mU_MatKs = getUniform("color_Ks", shader);
    mU_MatNs = getUniform("color_Ns", shader);
    mU_MatAlpha = getUniform("color_Alpha", shader);
    mU_MatText = getUniform("texture_sampler", shader);
    mU_MatUseTexture = getUniform("useTexture", shader);
    mU_MatLightSensitive = getUniform("lightSensitive", shader);

    /**[LIGHT]**/
    mU_ShadowMap = getUniform("shadowMap", shader);
    mU_LightMatrix = getUniform("light_matrix", shader);
    mU_LightDir = getUniform("lightDir", shader);
    mU_LightPower= getUniform("lightIntensity", shader);
    mU_LightAmbiant = getUniform("lightColor", shader);

    /**[MODEL]**/
    mU_ModelMatrix = getUniform("model_matrix", shader);

    /**[CAMERA]**/
    mU_MVP = getUniform("mvp", shader);
    mU_WorldEyePos = getUniform("worldeye_pos", shader);

    mSkyboxShader->bind(true);
    shader = mSkyboxShader.get();
    mU_Sky_MVP = getUniform("mvp", shader);
    mU_Sky_Texture = getUniform("texture_sampler", shader);
    mSkyboxShader->bind(false);
}

void RenderPhong::draw(Scene *scene, Camera *camera, Light* light, Texture_sptr shadowMap) const{
    bool skyboxSaveState = false;
///#########################[RENDU DE LA SKYBOX]############################################
    if(scene->hasSkybox()){
        Object_sptr skybox = scene->getSkybox();
        skyboxSaveState =  skybox->isEnabled();
        if(skyboxSaveState){
            mSkyboxShader->bind(true);
            glm::mat4 mvp = camera->getProjectionMatrix()*camera->getViewMatrix()*skybox->getTransform()->getTransformations();

            mSkyboxShader->setUniform(mU_Sky_MVP, mvp);
            mSkyboxShader->setUniformTexture(mU_Sky_Texture, 0);
            glActiveTexture(GL_TEXTURE0);

            glBindTexture(GL_TEXTURE_2D, skybox->getMaterial()->mTexture->getID());
                skybox->getShape()->draw();
            glBindTexture(GL_TEXTURE_2D, 0);
            glClear(GL_DEPTH_BUFFER_BIT);
            skybox->setEnabled(false);
        }
    }

///#########################[RENDU DE LA SCENE]################################################
    mShaderProgram->bind(true);

    // Définition de la shadowMap
    mShaderProgram->setUniformTexture(mU_ShadowMap, 1); // Shadow map sur la texture n1
    glActiveTexture(GL_TEXTURE1);
    shadowMap->bind(true);
    /**[LIGHT**/
    mShaderProgram->setUniform(mU_LightDir,     light->transform->getWorldAxis(AXIS_FRONT));
    mShaderProgram->setUniform(mU_LightPower,   light->power);
    mShaderProgram->setUniform(mU_LightAmbiant, light->ambiantColor);

//Le point de vue
    mShaderProgram->setUniform(mU_WorldEyePos, glm::normalize(camera->mTransform->getWorldAxis(AXIS_FRONT)));
//Fixe l'unitee de texture
    mShaderProgram->setUniformTexture(mU_MatText, 0);
    glActiveTexture(GL_TEXTURE0);

//Pour chaque primitive
    int nbprim=scene->getObjectsCount();
    int nbtprim=scene->getTranparentObjectsCount();

//Les primitives non transparentes
    mShaderProgram->setUniform(mU_MatAlpha,1.0f);
    while(nbprim>nbtprim)
    {
        nbprim--;
        Object_sptr object = scene->getObject(nbprim);
        if(!object->isEnabled()){
            continue;
        }
        //Fixe la matrice mvp
        glm::mat4 mvp, modelMatrix, lightMatrix;
        modelMatrix = object->getTransform()->getTransformations();
        mvp = camera->getProjectionMatrix()*camera->getViewMatrix()*modelMatrix;
        lightMatrix = LIGHT_PROJECTION*glm::lookAt(light->transform->getPosition(),light->transform->getPosition()+light->transform->getWorldAxis(AXIS_FRONT), light->transform->getWorldAxis(AXIS_UP))*modelMatrix;

        mShaderProgram->setUniform(mU_LightMatrix, lightMatrix);
        mShaderProgram->setUniform(mU_ModelMatrix, modelMatrix);
        mShaderProgram->setUniform(mU_MVP, mvp);

        //Fixe les parametres du materiau
        Material_sptr material = object->getMaterial();

        mShaderProgram->setUniform(mU_MatKd, material->mAmbiantColor_Kd);
        mShaderProgram->setUniform(mU_MatKs, material->mSpecularColor_Ks);
        mShaderProgram->setUniform(mU_MatNs, material->mSpecularExponent_Ns);
        mShaderProgram->setUniform(mU_MatLightSensitive, material->mLightSensitive);

        Texture_sptr texture = material->mTexture;
        if(texture){
            mShaderProgram->setUniform(mU_MatUseTexture, true);
            texture->bind(true);
        }else{
            mShaderProgram->setUniform(mU_MatUseTexture, false);
        }
        Shape_sptr  objectShape = object->getShape();
        objectShape->draw();
    }

//Les primitives transparentes
    glEnable(GL_BLEND);
    glDepthMask(GL_FALSE);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    while(nbtprim--)
    {
        Object_sptr object = scene->getObject(nbtprim);
        if(!object->isEnabled()){
            continue;
        }
        //Fixe la matrice mvp
        glm::mat4 mvp, modelMatrix;
        modelMatrix = object->getTransform()->getTransformations();
        mvp = camera->getProjectionMatrix()*camera->getViewMatrix()*modelMatrix;
        mShaderProgram->setUniform(mU_ModelMatrix, modelMatrix);
        mShaderProgram->setUniform(mU_MVP, mvp);

        //Fixe les parametres du materiau
        Material_sptr material = object->getMaterial();
        mShaderProgram->setUniform(mU_MatKd, material->mAmbiantColor_Kd);
        mShaderProgram->setUniform(mU_MatKs, material->mSpecularColor_Ks);
        mShaderProgram->setUniform(mU_MatNs, material->mSpecularExponent_Ns);
        mShaderProgram->setUniform(mU_MatAlpha, material->mTransparency_Alpha);
       // std::cout << material->mTransparency_Alpha << std::endl;

         Texture_sptr texture = material->mTexture;
        if(texture){
            mShaderProgram->setUniform(mU_MatUseTexture, true);
            texture->bind(true);
        }else{
            mShaderProgram->setUniform(mU_MatUseTexture, false);
        }

        Shape_sptr  objectShape = object->getShape();
        objectShape->draw();
    }
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    // Si la skybox était activée on la réactive
    if(skyboxSaveState){
        scene->getSkybox()->setEnabled(true);
    }

    mShaderProgram->bind(false);

}

RenderShadowMapping::RenderShadowMapping(void) : Render(){
    ShaderCreateInfo vertexInfo = {};
    vertexInfo.fromFile_nFromMemory = true;
    vertexInfo.sourceFile = "Data/Shaders/ShadowMapping.vert";

    ShaderCreateInfo fragInfo = {};
    fragInfo.fromFile_nFromMemory = true;
    fragInfo.sourceFile = "Data/Shaders/ShadowMapping.frag";

    OGL_ShaderProgramCreateInfo shaderInfo = {};
    shaderInfo.fragInfo = &fragInfo;
    shaderInfo.vertInfo = &vertexInfo;


    mShaderProgram = std::make_unique<OGL_ShaderProgram>(shaderInfo);
    initUniforms();
}

RenderShadowMapping::~RenderShadowMapping(void){ }

void RenderShadowMapping::draw(Scene* scene, Light *light){
    bool skyboxSaveState = false;
    if(scene->hasSkybox()){
        Object_sptr skybox = scene->getSkybox();
        skyboxSaveState =  skybox->isEnabled();
        skybox->setEnabled(false);
    }

    mShaderProgram->bind(true);

    int nbprim=scene->getObjectsCount();

    for(int i(0); i < nbprim; i++){
        Object_sptr object = scene->getObject(i);
        if(!object->isEnabled()){
            continue;
        }
        glm::mat4 mvp, modelMatrix;
        modelMatrix = object->getTransform()->getTransformations();
        glm::mat4 proj = LIGHT_PROJECTION;
        glm::mat4 view = glm::lookAt(light->transform->getPosition(),light->transform->getPosition()+light->transform->getWorldAxis(AXIS_FRONT), light->transform->getWorldAxis(AXIS_UP));

        mvp = proj*view*modelMatrix;
        mShaderProgram->setUniform(mU_MVP, mvp);
        object->getShape()->draw();
    }

    // Si la skybox était activée on la réactive
    if(skyboxSaveState){
        scene->getSkybox()->setEnabled(true);
    }

    mShaderProgram->bind(false);
}

void RenderShadowMapping::initUniforms(void){
    mShaderProgram->bind(true);
        mU_MVP = getUniform("mvp", mShaderProgram.get());
    mShaderProgram->bind(false);
}
