#ifndef BUFFER_H
#define BUFFER_H

/*!
* \file Obj_Loader.h
* \author Alexandre Brunet
* \date 28/01/2019
*/

#include "OGL_Object.h"
#include "geHeader.h"

namespace GEII
{
    /*!
    * \class OGL_Buffer
    * \brief Gère les objets OpenGL de type "buffer"
    */

    class OGL_Buffer : public OGL_Object
    {
    public:
        /*!
        * \brief Constructeur
        * Créé un objet OpenGL de type buffer à partir d'un tableau de données.
        * Un contexte OpenGL valide doit être actif lors de la construction de cet objet.\n
        * Lance une exception si la création du buffer échoue.
        * \param target : Cible du buffer.\n
        * Doit être une des constantes suivantes : GL_ARRAY_BUFFER, GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, GL_ELEMENT_ARRAY_BUFFER,
        * GL_PIXEL_PACK_BUFFER, GL_PIXEL_UNPACK_BUFFER, GL_TEXTURE_BUFFER, GL_TRANSFORM_FEEDBACK_BUFFER, ou GL_UNIFORM_BUFFER. )
        * \param usage : Information sur l'utilisation du buffer.\n
        * Doit être une des constantes suivantes : GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY,
        * GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, ou GL_DYNAMIC_COPY
        * \param sizeBytes : Taille totale du buffer en octets. Cette valeur ne peut pas ètre changée par la suite.
        * \param data : Données à envoyer dans le buffer. Si égal à nullptr aucune donnée n'est copiée dans le buffer.\n
        * Valeur par défaut : nullptr
        */
        OGL_Buffer(GLenum target, GLenum usage, unsigned int sizeBytes, const void* data = nullptr);

        /*!
        * \brief Destructeur.\n
        * Détruit le buffer OpenGL. Un contexte OpenGL valide doit être actif lors de la destruction de cet objet.
        */
        virtual ~OGL_Buffer(void);

        /*!
        * \brief Copie des données dans le buffer.\n
        * Un appel à bind() doit être éffectué avant l'appel de cette fonction.
        * \param data : Données à copier dans le buffer.
        * \param dataSizeBytes : Taille des données à copier dans le buffer en octets.
        * \param offsetBytes : Endroit ou les données commenceront à ètre copiées par rapport au début du buffer, exprimé en octets.
        */
        void sendSubData(const void* data, GLsizeiptr dataSizeBytes, GLintptr offsetBytes);

        /*!
        * \brief Active le buffer pour une utilisation ultérieure du buffer
        * (copie de données ou autre).
        */
        void bind(void) const override;

        /*!
        * \brief Désactive le buffer.\n
        * Une fois désactivé il est impossible d'utiliser le buffer. Pour réactiver le buffer un appel à bind() doit être réalisé.
        */
        void unbind(void) const override;

        /*!
        * \brief Retourne la taille du buffer en octet.\n
        * \return Taille du buffer, exprimé en octets.
        */
        int getSizeBytes(void) const;

        /*!
        * \brief Retourne la cible du buffer.\n
        * \return La cible du buffer (constante OpenGL).
        */
        GLenum getTarget(void) const;

        /*!
        * \brief Retourne l'utilisation buffer.\n
        * \return L'utilisation du buffer (constante OpenGL).
        */
        GLenum getUsage(void) const;
    private :
        const GLenum mGL_Target;
        const GLenum mGL_Usage;
        const int mSizeBytes;
    };

    /*!
    * \class VertexBuffer
    * \brief Spécialisation de OGL_Buffer pour la gestion des Vbo
    */
    class VertexBuffer : public OGL_Buffer{
    public:
        /*!
        * \brief Créé un Vbo
        * \param sizeBytes : Taille du Vbo, exprimée en octets.
        * \param data : Données à copier dans le Vbo. Rien n'est copié si le pointeur est nul.
        */
        VertexBuffer(unsigned int sizeBytes, const void* data = nullptr) : OGL_Buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, sizeBytes, data){ }
    };

    /*!
    * \class IndexBuffer
    * \brief Spécialisation de OGL_Buffer pour la gestion des Ibo
    */
    class IndexBuffer : public OGL_Buffer{
    public:
        /*!
        * \brief Créé un Ibo
        * \param sizeBytes : Taille du Ibo, exprimée en octets.
        * \param data : Données à copier dans le Ibo. Rien n'est copié si le pointeur est nul.
        */
        IndexBuffer(unsigned int sizeBytes, const void* data = nullptr) : OGL_Buffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, sizeBytes, data){ }
    };
}



#endif // BUFFER_H
