#include "OGL_FrameBuffer.h"
#include "Texture2D.h"

#if OGL_FRAMEBUFFER_DEBUG_ENABLE == 1 && GEII_DEBUG == 1
    #define OGL_FRAMEBUFFER_DEBUG(x) x
    #include "Log.h"
#else
    #define OGL_FRAMEBUFFER_DEBUG(x)
#endif // OGL_FRAMEBUFFER_DEBUG_ENABLE

namespace GEII
{
    OGL_FRAMEBUFFER_DEBUG(Logger OGL_FrameBuffer_logger("FrameBuffer"));

    OGL_FrameBuffer::OGL_FrameBuffer(unsigned int width, unsigned int height, AttachmentTarget colorTarget, AttachmentTarget depthTarget, int flags)
        : OGL_Object(), mWidth(width), mHeight(height), mColorTexture(nullptr), mDepthTexture(nullptr), mAttachementTargets{colorTarget, depthTarget}
    {
        glGenFramebuffers(1, &mGL_ID);
        glBindFramebuffer(GL_FRAMEBUFFER, mGL_ID);

        switch(colorTarget){
            case AttachmentTarget::Texture :
                mColorTexture = std::make_shared<Texture2D>(width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorTexture->getID(), 0);
                OGL_FRAMEBUFFER_DEBUG(OGL_FrameBuffer_logger.Info("Attaching texture ",  mColorTexture->getID(),  " attachment : COLOR_ATTACHMENT0 to framebuffer ", mGL_ID));
                break;
            case AttachmentTarget::RenderBuffer :
                {
                    GLuint rboColorId;
                    glGenRenderbuffers(1, &rboColorId);
                    glBindRenderbuffer(GL_RENDERBUFFER, rboColorId);
                    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB, width, height);
                    mRenderBuffers.push_back(rboColorId);
                    OGL_FRAMEBUFFER_DEBUG(OGL_FrameBuffer_logger.Info("Attaching renderbuffer ",  rboColorId,  " attachment : COLOR_ATTACHMENT0 to framebuffer ", mGL_ID));
                }
                break;
            case AttachmentTarget::None :
            default :
                break;
        }

          switch(depthTarget){
            case AttachmentTarget::Texture :
                mDepthTexture = std::make_shared<ShadowMap>(width, height);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture->getID(), 0);
                OGL_FRAMEBUFFER_DEBUG(OGL_FrameBuffer_logger.Info("Attaching texture ",  mDepthTexture->getID(),  " attachment : GL_DEPTH_ATTACHMENT to framebuffer ", mGL_ID));
                break;
            case AttachmentTarget::RenderBuffer :
                {
                    GLuint rboDepthId;
                    glGenRenderbuffers(1, &rboDepthId);
                    glBindRenderbuffer(GL_RENDERBUFFER, rboDepthId);
                    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
                    mRenderBuffers.push_back(rboDepthId);
                    OGL_FRAMEBUFFER_DEBUG(OGL_FrameBuffer_logger.Info("Attaching renderbuffer ",  rboDepthId,  " attachment : GL_DEPTH_COMPONENT to framebuffer ", mGL_ID));
                }
                break;
            case AttachmentTarget::None :
            default :
                break;
        }

        if(flags & FRAMEBUFFER_DISABLE_READ){
            OGL_FRAMEBUFFER_DEBUG(OGL_FrameBuffer_logger.Info("Reading from frame buffer ", mGL_ID, " disabled."));
            glReadBuffer(GL_NONE);
        }

        if(flags & FRAMEBUFFER_DISABLE_WRITE){
            OGL_FRAMEBUFFER_DEBUG(OGL_FrameBuffer_logger.Info("Writing to frame buffer ", mGL_ID, " disabled."));
            glDrawBuffer(GL_NONE);
        }

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE ){
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            destroy();
            throw std::runtime_error("Failed to create Frame Buffer (id = "+ std::to_string(mGL_ID) + ").");
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        OGL_FRAMEBUFFER_DEBUG(OGL_FrameBuffer_logger.Success("Frame buffer created (id=", mGL_ID, "), size = {", mWidth, "; ", mHeight, "}"));
    }


    OGL_FrameBuffer::~OGL_FrameBuffer(void){
        destroy();
    }

    void OGL_FrameBuffer::bind(void) const{
        glBindFramebuffer(GL_FRAMEBUFFER, mGL_ID);
    }
    void OGL_FrameBuffer::unbind(void) const{
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OGL_FrameBuffer::destroy(void) const{
        glDeleteFramebuffers(1, &mGL_ID);
        OGL_FRAMEBUFFER_DEBUG(OGL_FrameBuffer_logger.Info("Frame buffer deleted (id=", mGL_ID, ")"));
        for(GLuint id : mRenderBuffers){
            glDeleteRenderbuffers(1, &id);
            OGL_FRAMEBUFFER_DEBUG(OGL_FrameBuffer_logger.Info("Render buffer deleted (id=", id, ")"));
        }
    }
}
