#ifndef OGL_FRAMEBUFFER_H
#define OGL_FRAMEBUFFER_H

#include "geHeader.h"
#include "OGL_Object.h"

#define FRAMEBUFFER_DISABLE_READ  0x01
#define FRAMEBUFFER_DISABLE_WRITE 0x02


/*enum AttachmentDest{
    DEST_RENDERBUFFER_CREATE,
    DEST_TEXTURE
};*/

struct AttachmentData{
    GLenum attachment = GL_COLOR_ATTACHMENT0;
   // AttachmentDest dest = DEST_RENDERBUFFER_CREATE;
    GLuint textureID = 0;
};

struct FrameBuffer_CreateInfo{
    int flags = 0;
    std::vector<AttachmentData> attachments;
    unsigned int width, height;
};

class OGL_FrameBuffer : public OGL_Object
{
public:
    OGL_FrameBuffer(const FrameBuffer_CreateInfo &info);
    virtual ~OGL_FrameBuffer(void);

    virtual void bind(bool bindState) const;
    inline unsigned int getWidth(void) const { return mWidth; }
    inline unsigned int getHeight(void) const { return mHeight; }

protected:
    virtual void destroy(void);
    unsigned int mWidth, mHeight;
    //std::vector<GLuint> mRenderBuffers;
};

#endif // OGL_FRAMEBUFFER_H
