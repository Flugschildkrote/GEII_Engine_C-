#ifndef GETYPES_H_INCLUDED
#define GETYPES_H_INCLUDED

/*!
* \namespace GEII
* \brief Namespace regroupant tous les composants du moteur graphique.
*/

//--------------------------------------------------
//---------------[SHAPE]----------------------------
//--------------------------------------------------
namespace GEII
{
    class Shape;
    using Shape_uptr = std::unique_ptr<Shape>;
    using Shape_sptr = std::shared_ptr<Shape>;
    using Shape_wptr = std::weak_ptr<Shape>;
}
//--------------------------------------------------


//---------------[SCENE]----------------------------
class Scene;
using Scene_ID = unsigned int;
using Scene_ID_p = Scene_ID*;
//--------------------------------------------------


//--------------------------------------------------
//----------------[TEXTURE]-------------------------
///--------------------------------------------------
namespace GEII
{
    class OGL_Texture;
    class Texture2D;
    using Texture_uptr = std::unique_ptr<Texture2D>;
    using Texture_sptr = std::shared_ptr<Texture2D>;
    using Texture_wptr = std::weak_ptr<Texture2D>;

//--------------------------------------------------

//--------------------------------------------------
//----------------[MATERIAL]------------------------
//--------------------------------------------------
    class Material;
    using Material_uptr = std::unique_ptr<Material>;
    using Material_sptr = std::shared_ptr<Material>;
    using Material_wptr = std::weak_ptr<Material>;
//--------------------------------------------------

//--------------------------------------------------
//---------------[OGL_BUFFER]-----------------------
//--------------------------------------------------
    class OGL_Buffer;
    using OGL_Buffer_uptr = std::unique_ptr<OGL_Buffer>;
    using OGL_Buffer_sptr = std::shared_ptr<OGL_Buffer>;
    using OGL_Buffer_wptr = std::weak_ptr<OGL_Buffer>;

//--------------------------------------------------

//--------------------------------------------------
//---------------[OBJECT]-----------------------
//--------------------------------------------------
    using PickingID_t = uint32_t;
    class Object;
    using Object_uptr = std::unique_ptr<Object>;
    using Object_sptr = std::shared_ptr<Object>;
    using Object_wptr = std::weak_ptr<Object>;
//--------------------------------------------------

//--------------------------------------------------
//------------[OGL_ShaderProgram]-------------------
//--------------------------------------------------
    class OGL_ShaderProgram;
    using OGL_ShaderProgram_uptr = std::unique_ptr<OGL_ShaderProgram>;
    using OGL_ShaderProgram_sptr = std::shared_ptr<OGL_ShaderProgram>;
    using OGL_ShaderProgram_wptr = std::weak_ptr<OGL_ShaderProgram>;

//--------------------------------------------------

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

    class OGL_FrameBuffer;
    using OGL_FrameBuffer_sptr = std::shared_ptr<OGL_FrameBuffer>;
}




using uint = unsigned int;
#endif // GETYPES_H_INCLUDED
