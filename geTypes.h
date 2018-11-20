#ifndef GETYPES_H_INCLUDED
#define GETYPES_H_INCLUDED


///--------------------------------------------------
///---------------[SHAPE]----------------------------
///--------------------------------------------------
class Shape;
using Shape_uptr = std::unique_ptr<Shape>;
using Shape_sptr = std::shared_ptr<Shape>;
using Shape_wptr = std::weak_ptr<Shape>;
enum class ShapeSource : uint8_t {
    BOX = 0,
    SPHERE = 1,
    CYLINDER = 2,
    CONE = 3,
    TORUS = 4,
    FROM_BUFFER = 5,
    SQUARE = 6,
    SKYBOX = 7
};
struct ShapeCreateInfo{
    ShapeSource sourceType = ShapeSource::FROM_BUFFER;
    float radius, internalRadius = 0.0f;
    unsigned int stack, slice = 0;
    float half_x, half_y, half_z = 0.0f;
    float height = 0;
    const std::vector<float>* vertice = nullptr;
    const std::vector<float>* UVs = nullptr;
    const std::vector<float>* normals = nullptr;
    const std::vector<unsigned int>* indices = nullptr;
};
///--------------------------------------------------


///---------------[SCENE]----------------------------
class Scene;
using Scene_ID = unsigned int;
using Scene_ID_p = Scene_ID*;
///--------------------------------------------------


///--------------------------------------------------
///----------------[TEXTURE]-------------------------
///--------------------------------------------------
class OGL_Texture;
using Texture_uptr = std::unique_ptr<OGL_Texture>;
using Texture_sptr = std::shared_ptr<OGL_Texture>;
using Texture_wptr = std::weak_ptr<OGL_Texture>;
enum TextureFlag : uint8_t{
    TEXTURE_FLAG_NONE = 0,
    TEXTURE_FORCE_WIDTH           = 0b0000'0001,
    TEXTURE_FORCE_HEIGHT          = 0b0000'0010,
    TEXTURE_FORCE_SIZE            = 0b0000'0011,
    TEXTURE_FORCE_FORMAT          = 0b0000'0100,
    TEXTURE_FORCE_INTERNAL_FORMAT = 0b0000'1000,
   // TEXTURE_USE_BORDER_COLOR      = 0b0001'0000
};
struct OGL_TextureCreateInfo{
    bool fromFile_nFromBuffer = false;
    TextureFlag flags = TEXTURE_FLAG_NONE;
    std::string sourceFile;
    unsigned int width = 0;
    unsigned int height = 0;
    GLenum internalFormat = GL_INVALID_ENUM;
    GLenum format = GL_INVALID_ENUM;
    GLenum wrapS = GL_REPEAT;
    GLenum wrapT = GL_REPEAT;
    GLenum magFilter = GL_NEAREST;
    GLenum minFilter = GL_NEAREST;
    GLenum pixelType = GL_UNSIGNED_BYTE;
    glm::vec4 borderColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    unsigned char *data = nullptr;
    bool genMipMaps = false;
};
///--------------------------------------------------

///--------------------------------------------------
///----------------[MATERIAL]------------------------
///--------------------------------------------------
class Material;
using Material_uptr = std::unique_ptr<Material>;
using Material_sptr = std::shared_ptr<Material>;
using Material_wptr = std::weak_ptr<Material>;
struct MaterialCreateInfo{
    glm::vec3 ambiantColor_Kd = glm::vec3(1.0f , 1.0f, 1.0f);
    glm::vec3 specularColor_Ks = glm::vec3(0,0,0);
    float specularExponent_Ns = 1.0;
    float transparency_Alpha = 1.0f;
    bool lightSensitive = true;
    Texture_sptr texture = nullptr;
};
///--------------------------------------------------

///--------------------------------------------------
///---------------[OGL_BUFFER]-----------------------
///--------------------------------------------------
class OGL_Buffer;
using OGL_Buffer_uptr = std::unique_ptr<OGL_Buffer>;
using OGL_Buffer_sptr = std::shared_ptr<OGL_Buffer>;
using OGL_Buffer_wptr = std::weak_ptr<OGL_Buffer>;
///--------------------------------------------------

///--------------------------------------------------
///---------------[OBJECT]-----------------------
///--------------------------------------------------
using PickingID_t = uint32_t;

class Object;
using Object_uptr = std::unique_ptr<Object>;
using Object_sptr = std::shared_ptr<Object>;
using Object_wptr = std::weak_ptr<Object>;
struct ObjectCreateInfo{
    Material_wptr material;
    Shape_wptr shape;
    glm::mat4 transformations = glm::mat4(1.0f);
};
///--------------------------------------------------

///--------------------------------------------------
///------------[OGL_ShaderProgram]-------------------
///--------------------------------------------------
class OGL_ShaderProgram;
using OGL_ShaderProgram_uptr = std::unique_ptr<OGL_ShaderProgram>;
using OGL_ShaderProgram_sptr = std::shared_ptr<OGL_ShaderProgram>;
using OGL_ShaderProgram_wptr = std::weak_ptr<OGL_ShaderProgram>;
struct ShaderCreateInfo{
    std::string sourceFile = "";
    std::string memoryCode = "";
    bool fromFile_nFromMemory = false;
};
struct OGL_ShaderProgramCreateInfo{
    ShaderCreateInfo *vertInfo = nullptr;
    ShaderCreateInfo *fragInfo = nullptr;
};
///--------------------------------------------------

class Camera;

class Render;
class RenderPhong;

class Transform;
using Transform_sptr = std::shared_ptr<Transform>;
using Transform_wptr = std::weak_ptr<Transform>;
using Transform_uptr = std::unique_ptr<Transform>;

enum LightType{
    LIGHT_SUN = 0,
    LIGHT_SPOT = 1
};

struct Light;
using Light_sptr = std::shared_ptr<Light>;
#endif // GETYPES_H_INCLUDED
