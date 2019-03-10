#include "RenderPicking.h"
#include "Scene.h"
#include "Object.h"
#include "Shape.h"
#include "Camera.h"
#include "OGL_Shader.h"

namespace GEII
{
    RenderPicking::RenderPicking(void){
        mShaderProgram = std::make_unique<GEII::OGL_ShaderProgram>("Data/Shaders/Picking.vert", "Data/Shaders/Picking.frag");
        initUniforms();
        //ctor
    }

    RenderPicking::~RenderPicking(void) { }


    void RenderPicking::draw(Scene* scene, Camera* camera) const {
        bool skyboxSaveState = false;

        mShaderProgram->bind();

        if(scene->hasSkybox()){
            Object_sptr skybox = scene->getSkybox();
            skyboxSaveState = skybox->isEnabled();
            glm::mat4 mvp = camera->getProjectionMatrix()*camera->getViewMatrix()*skybox->getTransform()->getTransformations();

            mShaderProgram->setUniform(mU_mvp, mvp);
            mShaderProgram->setUniform(mU_colorPicking, RENDER_PICKING_SKYBOX_COLOR);

            skybox->getShape()->draw();
            glClear(GL_DEPTH_BUFFER_BIT);
            skybox->setEnabled(false);
        }

        unsigned int nbprim=scene->getObjectsCount();

        for(unsigned int i(0); i < nbprim; i++){
            Object_sptr object = scene->getObject(i);
            if(!object->isEnabled()){
                continue;
            }

            glm::mat4 mvp;
            mvp = camera->getProjectionMatrix()*camera->getViewMatrix()*object->getTransform()->getTransformations();

            uint8_t rouge = (i & 0x0000FF) ;
            uint8_t vert = (i & 0x00FF00) >> 8;
            uint8_t bleu = (i & 0xFF0000) >> 16;

            glm::vec3 pickingColor(static_cast<float>(rouge)/255.0f, static_cast<float>(vert)/255.0f, static_cast<float>(bleu)/255.0f);

            mShaderProgram->setUniform(mU_mvp, mvp);
            mShaderProgram->setUniform(mU_colorPicking, pickingColor);
            object->getShape()->draw();
        }

        // Si la skybox était activée on la réactive
        if(skyboxSaveState){
            scene->getSkybox()->setEnabled(true);
        }

        mShaderProgram->unbind();
    }

    PickingID_t RenderPicking::getClickedObject(int x, int y){
        unsigned char pixels[3];
        glReadPixels(x, y, 1, 1, GL_RGB , GL_UNSIGNED_BYTE, &pixels);
        PickingID_t id = (pixels[0] & 0xFF) | ((pixels[1] & 0xFF) << 8) | ((pixels[2] & 0xFF) << 16);
        return id;
    }

    void RenderPicking::initUniforms(void){
        mShaderProgram->bind();
            mU_colorPicking = getUniform("colorPicking", mShaderProgram.get());
            mU_mvp = getUniform("mvp", mShaderProgram.get());
        mShaderProgram->unbind();
    }
}
