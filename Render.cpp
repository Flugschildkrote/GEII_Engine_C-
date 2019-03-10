#include "Render.h"
#include "geException.h"
#include "OGL_Shader.h"
#include "Scene.h"
#include "Object.h"
#include "Material.h"
#include "Texture2D.h"
#include "Shape.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

namespace GEII
{
/**************[RENDER]***************************************/
    Render::Render(void) : mShaderProgram(nullptr){

    }

    Render::~Render(void) { }
    GLint Render::getUniform(const std::string &uniformName, OGL_ShaderProgram* shader) const{
        GLint result = shader->getUniformLocation(uniformName);
        if(result == GE_UNKNOWN_UNIFORM){
            throw geException("Failed to find uniform \""+uniformName+"\"");
        }
        DEBUG_BLOCK(std::cerr << "Uniform \"" << uniformName << "\" : success, ID = " << result << std::endl;)
        return result;
    }



    /**************[RENDER_PHONG]***************************************/
    RenderPhong::RenderPhong(void) : Render(){
        mShaderProgram = std::make_unique<OGL_ShaderProgram>("Data/Shaders/Phong.vert", "Data/Shaders/Phong.frag");
        mSkyboxShader = std::make_unique<OGL_ShaderProgram>( "Data/Shaders/Skybox.vert", "Data/Shaders/Skybox.frag");
        initUniforms();
    }

    RenderPhong::~RenderPhong() { }

    void RenderPhong::initUniforms(void){
        mShaderProgram->bind();
        OGL_ShaderProgram *shader = mShaderProgram.get();
        /**[MATERIAL]**/
        mU_MatKd = getUniform("color_Kd", shader);
        mU_MatKs = getUniform("color_Ks", shader);
        mU_MatNs = getUniform("color_Ns", shader);
        mU_MatAlpha = getUniform("color_Alpha", shader);
        mU_MatText = getUniform("texture_sampler", shader);
        mU_MatUseTexture = getUniform("useTexture", shader);
        mU_MatLightSensitive = getUniform("lightSensitive", shader);
        mU_MatUseNormalMapping = getUniform("useNormalMap", shader);
        mU_MatNormalMap = getUniform("normalMap", shader);

        /**[LIGHT]**/
        mSceneAmbiantColor = getUniform("sceneAmbiantColor", shader);
        for(unsigned int i(0); i < MAX_LIGHTS; i++){
            std::string num = std::to_string(i);
            mU_LightMatrix[i] = getUniform("light_matrix["+num+"]", shader);
            mU_ShadowMap[i] = getUniform("lights["+num+"].shadowMap", shader);
            mU_LightDir[i] = getUniform("lights["+num+"].dir", shader);
            mU_LightPower[i] = getUniform("lights["+num+"].intensity", shader);
            mU_LightAmbiant[i] = getUniform("lights["+num+"].color", shader);
            mU_LightType[i] = getUniform("lights["+num+"].type", shader);
            mU_LightPos[i] = getUniform("lights["+num+"].pos", shader);
            mU_LightAngle[i] = getUniform("lights["+num+"].angle", shader);
        }
        mU_LightCount = getUniform("lightCount", shader);

        /**[MODEL]**/
        mU_ModelMatrix = getUniform("model_matrix", shader);

        /**[CAMERA]**/
        mU_MVP = getUniform("mvp", shader);
        mU_WorldEyePos = getUniform("worldeye_pos", shader);

        mSkyboxShader->bind();
        shader = mSkyboxShader.get();
        mU_Sky_MVP = getUniform("mvp", shader);
        mU_Sky_Texture = getUniform("texture_sampler", shader);
        mSkyboxShader->unbind();
    }

