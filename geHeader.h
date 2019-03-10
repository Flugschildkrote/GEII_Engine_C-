#ifndef GEHEADER_H_INCLUDED
#define GEHEADER_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <cstdio>


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "GL/glew.h"

#include "geTypes.h"
#include "Debug_Preferences.h"

inline const std::string geToString(const glm::vec3 &vec){ return "[" + std::to_string(vec.x) + "; " +  std::to_string(vec.y) + "; "+  std::to_string(vec.z) + "]"; }
inline const std::string geToString(const glm::vec2 &vec){ return "[" + std::to_string(vec.x) + "; " +  std::to_string(vec.y) + "]"; }

inline const std::string geGetFileRep(const std::string &file){
    std::size_t pos = file.find_last_of('\\');
    if(pos == std::string::npos){
        pos = file.find_last_of('/');
        if(pos == std::string::npos){
            return "";
        }else{
            return file.substr(0, pos+1);
        }
    }else{
        return file.substr(0, pos+1);
    }
}

#ifdef DEBUG
#define CHECK_INDEX(index, max_value, error_code) if(index >= max_value) { error_code; }
#define CHECK_POINTER(ptr, error_code) if(ptr == nullptr) { error_code; }
#define DEBUG_LOG(stream, log) stream << "[LOG] " << log << std::endl;
#define DEBUG_BLOCK(block) block
#else
#define CHECK_INDEX(index, max_value, error_code)
#define CHECK_POINTER(ptr, error_code)
#define DEBUG_LOG(stream, log)
#define DEBUG_BLOCK(block)
#endif // DEBUG

#include "Log.h"

#define ENGINE_DEBUG 1
#define ENGINE_LOG_FULL_COLOR 0

#if ENGINE_DEBUG == 1
    #define ENGINE_DEBUG_CODE(x) x
    #define ENGINE_ABBORT(condition, ...) if(condition) { ENGINE_LOG_ERROR(__VA_ARGS__);\
                                                          ENGINE_LOG_ERROR("std::abort()");\
                                                          std::abort(); }
#else
    #define ENGINE_DEBUG_CODE(x)
    #define ENGINE_ABBORT(condition, ...)
#endif // ENGINE_DEBUG

#endif // GEHEADER_H_INCLUDED
