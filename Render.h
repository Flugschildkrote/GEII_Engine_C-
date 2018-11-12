#ifndef RENDER_H
#define RENDER_H

#include "geHeader.h"
#define GE_UNKNOWN_UNIFORM -1

struct Light{
    glm::vec3 ambiantColor;
    glm::vec3 power;
    glm::vec3 position;
};

class Render{
public:
    Render(void);
    virtual ~Render(void);

    virtual void draw(Scene *scene, Camera* camera) const = 0;
protected:
    GLint getUniform(const std::string &uniformName, OGL_ShaderProgram* shader) const;
    virtual void initUniforms(void) = 0;
    OGL_ShaderProgram_uptr mShaderProgram;
};

class RenderPhong : public Render{
public:
    RenderPhong(void);
    virtual ~RenderPhong(void);

    void update(Camera *camera);
    virtual void draw(Scene *scene, Camera *camera) const;
protected:
    virtual void initUniforms(void);
    Light mLight;
/**************[UNIFORMS]***************************************/
    /**[MATERIAL]**/
    GLint mU_MatKd, mU_MatKs, mU_MatNs, mU_MatAlpha, mU_MatText, mU_MatUseTexture;
    /**[LIGHT]**/
    GLint mU_LightPos, mU_LightPower, mU_LightAmbiant;
    /**[MODEL]**/
    GLint mU_ModelMatrix;
    /**[CAMERA]**/
    GLint mU_MVP;
    GLint mU_WorldEyePos;
    OGL_ShaderProgram_uptr mSkyboxShader;
/***************************************************************/
    GLint mU_Sky_MVP, mU_Sky_Texture;
};
#endif // RENDER_H
