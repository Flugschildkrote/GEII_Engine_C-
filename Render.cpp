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
    mLight.ambiantColor = glm::vec3(0.5f, 0.5f, 0.5f);
    mLight.position = glm::vec3(0.0f, 0.0f, -20.0f);
    mLight.power = glm::vec3(1.0f, 1.0f, 1.0f);

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

    /**[LIGHT]**/
    mU_LightPos = getUniform("lightPos", shader);
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

void RenderPhong::update(Camera *camera){
    mLight.position = camera->mPos;
}

void RenderPhong::draw(Scene *scene, Camera *camera) const{
    bool skyboxSaveState = false;
    if(scene->hasSkybox()){
        Object_sptr skybox = scene->getSkybox();
        skyboxSaveState =  skybox->isEnabled();
        if(skyboxSaveState){
            mSkyboxShader->bind(true);
            glm::mat4 mvp = camera->getProjectionMatrix()*camera->getViewMatrix()*skybox->getTransformations();

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

    mShaderProgram->bind(true);

    /**[LIGHT**/
    mShaderProgram->setUniform(mU_LightPos,     mLight.position);
    mShaderProgram->setUniform(mU_LightPower,   mLight.power);
    mShaderProgram->setUniform(mU_LightAmbiant, mLight.ambiantColor);

//Le point de vue
    mShaderProgram->setUniform(mU_WorldEyePos, glm::normalize(camera->getLookDir()));
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
        glm::mat4 mvp, modelMatrix;
        modelMatrix = object->getTransformations();
        mvp = camera->getProjectionMatrix()*camera->getViewMatrix()*modelMatrix;

        mShaderProgram->setUniform(mU_ModelMatrix, modelMatrix);
        mShaderProgram->setUniform(mU_MVP, mvp);

        //Fixe les parametres du materiau
        Material_sptr material = object->getMaterial();

        mShaderProgram->setUniform(mU_MatKd, material->mAmbiantColor_Kd);
        mShaderProgram->setUniform(mU_MatKs, material->mSpecularColor_Ks);
        mShaderProgram->setUniform(mU_MatNs, material->mSpecularExponent_Ns);

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
        modelMatrix = object->getTransformations();
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
