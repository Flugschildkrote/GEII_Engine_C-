#ifndef OGL_TEXTURE_H
#define OGL_TEXTURE_H

#include "OGL_Object.h"
#include "geHeader.h"
#include <string>
#include <vector>

#if defined(DEBUG) && defined(ENBABLE_LOG_TEXTURE_INFO)
#define DEBUG_TEXTURE_INFO(stream) \
    std::string name = (createInfo.fromFile_nFromBuffer ? createInfo.sourceFile : "From Buffers"); \
    stream << "Loaded texture : \"" << name << "\"\nSize : " << mWidth << "x" << mHeight << "\nComponents : " << \
    (int) getComponentsFromFormat(mGL_Format) << "\nMipmaps : " << (mHasMipMaps ? "True" : "False") << "\nGL_ID : " << mGL_ID << "\n\n";
#else
#define DEBUG_TEXTURE_INFO
#endif // defined

class OGL_Texture : public OGL_Object
{
public:
    OGL_Texture(const OGL_TextureCreateInfo &createInfo);
    virtual ~OGL_Texture(void);

    void bind(bool bindState) const;
protected:
    void destroy(void);

    char getComponentsFromFormat(GLenum format) const;
    GLenum getFormatFromComponents(unsigned int components) const;

    unsigned int mWidth, mHeight;
    GLenum mGL_Format, mGL_InternalFormat;
    bool mHasMipMaps;
};

#endif // OGL_TEXTURE_H
