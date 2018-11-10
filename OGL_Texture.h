#ifndef OGL_TEXTURE_H
#define OGL_TEXTURE_H

#include "OGL_Object.h"
#include "geHeader.h"
#include <string>
#include <vector>

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
