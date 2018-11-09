#include "ImageLoader.h"
#include "geException.h"
#include <iostream>
#include <string>
#include <fstream>

ImageLoader::ImageLoader(void) {}

ImageLoader::ImageLoader(const std::string &file){
    loadFile(file);
}

ImageLoader::~ImageLoader(void) {}

void ImageLoader::loadFile(const std::string &file){
    const std::string ext = getFileExt(file);

    if(ext == "BMP"){
        loadBMP(file);
    }else{
        throw geException("ImageLoader : " + ext + " image format is not supported.");
    }
}

void ImageLoader::clear(void){
    mPixels.clear();
    mWidth = 0;
    mHeight = 0;
    mGL_PixelFormat = GL_INVALID_ENUM;
}

unsigned int ImageLoader::getWidth(void) const { return mWidth; }
unsigned int ImageLoader::getHeight(void) const { return mHeight; }
GLuint ImageLoader::getPixelFormat(void) const { return mGL_PixelFormat; }
std::vector<uint8_t>* ImageLoader::getPixels(void) { return &mPixels; }

void ImageLoader::loadBMP(const std::string &file){
    std::ifstream stream(file, std::ifstream::binary);

    if(!stream)
        throw geException("ImageLoader : Failed to open \"" + file+ "\".");
   /* FILE* stream;
    if(!(stream = fopen(filename,"rb")))
       return 0;*/

    //L'entete
    char ee;
    uint8_t header[2] = {'B', 'M'}; //0x00 - 0x01
    for(unsigned char i(0); i < 2; i++){
        ee = stream.get();
        if(ee != header[i])
            throw geException("ImageLoader : in \"" + file + "\" : invalid BMP usage.");
    }

    //Taille d'une ligne
    read4Bytes(stream); // 0x02 - 0x05
    //Reserved
    readWord(stream); // 0x06 - 0x07
    readWord(stream); // 0x08 - 0x09
    //bfOffBits
    read4Bytes(stream); // 0x0A - 0x0D

    uint32_t n40 = read4Bytes(stream); //0x0E - 0x11
    if(n40!=40)
        throw geException("ImageLoader : in \"" + file + "\" : corrupted or invalid file.");

    //ligne colonne
    uint32_t ncolonne = read4Bytes(stream); //0x12 - 0x15
    uint32_t nligne = read4Bytes(stream);//0x16 - 0x19


    //les autres info
    readWord(stream); // 0x1A - 0x1B
    readWord(stream); // 0x1C - 0x1D

    read4Bytes(stream); // 0x1E - 0x21
    read4Bytes(stream);// 0x22 - 0x25
    read4Bytes(stream);// 0x26 - 0x29
    read4Bytes(stream);// 0x2A - 0x2D
    read4Bytes(stream);// 0x2E - 0x31
    read4Bytes(stream);// 0x32 - 0x35


    uint8_t val_unsigned_char;
    //Ecrit les données
    int bf = (4-((ncolonne*3)%4)) & 0x03; //panding 32 bits
    //int bf = (ncolonne*3)%4; //panding 32 bits

    mPixels.resize(3*nligne*ncolonne);
    //for(l=nligne-1; l>=0; l--)
    for(unsigned int l=0; l<nligne; l++)
    {
        for(unsigned int c=0; c<ncolonne; c++)
        {
            mPixels[(l*ncolonne+c)*3+2] = stream.get();
            mPixels[(l*ncolonne+c)*3+1] = stream.get();
            mPixels[(l*ncolonne+c)*3] = stream.get();
        }
        for(int p=0; p<bf; p++){
            val_unsigned_char = static_cast<uint8_t>(stream.get());
        }
    }

    stream.close();
    mWidth = ncolonne;
    mHeight = nligne;
    mGL_PixelFormat = GL_RGB;
}

const std::string ImageLoader::getFileExt(const std::string &file) const {
    std::size_t pos = file.find_last_of('.');
    if(pos == std::string::npos)
        throw geException("ImageLoader : Failed to get file extension of \""+file+"\"");
    std::string ext = file.substr(pos+1);
    for(unsigned int i(0); i < ext.size(); i++){
        if(ext[i] >= 'a' && ext[i] <= 'z')
            ext[i] = ext[i] + 'A' - 'a';
    }
    return static_cast<const std::string>(ext);
}

uint16_t ImageLoader::readWord(std::ifstream &stream){
    uint8_t value[2];
    value[0] = stream.get();
    value[1] = stream.get();
    uint16_t returnValue = (value[0] & 0xFF) | ((value[1] & 0xFF) << 8);
    return returnValue;
}

uint32_t ImageLoader::read4Bytes(std::ifstream &stream){
    uint8_t value[4];
    value[0] = stream.get();
    value[1] = stream.get();
    value[2] = stream.get();
    value[3] = stream.get();
    uint32_t returnValue = (value[0] & 0xFF) | ((value[1] & 0xFF) << 8) | ((value[2] & 0xFF) << 16) | ((value[3] & 0xFF) << 24);
    return returnValue;
}
