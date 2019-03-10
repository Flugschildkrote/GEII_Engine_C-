#include "Texture2D.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#if OGL_TEXTURE_2D_DEBUG_ENABLE == 1 && GEII_DEBUG == 1
    #define OGL_TEXTURE_2D_DEBUG(x) x
    #include "Log.h"
#else
    #define OGL_TEXTURE_2D_DEBUG(x)
#endif // OGL_SHADER_DEBUG_ENABLE


namespace GEII
{
    OGL_TEXTURE_2D_DEBUG(static Logger OGL_Texture2DLogger("Texture2D");)

    Texture2D::Texture2D(const std::string &file, bool genMipMaps) : OGL_Texture(GL_TEXTURE_2D, genMipMaps), mName(file)
    {
        int channels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *pixels = stbi_load(file.c_str(), &mWidth, &mHeight, &channels, 0);
        OGL_TEXTURE_2D_DEBUG
        (
            if(!pixels){
                throw std::runtime_error("Failed to read \"" + file + "\".");
            }
        )
        mGL_Format = getFormatFromComponents(channels);
        mGL_InternalFormat = mGL_Format;
        // On envoie les données dans la texture;
        glBindTexture(GL_TEXTURE_2D, mGL_ID);
            glPixelStorei(GL_UNPACK_ALIGNMENT,1);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexImage2D(GL_TEXTURE_2D, 0, mGL_InternalFormat, mWidth, mHeight, 0, mGL_Format, GL_UNSIGNED_BYTE , pixels);
            if(mHasMipMaps)
                glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(pixels);

        OGL_TEXTURE_2D_DEBUG(OGL_Texture2DLogger.Success("Texture2D created from \"", file, "\" (id=", mGL_ID, ")."));
        OGL_TEXTURE_2D_DEBUG(PRINT_TEXTURE_INFO(file));
    }

    Texture2D::Texture2D(int width, int height, GLenum format, GLenum internalFormat, GLenum dataType, const void* data, bool genMipMaps) :
        OGL_Texture(GL_TEXTURE_2D, genMipMaps), mName(""), mWidth(width), mHeight(height)
    {
        mGL_Format = format;
        mGL_InternalFormat = internalFormat;
        glBindTexture(GL_TEXTURE_2D, mGL_ID);
            glPixelStorei(GL_UNPACK_ALIGNMENT,1);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexImage2D(GL_TEXTURE_2D, 0, mGL_InternalFormat, mWidth, mHeight, 0, mGL_Format, dataType , data);
            if(mHasMipMaps)
                glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        OGL_TEXTURE_2D_DEBUG(OGL_Texture2DLogger.Success("Texture2D created (id=", mGL_ID, ")."));
        OGL_TEXTURE_2D_DEBUG(PRINT_TEXTURE_INFO("From buffer"));
    }

    Texture2D::~Texture2D(void){
        OGL_TEXTURE_2D_DEBUG(OGL_Texture2DLogger.Info("Texture destroyed (id=", mGL_ID, ")."));
    }

    /*virtual void bind(void) const override;
    virtual void unbind(void) const override;*/

    int Texture2D::getHeight(void) const { return mWidth; }
    int Texture2D::getWidth(void) const { return mHeight; }



    ShadowMap::ShadowMap(int width, int height) : Texture2D(width, height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr, false){
        glBindTexture(GL_TEXTURE_2D, mGL_ID);
            float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
        glBindTexture(GL_TEXTURE_2D, 0);

    }
    ShadowMap::~ShadowMap(void) { }
}
