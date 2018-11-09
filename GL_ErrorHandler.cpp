#include "GL_ErrorHandler.h"
#include <iostream>

GLenum mSource = GL_INVALID_ENUM, mType = GL_INVALID_ENUM, mSeverity = GL_INVALID_ENUM;
GLuint mID = 0;
std::string mMessage;
bool mErrorFlag;

// During init, enable debug output


bool geGetErrorFlag(void) { return mErrorFlag; }
void geClearErrorFlag(void) { mErrorFlag = false; }
const std::string geGetErrorString(void) { return mMessage; }
void GLAPIENTRY geErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam){
  mErrorFlag = true;
  mSource = source;
  mType = type;
  mID = id;
  mSeverity = severity;
  mMessage = message;
  std::cerr << "GL CALLBACK:  type = " << type << ", severity = " << severity << ", message = " << message << std::endl;
}

