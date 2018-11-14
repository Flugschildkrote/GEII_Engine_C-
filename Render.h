#ifndef RENDER_H
#define RENDER_H

#include "geHeader.h"
#define GE_UNKNOWN_UNIFORM -1

#define LIGHT_PROJECTION glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 200.0f)
class RenderShadowMapping;

struct Light{
    glm::vec3 ambiantColor;
    glm::vec3 power;
    glm::vec3 position;
    glm::vec3 dir;
};

class Render{
public:
    Render(void);
    virtual ~Render(void);

protected:
    GLint getUniform(const std::string &uniformName, OGL_ShaderProgram* shader) const;
    virtual void initUniforms(void) = 0;
    OGL_ShaderProgram_uptr mShaderProgram;
};

class RenderPhong : public Render{
public:
    RenderPhong(void);
    virtual ~RenderPhong(void);

    virtual void draw(Scene *scene, Camera *camera, Light* light, Texture_sptr shadowMap) const;
protected:
    virtual void initUniforms(void);
    //std::unique_ptr<RenderShadowMapping> mShadowMappingRender;
/**************[UNIFORMS]***************************************/
    /**[MATERIAL]**/
    GLint mU_MatKd, mU_MatKs, mU_MatNs, mU_MatAlpha, mU_MatText, mU_MatUseTexture, mU_MatLightSensitive;
    /**[LIGHT]**/
    GLint mU_LightMatrix, mU_ShadowMap;
    GLint mU_LightDir, mU_LightPower, mU_LightAmbiant;
    /**[MODEL]**/
    GLint mU_ModelMatrix;
    /**[CAMERA]**/
    GLint mU_MVP;
    GLint mU_WorldEyePos;
    OGL_ShaderProgram_uptr mSkyboxShader;
/***************************************************************/
    GLint mU_Sky_MVP, mU_Sky_Texture;
};

class RenderShadowMapping : public Render{
public :
    RenderShadowMapping(void);
    ~RenderShadowMapping(void);

    virtual void draw(Scene* scene, Light *light);
protected :
    virtual void initUniforms(void);
    /**************[UNIFORMS]***************************************/
    /**[CAMERA]**/
    GLint mU_MVP;
/***************************************************************/
};
#endif // RENDER_H
