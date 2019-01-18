#include "OGL_Shader.h"
#include <iostream>
#include <fstream>
#include <array>
#include "glm/gtc/type_ptr.hpp"
#include "geException.h"

OGL_ShaderProgram::OGL_ShaderProgram(const OGL_ShaderProgramCreateInfo &createInfo) : OGL_Object() {
    GLuint vertShader, fragShader;
    CHECK_POINTER(createInfo.vertInfo, std::cerr << "Warning : OGL_ShaderProgram(), vertInfo must be a valid pointer.");
    CHECK_POINTER(createInfo.fragInfo, std::cerr << "Warning : OGL_ShaderProgram(), fragInfo must be a valid pointer.");

    try{
        compileShader(vertShader, GL_VERTEX_SHADER, *createInfo.vertInfo);
        compileShader(fragShader, GL_FRAGMENT_SHADER, *createInfo.fragInfo);
    }catch(const geException &e){
        glDeleteShader(vertShader);
        glDeleteShader(fragShader);
        throw geException("Failed to create OGL_ShaderProgram : " + e.toString());
    }

    mGL_ID = glCreateProgram();
    // On attache les shader au programme
    glAttachShader(mGL_ID, vertShader);
    glAttachShader(mGL_ID, fragShader);

    // On définit les locations des attributs
    glBindAttribLocation(mGL_ID, 0, "in_Vertex");
    glBindAttribLocation(mGL_ID, 1, "in_Normal");
    glBindAttribLocation(mGL_ID, 2, "in_UV");
    glBindAttribLocation(mGL_ID, 3, "in_Tangent");
    glBindAttribLocation(mGL_ID, 4, "in_BiTangent");

    glLinkProgram(mGL_ID);

    // Plus besoin des shaders
    glDetachShader(mGL_ID, vertShader);
    glDeleteShader(vertShader);
    glDetachShader(mGL_ID, fragShader);
    glDeleteShader(fragShader);

    // Detection d'erreurs
    GLint linkStatus(0);
    glGetProgramiv(mGL_ID, GL_LINK_STATUS, &linkStatus);

    if(linkStatus != GL_TRUE){
        GLint errorSize(0);
        glGetProgramiv(mGL_ID, GL_INFO_LOG_LENGTH, &errorSize);
        char *error_str = new char[errorSize+1];
        glGetProgramInfoLog(mGL_ID, errorSize, &errorSize, error_str);
        error_str[errorSize] = '\0';
        delete[] error_str;
        glDeleteProgram(mGL_ID);
        throw geException("Failed to create OGL_ShaderProgram : Failed to link shader.");
    }
    LOG_OGL_LIFETIME(std::cerr, "OGL_ShaderProgram created.");
}

OGL_ShaderProgram::~OGL_ShaderProgram() {
    destroy();
    LOG_OGL_LIFETIME(std::cerr, "OGL_ShaderProgram destroyed.");
}

void OGL_ShaderProgram::bind(bool bindState) const { glUseProgram(bindState ? mGL_ID : 0); }
void OGL_ShaderProgram::destroy(void) { glDeleteProgram(mGL_ID); }

void OGL_ShaderProgram::compileShader(GLuint &shader, GLenum type, const ShaderCreateInfo &shaderInfo){
    shader = glCreateShader(type);
    if(!shader){
        throw geException("Failed to create shader : Unknown shader type.");
    }

    std::string fileCode = "";
    if(shaderInfo.fromFile_nFromMemory){
        std::ifstream inStream(shaderInfo.sourceFile.c_str());
        if(!inStream){
            glDeleteShader(shader);
            throw geException("Failed to open \"" + shaderInfo.sourceFile + "\".");
        }

        std::string line;
        while(getline(inStream, line)){
            fileCode += (line + '\n');
        }
        inStream.close();
    }

    const GLchar* glSourceCode = shaderInfo.fromFile_nFromMemory ? fileCode.c_str(): shaderInfo.memoryCode.c_str();
    glShaderSource(shader, 1, &glSourceCode , 0);
    glCompileShader(shader);

    GLint error(0);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &error);

    if(error != GL_TRUE){
        GLint errorSize(0);
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &errorSize);

        std::string error_str;
        glGetShaderInfoLog(shader, errorSize, &errorSize, &error_str[0]);
        error_str[errorSize] = '\0';
        glDeleteShader(shader);
        throw geException("Failed to compile shader : " + std::string(error_str));
    }
}

GLint OGL_ShaderProgram::getUniformLocation(const std::string &name) const{ return glGetUniformLocation(mGL_ID, name.c_str()); }

void OGL_ShaderProgram::setUniform(GLuint location, const glm::mat4 &mat4) const{
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat4));
}
void OGL_ShaderProgram::setUniform(GLuint location, const glm::vec4 &vec4) const{
    glUniform4f(location, vec4.x, vec4.y, vec4.z, vec4.z);
}
void OGL_ShaderProgram::setUniform(GLuint location, const glm::vec3 &vec3) const{
    glUniform3f(location, vec3.x, vec3.y, vec3.z);
}
void OGL_ShaderProgram::setUniform(GLuint location, float value)const {
    glUniform1f(location,value);
}
void OGL_ShaderProgram::setUniform(GLuint location, int value)const {
    glUniform1i(location,value);
}

void OGL_ShaderProgram::setUniformTexture(GLuint location, GLint textureUnit) const{
    glUniform1i(location, textureUnit);
}
