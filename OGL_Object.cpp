#include "OGL_Object.h"

namespace GEII
{
    OGL_Object::OGL_Object(void) : mGL_ID(0) { }
    OGL_Object::~OGL_Object(void) { }

    GLuint OGL_Object::getID(void) const { return mGL_ID; }
}
