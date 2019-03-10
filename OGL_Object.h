#ifndef OGL_OBJECT_H
#define OGL_OBJECT_H

/*!
* \file OGL_Texture.h
* \author Alexandre Brunet
* \date 12/02/2019
*/

#include "geHeader.h"
#include "Log.h"

namespace GEII
{   /*!
    * \class OGL_Object
    * \brief Gère les objets OpenGL
    *
    * Cette classe est abtraite.\n
    * Un contexte OpenGL valide doit être actif durant toute la durée de vie des instances de cette classe et des ses classes dérivées.
    */
    class OGL_Object
    {
    public:
        /*!
        * \brief Constructeur par défaut.
        *
        * Les ressources OpenGL devront être initialisées dans le constructeur des classes dérivées.
        */
        OGL_Object(void);

        /*!
        * \brief Destructeur
        *
        * Les ressources OpenGL devront être libérées dans le destructeur des classes dérivées.
        */
        virtual ~OGL_Object(void);

        /*!
        * \brief Interdit l'utilisation du constructeur de copie.
        */
        OGL_Object(const OGL_Object &other) = delete;

        /*!
        * \brief Interdit l'utilisation l'opérateur de copie.
        */
        OGL_Object& operator=(const OGL_Object &other) = delete;

        /*!
        * \brief Active l'objet OpenGL géré pour une utilisation ultérieure.
        *
        * Vituelle pure.\n
        * Cette fonction doit être implémentée dans chaque classe fille.
        */
        virtual void bind(void) const = 0;

        /*!
        * \brief Désactive l'objet OpenGL.
        *
        * Vituelle pure.\n
        * Cette fonction doit être implémentée dans chaque classe fille.
        */
        virtual void unbind(void) const = 0;

        /*!
        * \brief Retourne l'ID de l'objet OpenGL.
        * \return L'ID de l'objet OpenGL.
        */
        GLuint getID(void) const;
    protected:
        GLuint mGL_ID;
    };
}

#endif // OGL_OBJECT_H
