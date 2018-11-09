#include "OGL_Texture.h"
#include "ImageLoader.h"
#include "geException.h"
#include <iostream>

OGL_Texture::OGL_Texture(const OGL_TextureCreateInfo &createInfo) : OGL_Object(), mWidth(0), mHeight(0), mGL_Format(GL_INVALID_ENUM), mGL_InternalFormat(GL_INVALID_ENUM), mHasMipMaps(false){
    glGenTextures(1, &mGL_ID);
    std::vector<uint8_t> *pixels = nullptr;

    ImageLoader loader;

    if(!createInfo.fromFile_nFromBuffer){
        mHeight = createInfo.height;
        mWidth = createInfo.width;
        mGL_Format = createInfo.format;
        pixels = createInfo.pixels;
    }else{
        loader.loadFile(createInfo.sourceFile);
        mWidth = (createInfo.flags & TEXTURE_FORCE_WIDTH) ? createInfo.width : loader.getWidth();
        mHeight = (createInfo.flags & TEXTURE_FORCE_HEIGHT) ? createInfo.height : loader.getHeight();
        mGL_Format = (createInfo.flags & TEXTURE_FORCE_FORMAT) ? createInfo.format : loader.getPixelFormat();

        unsigned int lWidth = loader.getWidth();
        unsigned int lHeight = loader.getHeight();
        int lFormatSize = getFormatSize(loader.getPixelFormat());
        if(mWidth*mHeight*getFormatSize(mGL_Format) > lWidth*lHeight*lFormatSize){
            char formatSize = getFormatSize(mGL_Format);
            pixels = new std::vector<uint8_t>(mHeight*mWidth*formatSize);
           /* unsigned int px = mWidth*formatSize-lWidth*lFormatSize;
            unsigned int py =*/
            std::vector<uint8_t>* loaderPixels = loader.getPixels();
            unsigned int maxValue = loader.getPixels()->size();
            for(unsigned int i(0); i < maxValue; i++){
                (*pixels)[i] = (*loaderPixels)[i];
            }
            for(unsigned int i(maxValue); i < pixels->size(); i++){
                (*pixels)[i] = 255;
            }
        }else{
            pixels = loader.getPixels();
        }
    }
    mHasMipMaps = createInfo.genMipMaps;

    // Définition du format interne
    if(createInfo.flags & TEXTURE_FORCE_INTERNAL_FORMAT){
        mGL_InternalFormat = createInfo.internalFormat;
    }else if(mGL_Format == GL_RED || mGL_Format == GL_RG || mGL_Format == GL_DEPTH_COMPONENT || mGL_Format == GL_DEPTH_STENCIL){
        mGL_InternalFormat = mGL_Format;
    }else if(mGL_Format == GL_RGB || mGL_Format == GL_BGR){
        mGL_InternalFormat = GL_RGB;
    }else if(mGL_Format == GL_RGBA || mGL_Format == GL_BGRA){
        mGL_InternalFormat = GL_RGBA;
    }

    // On envoie les données dans la texture;
    glBindTexture(GL_TEXTURE_2D, mGL_ID);
       // glPixelStorei(GL_PACK_ALIGNMENT,1);
        glPixelStorei(GL_UNPACK_ALIGNMENT,1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, createInfo.minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, createInfo.magFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, createInfo.wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, createInfo.wrapT);
        glTexImage2D(GL_TEXTURE_2D, 0, mGL_InternalFormat, mWidth, mHeight, 0, mGL_Format, GL_UNSIGNED_BYTE, pixels->data());
        if(mHasMipMaps)
            glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    // On détruit les pixels si on les a creé
    if(pixels != createInfo.pixels && pixels != loader.getPixels()){
        delete pixels;
    }

    // Vérifie si texture OK
    if(glIsTexture(mGL_ID) == GL_FALSE){
        destroy();
        throw geException("Failed to load texture.");
    }
    LOG_OGL_LIFETIME(std::cerr, "OGL_Texture created.");
}

OGL_Texture::~OGL_Texture(void){
    destroy();
    LOG_OGL_LIFETIME(std::cerr, "OGL_Texture destroyed.");
}

void OGL_Texture::bind(bool bindState) const {
    glBindTexture(GL_TEXTURE_2D, bindState ? mGL_ID : 0);
}

void OGL_Texture::destroy(void) {
    glDeleteTextures(1, &mGL_ID);
}

char OGL_Texture::getFormatSize(GLenum format) const{
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

