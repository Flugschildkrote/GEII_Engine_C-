#ifndef OGL_OBJECT_H
#define OGL_OBJECT_H

#include "geHeader.h"

#if (defined(DEBUG)) && (defined(DEBUG_OGL_OBJECT_LIFETIME))
#define LOG_OGL_LIFETIME(stream, log) stream << log << std::endl;
#else
#define LOG_OGL_LIFETIME(stream, log)
#endif // DEBUG


class OGL_Object
{
public:
    OGL_Object(void);
    virtual ~OGL_Object(void);

    OGL_Object(const OGL_Object &other) = delete;
    OGL_Object& operator=(const OGL_Object &other) = delete;

    virtual void bind(bool bindState) const = 0;
    GLuint getID(void) const;
protected:
    virtual void destroy(void) = 0;

    GLuint mGL_ID;
};

#endif // OGL_OBJECT_H