    void RenderPhong::draw(Scene *scene, Camera *camera) const{
    ///#########################[RENDU DE LA SKYBOX]############################################
        Object_sptr skybox = scene->getSkybox();
        if(skybox){
            mSkyboxShader->bind();
            glm::mat4 mvp = camera->getProjectionMatrix()*camera->getViewMatrix()*glm::translate(glm::mat4(1.0f), camera->getTransform()->getPosition());
            mSkyboxShader->setUniform(mU_Sky_MVP, mvp);
            mSkyboxShader->setUniformTexture(mU_Sky_Texture, 0);
            glActiveTexture(GL_TEXTURE0);

            glBindTexture(GL_TEXTURE_2D, skybox->getMaterial()->getDiffuseTexture()->getID());
                skybox->getShape()->draw();
            glBindTexture(GL_TEXTURE_2D, 0);
            glClear(GL_DEPTH_BUFFER_BIT);
        }

    ///#########################[RENDU DE LA SCENE]################################################
        mShaderProgram->bind();

        // Définition de la shadowMap
        mShaderProgram->setUniform(mSceneAmbiantColor, scene->getAmbiantColor());
        for(unsigned int i(0); i < scene->getLightCount(); i++){
            Light* light = scene->getLight(i).get();
            mShaderProgram->setUniformTexture(mU_ShadowMap[i], i+1); // Shadow map sur la texture n1
            glActiveTexture(GL_TEXTURE1+i);
            //lights[i]->shadowMap->bind();
            light->frameBuffer.getDepthTexture()->bind();
            /**[LIGHT**/
            mShaderProgram->setUniform(mU_LightDir[i],     light->transform->getWorldAxis(AXIS_FRONT));
            mShaderProgram->setUniform(mU_LightPower[i],   light->power);
            mShaderProgram->setUniform(mU_LightAmbiant[i], light->ambiantColor);
            mShaderProgram->setUniform(mU_LightType[i],    light->type);
            mShaderProgram->setUniform(mU_LightPos[i],    light->transform->getPosition());
            mShaderProgram->setUniform(mU_LightAngle[i],   light->angle);
        }
        mShaderProgram->setUniform(mU_LightCount, (int) scene->getLightCount());
    //Le point de vue
        mShaderProgram->setUniform(mU_WorldEyePos, glm::normalize(camera->getTransform()->getPosition()));
    //Fixe l'unitee de texture
        mShaderProgram->setUniformTexture(mU_MatText, 0);
        glActiveTexture(GL_TEXTURE0);
        mShaderProgram->setUniformTexture(mU_MatNormalMap, 1);

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

            for(unsigned int i(0); i < scene->getLightCount(); i++){
                Light* light = scene->getLight(i).get();
                glm::mat4 lightProj;
                switch(light->type){
                case LIGHT_SUN :
                    lightProj = LIGHT_SUN_PROJECTION;
                    break;
                case LIGHT_SPOT :
                    lightProj = LIGHT_SPOT_PROJECTION(light->angle);
                    break;
                default :
                    break;
                }

                lightMatrix = lightProj*glm::lookAt(light->transform->getPosition(),light->transform->getPosition()+light->transform->getWorldAxis(AXIS_FRONT),
                                                    light->transform->getWorldAxis(AXIS_UP))*modelMatrix;
                mShaderProgram->setUniform(mU_LightMatrix[i], lightMatrix);
            }

            mShaderProgram->setUniform(mU_ModelMatrix, modelMatrix);
            mShaderProgram->setUniform(mU_MVP, mvp);

            //Fixe les parametres du materiau
            Material_sptr material = object->getMaterial();

            mShaderProgram->setUniform(mU_MatKd, material->getDiffuseColor());
            mShaderProgram->setUniform(mU_MatKs, material->getSpecularColor());
            mShaderProgram->setUniform(mU_MatNs, material->getSpecularExponent());
            mShaderProgram->setUniform(mU_MatLightSensitive, material->isLightSensitive());

            // Texture classique
            Texture_sptr texture = material->getDiffuseTexture();
            if(texture){
                mShaderProgram->setUniform(mU_MatUseTexture, true);
                glActiveTexture(GL_TEXTURE0);
                texture->bind();
            }else{
                mShaderProgram->setUniform(mU_MatUseTexture, false);
            }

            // Texture de normale
            texture = material->getNormalMap();
            if(texture){
                mShaderProgram->setUniform(mU_MatUseNormalMapping, true);
                glActiveTexture(GL_TEXTURE1);
                texture->bind();
            }else{
                mShaderProgram->setUniform(mU_MatUseNormalMapping, false);
            }
            Shape_sptr  objectShape = object->getShape();
            objectShape->draw();
        }

