#include "OGL_Buffer.h"
#include "geException.h"

OGL_Buffer::OGL_Buffer(const OGL_BufferCreateInfo& createInfo) : OGL_Object(), mGL_Target(createInfo.GL_target), mGL_Usage(createInfo.GL_usage), mSizeBytes(createInfo.sizeBytes){
    glGenBuffers(1 ,&mGL_ID);

    glBindBuffer(mGL_Target, mGL_ID);
    glBufferData(mGL_Target, mSizeBytes, createInfo.data, mGL_Usage);
    glBindBuffer(mGL_Target, 0);
    if(!glIsBuffer(mGL_ID)){
        throw geException("Failed to create OGL_Buffer.");
    }
}

OGL_Buffer::~OGL_Buffer(void){
    destroy();
}

OGL_Buffer_uptr OGL_Buffer::create_uptr(const OGL_BufferCreateInfo &createInfo){ return std::make_unique<OGL_Buffer>(createInfo); }
OGL_Buffer_sptr OGL_Buffer::create_sptr(const OGL_BufferCreateInfo &createInfo){ return std::make_shared<OGL_Buffer>(createInfo); }

void OGL_Buffer::sendSubData(const void* data, GLsizeiptr dataSizeBytes, GLintptr offsetBytes, bool autoBind){
    if(autoBind)
        glBindBuffer(mGL_Target, mGL_ID);
    glBufferSubData(mGL_Target, offsetBytes, dataSizeBytes, data);
     if(autoBind)
        glBindBuffer(mGL_Target, 0);
}

void OGL_Buffer::bind(bool bindState) const{
    glBindBuffer(mGL_Target, bindState ? mGL_ID : 0);
}

int OGL_Buffer::getSizeBytes(void) const { return mSizeBytes; }
GLenum OGL_Buffer::getTarget(void) const { return mGL_Target; }
GLenum OGL_Buffer::getUsage(void) const { return mGL_Usage; }

void OGL_Buffer::destroy(void){
    glDeleteBuffers(1, &mGL_ID);
}
