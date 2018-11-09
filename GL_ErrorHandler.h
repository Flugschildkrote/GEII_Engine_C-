#ifndef GL_ERRORHANDLER_H
#define GL_ERRORHANDLER_H

#include "GL/glew.h"
#include <string>

bool geGetErrorFlag(void);
void geClearErrorFlag(void);
const std::string geGetErrorString(void);
void GLAPIENTRY geErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);


#endif // GL_ERRORHANDLER_H
