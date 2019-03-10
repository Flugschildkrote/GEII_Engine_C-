#include "OGL_Buffer.h"

namespace GEII
{
    OGL_Buffer::OGL_Buffer(GLenum target, GLenum usage, unsigned int sizeBytes, const void* data) :
        OGL_Object(), mGL_Target(target), mGL_Usage(usage), mSizeBytes(sizeBytes)
    {
        glGenBuffers(1 ,&mGL_ID);

        glBindBuffer(mGL_Target, mGL_ID);
        glBufferData(mGL_Target, mSizeBytes, data, mGL_Usage);
        glBindBuffer(mGL_Target, 0);

        if(!glIsBuffer(mGL_ID)){
            throw std::runtime_error("Failed to create buffer (id = " + std::to_string(mGL_ID)+").");
        }
    }

    OGL_Buffer::~OGL_Buffer(void){
        glDeleteBuffers(1, &mGL_ID);
    }

    void OGL_Buffer::sendSubData(const void* data, GLsizeiptr dataSizeBytes, GLintptr offsetBytes){
        glBufferSubData(mGL_Target, offsetBytes, dataSizeBytes, data);
    }

    void OGL_Buffer::bind(void) const{
        glBindBuffer(mGL_Target, mGL_ID);
    }

    void OGL_Buffer::unbind(void) const{
        glBindBuffer(mGL_Target, 0);
    }

    int OGL_Buffer::getSizeBytes(void) const { return mSizeBytes; }
    GLenum OGL_Buffer::getTarget(void) const { return mGL_Target; }
    GLenum OGL_Buffer::getUsage(void) const { return mGL_Usage; }
}
