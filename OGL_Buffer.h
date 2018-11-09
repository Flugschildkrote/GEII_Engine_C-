#ifndef BUFFER_H
#define BUFFER_H

#include "OGL_Object.h"
#include "geHeader.h"

struct OGL_BufferCreateInfo{
    GLenum GL_target = GL_INVALID_ENUM;
    GLenum GL_usage = GL_INVALID_ENUM;
    unsigned int sizeBytes = 0;
    const void* data = NULL;
};


class OGL_Buffer : public OGL_Object{
public:
    OGL_Buffer(const OGL_BufferCreateInfo& createInfo);
    virtual ~OGL_Buffer(void);

    static OGL_Buffer_uptr create_uptr(const OGL_BufferCreateInfo &createInfo);
    static OGL_Buffer_sptr create_sptr(const OGL_BufferCreateInfo &createInfo);

    void sendSubData(const void* data, GLsizeiptr dataSizeBytes, GLintptr offsetBytes, bool autoBind = true);
    void bind(bool bindState) const;

    int getSizeBytes(void) const;
    GLenum getTarget(void) const;
    GLenum getUsage(void) const;
protected :
    void destroy(void);

    GLenum mGL_Target;
    GLenum mGL_Usage;
    int mSizeBytes;
};



#endif // BUFFER_H
