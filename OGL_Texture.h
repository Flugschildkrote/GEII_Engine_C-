#ifndef OGL_TEXTURE_H
#define OGL_TEXTURE_H

/*!
* \file OGL_Texture.h
* \author Alexandre Brunet
* \date 31/01/2019
*/

#include "OGL_Object.h"
#include "geHeader.h"

namespace GEII
{
    /*!
    * \class OGL_Texture
    * \brief Gère les objets OpenGL de type texture. Classe abstraite
    *
    * Cette classe est une classe abstraite, c'est à dire qui ne peut pas être instanciée directement.\n
    * Afin de gérer les textures classique par exemple, utiliser la classe Texture2D dérivée.\n
    * La texture est créée lors de la construction de l'objet et détruite automatiquement lors de sa destruction.\n
    * Un contexte OpenGL valide doit être valide pendant toute la durée de vie de cet objet (destruction incluse).
    */
    class OGL_Texture : public OGL_Object
    {
    public:
        /*!
        * \brief Génère une texture OpenGL
        *
        * \param target : Type de la texture (GL_TEXTURE_1D, GL_TEXTURE_2D...).
        * \param genMipMaps : Indique si des MipMaps doivent être générées lors de la création de la texture.
        */
        OGL_Texture(GLenum target, bool genMipMaps);

        /*!
        * \brief Détruit la texture OpenGL associée.
        */
        virtual ~OGL_Texture(void) = 0;

        /*!
        * \brief Active la Texture pour une utilisation ultérieure.
        */
        virtual void bind(void) const override;

        /*!
        * \brief Désactive la Texure.\n
        *
        * Une fois désactivé il est impossible d'utiliser la Texture. Pour le réactiver un appel à bind() est nécéssaire.
        */
        virtual void unbind(void) const override;

        /*!
        * \brief Retourne le format interne de la texture.
        *
        * (Format utilisé pour le stockage par la carte graphique)
        * \return Format interne de la texture.
        */
        GLenum getInternalFormat(void) const { return mGL_InternalFormat; }

        /*!
        * \brief Retourne le format de la texture.
        *
        * Le format représente le format des données composant la texture (RGB, RG etc..)
        * \return Format de la texture.
        */
        GLenum getFormat(void) const { return mGL_Format; }

        /*!
        * \brief Retourne un booleen indiquant si la texture possède des mipmaps.
        *
        * \return Booléen indiquant si la texture possède des mipmaps.
        */
        bool hasMipMaps(void) const { return mHasMipMaps; }
    protected:
        char getComponentsFromFormat(GLenum format) const;
        GLenum getFormatFromComponents(unsigned int components) const;

        const GLenum mGL_Target;
        GLenum mGL_Format, mGL_InternalFormat;
        bool mHasMipMaps;
    };
}

#endif // OGL_TEXTURE_H
