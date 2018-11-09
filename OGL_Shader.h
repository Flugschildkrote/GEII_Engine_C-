#ifndef OGLSHADERS_H
#define OGLSHADERS_H

#include "geHeader.h"
#include "OGL_Object.h"

class OGL_ShaderProgram : public OGL_Object {
public:
    OGL_ShaderProgram(const OGL_ShaderProgramCreateInfo &createInfo);
    virtual ~OGL_ShaderProgram(void);

    void bind(bool bindState) const;

    GLint getUniformLocation(const std::string &name) const;
    void setUniform(GLuint location, const glm::mat4 &mat4) const;
    void setUniform(GLuint location, const glm::vec4 &vec4) const;
    void setUniform(GLuint location, const glm::vec3 &vec3) const;
    void setUniform(GLuint location, float value) const;
    void setUniformTexture(GLuint location, GLint textureUnit) const;

    //------------------[TO DO]---------------------------------------------------------------------------------
    //void OGLShaderProgramSetTextureUnit(GLint id, GLint unit);

private:
    virtual void destroy(void);
    void compileShader(GLuint &shader, GLenum type, const ShaderCreateInfo &shaderInfo);
};

#endif // OGLSHADERS_H
