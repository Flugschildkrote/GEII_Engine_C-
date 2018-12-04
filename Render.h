#ifndef RENDER_H
#define RENDER_H

#include "geHeader.h"
#include "Transform.h"
#define GE_UNKNOWN_UNIFORM -1

#define LIGHT_SUN_PROJECTION glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 200.0f)
#define LIGHT_SPOT_PROJECTION(angle) glm::perspective(glm::radians(angle), 1.0f, 0.1f, 200.0f)

#define MAX_LIGHTS 2
class RenderShadowMapping;

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

    virtual void draw(Scene *scene, Camera *camera, const std::vector<Light_sptr> &lights) const;
protected:
    virtual void initUniforms(void);
    //std::unique_ptr<RenderShadowMapping> mShadowMappingRender;
/**************[UNIFORMS]***************************************/
    /**[MATERIAL]**/
    GLint mU_MatKd, mU_MatKs, mU_MatNs, mU_MatAlpha, mU_MatText, mU_MatUseTexture, mU_MatLightSensitive;
    /**[LIGHT]**/
    GLint mU_LightMatrix[MAX_LIGHTS], mU_ShadowMap[MAX_LIGHTS];
    GLint mU_LightDir[MAX_LIGHTS], mU_LightPower[MAX_LIGHTS], mU_LightAmbiant[MAX_LIGHTS];
    GLint mU_LightType[MAX_LIGHTS];
    GLint mU_LightPos[MAX_LIGHTS];
    GLint mU_LightAngle[MAX_LIGHTS];
    GLint mU_LightCount;
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
