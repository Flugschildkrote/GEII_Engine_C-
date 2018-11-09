#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include "GL/glew.h"
#include <string>
#include <vector>
#include <fstream>

class ImageLoader
{
public:
    ImageLoader(void);
    ImageLoader(const std::string &file);
    virtual ~ImageLoader(void);

    void loadFile(const std::string &file);
    void clear(void);

    unsigned int getWidth(void) const;
    unsigned int getHeight(void) const;
    GLuint getPixelFormat(void) const;
    std::vector<uint8_t> *getPixels(void);

protected:
    void loadBMP(const std::string &file);
    const std::string getFileExt(const std::string &file) const;
    uint16_t readWord(std::ifstream &stream); // Lecture 16 bits (2 octets)
    uint32_t read4Bytes(std::ifstream &stream); // Lecture 4 octets

    unsigned int mWidth, mHeight;
    GLenum mGL_PixelFormat;
    std::vector<uint8_t> mPixels;
};

#endif // IMAGELOADER_H
