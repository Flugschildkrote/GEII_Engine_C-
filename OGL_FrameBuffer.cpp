#include "OGL_FrameBuffer.h"
#include "geException.h"

OGL_FrameBuffer::OGL_FrameBuffer(const FrameBuffer_CreateInfo &info): OGL_Object(), mWidth(info.width), mHeight(info.height){
    glGenFramebuffers(1, &mGL_ID);
    glBindFramebuffer(GL_FRAMEBUFFER, mGL_ID);

       // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,info.attachments[0].textureID, 0);
         //   glReadBuffer(GL_COLOR_ATTACHMENT0);
           // glDrawBuffer(GL_COLOR_ATTACHMENT0);

    for(unsigned int i=0; i < info.attachments.size(); i++){
        glFramebufferTexture2D(GL_FRAMEBUFFER, info.attachments[i].attachment, GL_TEXTURE_2D,info.attachments[i].textureID, 0);
        DEBUG_BLOCK(std::cerr << "Attaching texture " << info.attachments[i].textureID <<" attachment " << info.attachments[i].attachment << " to framebuffer " << mGL_ID << "\n");
    }

    if(info.flags & FRAMEBUFFER_DISABLE_READ){
        DEBUG_BLOCK(std::cerr << "Disabling reading from frame buffer "<< mGL_ID << std::endl;)
        glReadBuffer(GL_NONE);
    }
    if(info.flags & FRAMEBUFFER_DISABLE_WRITE){
        DEBUG_BLOCK(std::cerr << "Disabling writing to frame buffer "<< mGL_ID << std::endl;)
        glDrawBuffer(GL_NONE);
    }
 /*   GLuint rboColorId;
    glGenRenderbuffers(1, &rboColorId);
    glBindRenderbuffer(GL_RENDERBUFFER, rboColorId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB, 512, 512);

    GLuint rboDepthId;
    glGenRenderbuffers(1, &rboDepthId);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepthId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);


// attach colorbuffer image to FBO
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,       // 1. fbo target: GL_FRAMEBUFFER
                            GL_COLOR_ATTACHMENT0, // 2. color attachment point
                            GL_RENDERBUFFER,      // 3. rbo target: GL_RENDERBUFFER
                            rboColorId);          // 4. rbo ID

// attach depthbuffer image to FBO
glFramebufferRenderbuffer(GL_FRAMEBUFFER,       // 1. fbo target: GL_FRAMEBUFFER
                          GL_DEPTH_ATTACHMENT,  // 2. depth attachment point
                          GL_RENDERBUFFER,      // 3. rbo target: GL_RENDERBUFFER
                          rboDepthId);          // 4. rbo ID*/


      /*  GLuint renderBuffer;
        glGenRenderbuffers(1, &renderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 500, 600);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);*/



    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE ){
        //printf("#ERROR : Failed to create frame buffer object.");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        throw(geException("Failed to create Frame Buffer."));
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OGL_FrameBuffer::~OGL_FrameBuffer(void){
    //dtor
}

void OGL_FrameBuffer::bind(bool bindState) const{
    glBindFramebuffer(GL_FRAMEBUFFER, bindState ? mGL_ID: 0);
}

void OGL_FrameBuffer::destroy(void){
    glDeleteFramebuffers(1, &mGL_ID);
}

