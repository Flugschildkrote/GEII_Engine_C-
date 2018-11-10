#ifndef DEBUG_PREFERENCES_H_INCLUDED
#define DEBUG_PREFERENCES_H_INCLUDED

///##############[OBJ_LOADER]######################
//#define DEBUG_OGL_OBJECT_LIFETIME
#define ENBABLE_LOG_TEXTURE_INFO

#if defined(DEBUG) && defined(ENBABLE_LOG_TEXTURE_INFO)
#define DEBUG_TEXTURE_INFO(stream) \
    std::string name = (createInfo.fromFile_nFromBuffer ? createInfo.sourceFile : "From Buffers"); \
    stream << "Loaded texture : \"" << name << "\"\nSize : " << mWidth << "x" << mHeight << "\nComponents : " << \
    (int) getComponentsFromFormat(mGL_Format) << "\nMipmaps : " << (mHasMipMaps ? "True" : "False") << "\nGL_ID : " << mGL_ID << "\n\n";
#else
#define DEBUG_TEXTURE_INFO
#endif // defined

#endif // DEBUG_PREFERENCES_H_INCLUDED
