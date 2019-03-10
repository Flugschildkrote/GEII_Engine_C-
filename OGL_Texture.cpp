#include "OGL_Texture.h"
#include "Log.h"
#include <iostream>

namespace GEII
{
    OGL_Texture::OGL_Texture(GLenum target, bool genMipMaps) : mGL_Target(target), mHasMipMaps(genMipMaps){
        glGenTextures(1, &mGL_ID);
    }

    OGL_Texture::~OGL_Texture(void){
        glDeleteTextures(1, &mGL_ID);
    }


    void OGL_Texture::bind(void) const {
        glBindTexture(mGL_Target, mGL_ID);
    }

    void OGL_Texture::unbind(void) const {
        glBindTexture(mGL_Target, 0);
    }

    char OGL_Texture::getComponentsFromFormat(GLenum format) const{
        if(format == GL_RED || format == GL_DEPTH_COMPONENT)
            return 1;
        else if(format == GL_DEPTH_STENCIL || format == GL_RG)
            return 2;
        else if(format == GL_RGB || format == GL_BGR)
            return 3;
        else if(format == GL_RGBA || format == GL_BGRA)
            return 4;
        return -1;
    }

    GLenum OGL_Texture::getFormatFromComponents(unsigned int components) const{
        switch(components){
        case 1:
            return GL_RED;
            break;
        case 2:
            return GL_RG;
            break;
        case 3:
            return GL_RGB;
            break;
        case 4:
            return GL_RGBA;
            break;
        default:
            return GL_INVALID_ENUM;
            break;
        }
    }
}
