#include "OGL_Texture.h"
#include "geException.h"
#include <iostream>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

OGL_Texture::OGL_Texture(const OGL_TextureCreateInfo &createInfo) : OGL_Object(), mWidth(0), mHeight(0), mGL_Format(GL_INVALID_ENUM), mGL_InternalFormat(GL_INVALID_ENUM), mHasMipMaps(false){
    glGenTextures(1, &mGL_ID);
    unsigned char *pixels = nullptr;
    bool fromPicture = false;

    if(!createInfo.fromFile_nFromBuffer){
        mHeight = createInfo.height;
        mWidth = createInfo.width;
        mGL_Format = createInfo.format;
        pixels = createInfo.data;
    }else{
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *c_pixels = stbi_load(createInfo.sourceFile.c_str(), &width, &height, &channels, 0);

        mWidth = (createInfo.flags & TEXTURE_FORCE_WIDTH) ? createInfo.width : width;
        mHeight = (createInfo.flags & TEXTURE_FORCE_HEIGHT) ? createInfo.height : height;
        mGL_Format = (createInfo.flags & TEXTURE_FORCE_FORMAT) ? createInfo.format : getFormatFromComponents(channels);

        if(mWidth*mHeight*getComponentsFromFormat(mGL_Format) > width*height*channels){
            char formatSize = getComponentsFromFormat(mGL_Format);
            DEBUG_BLOCK(if(formatSize == -1){ std::cerr << "Image : invalid format." << std::endl; });
            int newPixelsSize = mHeight*mWidth*formatSize;
            unsigned char *newPixels = new unsigned char[newPixelsSize];
            unsigned int maxValue = width*height*channels;
            for(unsigned int i(0); i < maxValue; i++){
                newPixels[i] = c_pixels[i];
            }
            for(unsigned int i(maxValue); i < newPixelsSize; i++){
                newPixels[i] = 255;
            }
            pixels = newPixels;
            stbi_image_free(c_pixels);
        }else{
            fromPicture = true;
            pixels = c_pixels;
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
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(createInfo.borderColor));
        glTexImage2D(GL_TEXTURE_2D, 0, mGL_InternalFormat, mWidth, mHeight, 0, mGL_Format, createInfo.pixelType, pixels);
        if(mHasMipMaps)
            glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    // On détruit les pixels si on les a creé
    if(pixels != createInfo.data){
        if(fromPicture){
            stbi_image_free(pixels);
        }else{
            delete[] pixels;
        }
    }

    // Vérifie si texture OK
    if(glIsTexture(mGL_ID) == GL_FALSE){
        destroy();
        throw geException("Failed to load texture.");
    }
    LOG_OGL_LIFETIME(std::cerr, "OGL_Texture created.");
    DEBUG_TEXTURE_INFO(std::cerr);
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