    //Les primitives transparentes
        glEnable(GL_BLEND);
        glDepthMask(GL_FALSE);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        while(nbtprim--)
        {
            Object_sptr object = scene->getObject(nbtprim);
            if(!object->isEnabled()){
                continue;
            }

             //Fixe la matrice mvp
            glm::mat4 mvp, modelMatrix, lightMatrix;
            modelMatrix = object->getTransform()->getTransformations();
            mvp = camera->getProjectionMatrix()*camera->getViewMatrix()*modelMatrix;

            for(unsigned int i(0); i < scene->getLightCount(); i++){
                Light* light = scene->getLight(i).get();
                glm::mat4 lightProj;
                switch(light->type){
                case LIGHT_SUN :
                    lightProj = LIGHT_SUN_PROJECTION;
                    break;
                case LIGHT_SPOT :
                    lightProj = LIGHT_SPOT_PROJECTION(light->angle);
                    break;
                default :
                    break;
                }

                lightMatrix = lightProj*glm::lookAt(light->transform->getPosition(),light->transform->getPosition()+light->transform->getWorldAxis(AXIS_FRONT),
                                                    light->transform->getWorldAxis(AXIS_UP))*modelMatrix;
                mShaderProgram->setUniform(mU_LightMatrix[i], lightMatrix);
            }


            mShaderProgram->setUniform(mU_ModelMatrix, modelMatrix);
            mShaderProgram->setUniform(mU_MVP, mvp);

            //Fixe les parametres du materiau
            Material_sptr material = object->getMaterial();

            mShaderProgram->setUniform(mU_MatKd, material->getDiffuseColor());
            mShaderProgram->setUniform(mU_MatKs, material->getSpecularColor());
            mShaderProgram->setUniform(mU_MatNs, material->getSpecularExponent());
            mShaderProgram->setUniform(mU_MatLightSensitive, material->isLightSensitive());

            // Texture classique
            Texture_sptr texture = material->getDiffuseTexture();
            if(texture){
                mShaderProgram->setUniform(mU_MatUseTexture, true);
                glActiveTexture(GL_TEXTURE0);
                texture->bind();
            }else{
                mShaderProgram->setUniform(mU_MatUseTexture, false);
            }

            // Texture de normale
            texture = material->getNormalMap();
            if(texture){
                mShaderProgram->setUniform(mU_MatUseNormalMapping, true);
                glActiveTexture(GL_TEXTURE1);
                texture->bind();
            }else{
                mShaderProgram->setUniform(mU_MatUseNormalMapping, false);
            }
            Shape_sptr  objectShape = object->getShape();
            objectShape->draw();
        }
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        mShaderProgram->unbind();

    }





    RenderShadowMapping::RenderShadowMapping(void) : Render(){
        mShaderProgram = std::make_unique<OGL_ShaderProgram>("Data/Shaders/ShadowMapping.vert", "Data/Shaders/ShadowMapping.frag");
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

        mShaderProgram->bind();

        int nbprim=scene->getObjectsCount();

        for(int i(0); i < nbprim; i++){
            Object_sptr object = scene->getObject(i);
            if(!object->isEnabled()){
                continue;
            }
            glm::mat4 mvp, modelMatrix;
            modelMatrix = object->getTransform()->getTransformations();

            glm::mat4 proj;
            switch(light->type){
            case LIGHT_SUN :
                proj = LIGHT_SUN_PROJECTION;
                break;
            case LIGHT_SPOT :
                proj = LIGHT_SPOT_PROJECTION(light->angle);
                break;
            default :
                break;
            }

            glm::mat4 view = glm::lookAt(light->transform->getPosition(),light->transform->getPosition()+light->transform->getWorldAxis(AXIS_FRONT), light->transform->getWorldAxis(AXIS_UP));

            mvp = proj*view*modelMatrix;
            mShaderProgram->setUniform(mU_MVP, mvp);
            object->getShape()->draw();
        }

        // Si la skybox était activée on la réactive
        if(skyboxSaveState){
            scene->getSkybox()->setEnabled(true);
        }

        mShaderProgram->unbind();
    }

    void RenderShadowMapping::initUniforms(void){
        mShaderProgram->bind();
            mU_MVP = getUniform("mvp", mShaderProgram.get());
        mShaderProgram->unbind();
    }
}
