#ifndef RENDER_H
#define RENDER_H

/*!
* \file Render.h
* \author Alexandre Brunet
* \date 12/02/2019
* \brief Contient les définitions des classes Render, RenderPhong et RenderShadowMapping
*/

#include "geHeader.h"

#define GE_UNKNOWN_UNIFORM -1
#define PROJ_SIZE 30.0f
#define LIGHT_SUN_PROJECTION glm::ortho(-PROJ_SIZE, PROJ_SIZE, -PROJ_SIZE, PROJ_SIZE, 0.1f, 200.0f)
#define LIGHT_SPOT_PROJECTION(angle) glm::perspective(glm::radians(angle), 1.0f, 0.1f, 200.0f)

#ifndef MAX_LIGHTS
    #define MAX_LIGHTS 2
#endif // MAX_LIGHTS

namespace GEII
{
//----------------------------------------------------------------------
//-----------------------------[RENDER]---------------------------------
//----------------------------------------------------------------------
    /*!
    * \class Render
    * \brief Gère l'affichage des objets à l'écran.
    *
    * Classe abstraite. Cette classe doit être dérivée pour être utilisée.\n
    * C'est à partir de cette classe que les objets vont être dessinés.
    */
    class Render
    {
        public:
            /*!
            * \brief Constructeur
            *
            * Utilisation dans les classes dérivées :\n
            * 1 - Charger différents shaders\n
            * 2 - Stocker les locations des variables uniformes avec un appel à initUniforms().
            */
            Render(void);

            /*!
            * \brief Destructeur
            */
            virtual ~Render(void);

        protected:
            /*!
            * \brief Permet d'acceder à la location OpenGL d'une variable uniforme.
            *
            * Lance un exception si la variable uniforme est introuvable.
            *
            * \param uniformName : Nom de la variable uniforme de le shader.
            * \param shader : Pointeur sur le shader ou se trouve la variable uniforme.
            * \return Location OpenGL de la variable uniforme.
            */
            GLint getUniform(const std::string &uniformName, OGL_ShaderProgram* shader) const;

            /*!
            * \brief Stocke les locations des variables uniformes afin de pouvoir y accéder plus tard.
            *
            * Doit être implémentée dans chaque classe dérivée.
            */
            virtual void initUniforms(void) = 0;
            OGL_ShaderProgram_uptr mShaderProgram;
    };

//----------------------------------------------------------------------
//--------------------------[RENDER PHONG]------------------------------
//----------------------------------------------------------------------
    /*!
    * \class RenderPhong
    * \brief Dessine les objets avec un éclairage de phong
    */
    class RenderPhong : public Render
    {
        public:
            /*!
            * \brief Constructeur
            */
            RenderPhong(void);

            /*!
            * \brief Destructeur
            */
            virtual ~RenderPhong(void);

            /*!
            * \brief Permet de dessiner les Object contenus dans une Scene.
            * \param scene : Pointeur sur la Scene à dessiner.
            * \param camera : Caméra a partir de laquelle la Scene sera dessinée.
            * \param ligths : Tableau contenant les lumières à utiliser lors du dessin.
            */
            virtual void draw(Scene *scene, Camera *camera) const;
        protected:
            virtual void initUniforms(void);
        //**************[UNIFORMS]***************************************/
            /**[MATERIAL]**/
            GLint mU_MatKd, mU_MatKs, mU_MatNs, mU_MatAlpha, mU_MatText, mU_MatUseTexture, mU_MatLightSensitive;
            /**[LIGHT]**/
            GLint mSceneAmbiantColor;
            GLint mU_LightMatrix[MAX_LIGHTS], mU_ShadowMap[MAX_LIGHTS];
            GLint mU_LightDir[MAX_LIGHTS], mU_LightPower[MAX_LIGHTS], mU_LightAmbiant[MAX_LIGHTS];
            GLint mU_LightType[MAX_LIGHTS];
            GLint mU_LightPos[MAX_LIGHTS];
            GLint mU_LightAngle[MAX_LIGHTS];
            GLint mU_LightCount;
            /**[MODEL]**/
            GLint mU_ModelMatrix;
            /**[CAMERA]**/
            GLint mU_MVP;
            GLint mU_WorldEyePos;
            /**[NORMAL MAPPING]**/
            GLuint mU_MatUseNormalMapping;
            GLuint mU_MatNormalMap;
            /**[SKYBOX]*/
            GLint mU_Sky_MVP, mU_Sky_Texture;
        /***************************************************************/

            OGL_ShaderProgram_uptr mSkyboxShader;
    };

    /*!
    * \class RenderShadowMapping
    * \brief Dessine les objets pour générer les ShadowMaps
    *
    * A revoir le fontionnement des light avec intégration ou non des shadow maps.
    */
    class RenderShadowMapping : public Render{
    public :
        RenderShadowMapping(void);
        ~RenderShadowMapping(void);

        virtual void draw(Scene* scene, Light *light);
    protected :
        virtual void initUniforms(void);
        /**************[UNIFORMS]***************************************/
        /**[CAMERA]**/
        GLint mU_MVP;
    /***************************************************************/
    };
}
#endif // RENDER_H
