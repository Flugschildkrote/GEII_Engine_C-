#ifndef OGL_FRAMEBUFFER_H
#define OGL_FRAMEBUFFER_H

/*!
* \file OGL_FrameBuffer.h
* \author Alexandre Brunet
* \date 28/01/2019
*/

#include "geHeader.h"
#include "OGL_Object.h"

#define FRAMEBUFFER_DISABLE_READ  0x01
#define FRAMEBUFFER_DISABLE_WRITE 0x02

namespace GEII
{
    /*!
    * \enum AttachmentTarget
    * \brief Utilisé pour décrire comment sera utilisé un attachement lors de la création d'un FrameBuffer.
    */
    enum class AttachmentTarget{
        Texture, /*!< L'attachement sera lié à un objet Texture2D. */
        RenderBuffer, /*!< L'attachement sera lié à un render buffer. */
        None /*!< L'attachement ne sera pas créé. */
    };

    /*!
    * \class OGL_FrameBuffer
    * \brief Gestion des objets OpenGL de type Frame Buffer (FBO).
    *
    * Cette classe est utilisée pour gérer les Frame Buffer Objets (FBO).\n
    * Un FBO est utilisé pour effectuer un rendu sur une cible autre qu'un écran, ce qui permet de déssiner des scenes 3D directement dans une texture par exemple.
    */
    class OGL_FrameBuffer : public OGL_Object
    {
    public:
        /*!
        * \brief Constructeur
        *
        * Construit un FBO avec une taille donnée.\n
        * Une exception est lancée si la création du FBO échoue.
        * \param width : Largeur du FBO en pixels.
        * \param height : Hauteur du FBO en pixels.
        * \param colorTarget : Définit comment créer l'attachement GL_COLOR_ATTACHMENT0
        * \param depthTarget : Définit comment créer l'attachement GL_DEPTH_ATTACHMENT
        * \param flags : Définit les règles d'écriture et de lecture vers le FBO.\n
        * Flags dosponibles : FRAMEBUFFER_DISABLE_READ, FRAMEBUFFER_DISABLE_WRITE
        */
        OGL_FrameBuffer(unsigned int width, unsigned int height, AttachmentTarget colorTarget, AttachmentTarget depthTarget, int flags = 0);

        /*!
        * \brief Destructeur
        *
        * Détruit le FBO et les Renderbuffers si ils existent.
        */
        virtual ~OGL_FrameBuffer(void);

        /*!
        * \brief Active le FBO pour une utilisation ultérieure. (Dessin du FBO ou autre).
        */
        virtual void bind(void) const override;

        /*!
        * \brief Désactive le FBO.\n
        *
        * Une fois désactivé il est impossible d'utiliser le FBO. Pour le réactiver un appel à bind() est nécéssaire.
        */
        virtual void unbind(void) const override;

        /*!
        * \brief Getter sur la texture attachée à GL_COLOR_ATTACHMENT0.
        *
        * Si elle n'existe pas la valeur retournée pointe sur nullptr.
        * \return Texture attachée à GL_COLOR_ATTACHMENT0.
        */
        Texture_sptr getColorTexture(void) const { return mColorTexture; }

        /*!
        * \brief Getter sur la texture attachée à GL_DEPTH_ATTACHMENT.
        *
        * Si elle n'existe pas la valeur retournée pointe sur nullptr.
        * \return Texture attachée à GL_DEPTH_ATTACHMENT.
        */
        Texture_sptr getDepthTexture(void) const { return mDepthTexture; }

         /*!
        * \return Largeur du FBO en pixels.
        */
        inline unsigned int getWidth(void) const { return mWidth; }

        /*!
        * \return Hauteur du FBO en pixels.
        */
        inline unsigned int getHeight(void) const { return mHeight; }

    private:
        void destroy(void) const;

        unsigned int mWidth, mHeight;
        Texture_sptr mColorTexture;
        Texture_sptr mDepthTexture;
        std::vector<GLuint> mRenderBuffers;
        AttachmentTarget mAttachementTargets[2];
    };

    /*!
    * \class ShadowMap_FrameBuffer
    * \brief Spécialisation de OGL_FrameBuffer pour l'utilisation des lampes.
    *
    * La shadow map est stocquée dans le texture de profondeur (GL_DEPTH_ATTACHMENT).\n
    * Aucune texture n'est attachée à la couleur.\n
    * La lecture et l'écriture vers le FBO sont désactivées.
    */
    class ShadowMap_FrameBuffer : public OGL_FrameBuffer
    {
    public :
        /*!
        * \brief Constructeur
        * \param width : Largeur du FBO en pixels.
        * \param height : Hauteur du FBO en pixels.
        */
        ShadowMap_FrameBuffer(unsigned int width, unsigned int height)
            : OGL_FrameBuffer(width, height, AttachmentTarget::None, AttachmentTarget::Texture, FRAMEBUFFER_DISABLE_READ | FRAMEBUFFER_DISABLE_WRITE ) { }
    };
}

#endif // OGL_FRAMEBUFFER_H
