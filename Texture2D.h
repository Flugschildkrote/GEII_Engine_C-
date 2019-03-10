#ifndef TEXTURE2D_H_INCLUDED
#define TEXTURE2D_H_INCLUDED

/*!
* \file OGL_Texture.h
* \author Alexandre Brunet
* \date 31/01/2019
*/

#include "OGL_Texture.h"

#define PRINT_TEXTURE_INFO(name) \
    OGL_Texture2DLogger.Info("\n\tLoaded texture : \"", name, "\"\n\tSize : ", mWidth, "x", mHeight, "\n\tComponents : ", \
    (int) getComponentsFromFormat(mGL_Format), "\n\tMipmaps : ", (mHasMipMaps ? "True" : "False"), "\n\tGL_ID : ", mGL_ID, "\n\n");


namespace GEII
{
    class Texture2D : public OGL_Texture
    {
    public:
        Texture2D(const std::string &file, bool genMipMaps = false);
        Texture2D(int width, int height, GLenum format, GLenum internalFormat, GLenum dataType, const void* data, bool mipMaps = false);
        virtual ~Texture2D(void);

        void setName(const std::string& name) { mName = name; }
        const std::string& getName(void) const { return mName; }

        int getHeight(void) const;
        int getWidth(void) const;
    protected:
        std::string mName;
        int mWidth, mHeight;
    };

    class ShadowMap : public Texture2D
    {
    public:
        ShadowMap(int width, int height);
        virtual ~ShadowMap(void);
    };
}

#endif // TEXTURE2D_H_INCLUDED
