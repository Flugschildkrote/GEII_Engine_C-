#ifndef OGLSHADERS_H
#define OGLSHADERS_H

/*!
* \file OGL_Shader.h
* \author Alexandre Brunet
* \date 29/01/2019
*/

#include "geHeader.h"
#include "OGL_Object.h"

namespace GEII
{
    /*!
    * \class OGL_ShaderProgram
    * \brief Gère les objets OpengGL de type "ShaderProgram"
    *
    * Les attributs de vertices utilisés dans le code GLSL sont les suivants :\n
    * "in_Vertex"  (location 0) : vec3 comportant les positions des points du modèle 3D\n
    * "in_Normal"  (location 1) : vec3 contenant la normale de chaque point.\n
    * "in_UV"      (location 2) : vec2 contenant les coordonnées de textures de chaque point.\n
    * "in_Tangent" (location 3) : vec3 contenant les vecteurs tangents de chaque point. (utilisé pour le normal mapping)\n
    * "in_BiTangent" (location 4) : vec3 contenant les vecteurs bitangents de chaque point. (utilisé pour le normal mapping)\n
    */

    class OGL_ShaderProgram : public OGL_Object
    {
        public:
            /*!
            * \brief Crée un Shader à partir de code GLSL
            *
            * Lance une exception si il y a une erreur lors de la création du shader.
            * \param vShader :\n
            * - isFile = true -> Nom du fichier contenant le code GLSL du vertex shader.\n
            * - isFile = false -> Code GLSL du vertex shader.\n
            * \param fShader :\n
            * - isFile = true -> Nom du fichier contenant le code GLSL du fragment shader.\n
            * - isFile = false -> Code GLSL du fragment shader.\n
            * \param isFile : indique si les chaines de caracères sont des noms de fichiers (true) ou du code GLSL (false).
            */
            OGL_ShaderProgram(const std::string& vShader, const std::string& fShader, bool isFile = true);

            /*!
            * \brief Détruit le Shader
            */
            virtual ~OGL_ShaderProgram(void);


            /*!
            * \brief Active le Shader pour une utilisation ultérieure. (Dessin ou envoi de variable uniformes par exemple).
            */
            virtual void bind(void) const override;

            /*!
            * \brief Désactive le Shader.\n
            *
            * Une fois désactivé il est impossible d'utiliser le Shader. Pour le réactiver un appel à bind() est nécéssaire.
            */
            virtual void unbind(void) const override;

            /*!
            * \brief Location d'une variable uniforme dans le code GLSL.
            *
            * Permet d'accéder à la location d'une variable uniforme afin de pouvoir l'utiliser par la suite.\n
            * Un appel à bind() doit être effectué avant l'appel de cette fonction.
            * \return Location de la variable. -1 si la variable est introuvable.
            */
            GLint getUniformLocation(const std::string &name) const;

            /*!
            * \brief Permet de copier une mat4 dans une variable uniforme.
            *
            * Un appel à bind() doit être effectué avant l'appel de cette fonction.
            * \param location : Location de la variable uniforme obtenue avec getUniformLocation().
            * \param mat4 : Matrice à copier dans la variable uniforme.
            */
            void setUniform(GLuint location, const glm::mat4 &mat4) const;

            /*!
            * \brief Permet de copier un vec4 dans une variable uniforme.
            *
            * Un appel à bind() doit être effectué avant l'appel de cette fonction.
            * \param location : Location de la variable uniforme obtenue avec getUniformLocation().
            * \param vec4 : Vecteur à copier dans la variable uniforme.
            */
            void setUniform(GLuint location, const glm::vec4 &vec4) const;

            /*!
            * \brief Permet de copier un vec3 dans une variable uniforme.
            *
            * Un appel à bind() doit être effectué avant l'appel de cette fonction.
            * \param location : Location de la variable uniforme obtenue avec getUniformLocation().
            * \param vec3 : Vecteur à copier dans la variable uniforme.
            */
            void setUniform(GLuint location, const glm::vec3 &vec3) const;

            /*!
            * \brief Permet de copier un float dans une variable uniforme.
            *
            * Un appel à bind() doit être effectué avant l'appel de cette fonction.
            * \param location : Location de la variable uniforme obtenue avec getUniformLocation().
            * \param value : Valeur à copier dans la variable uniforme.
            */
            void setUniform(GLuint location, float value) const;

            /*!
            * \brief Permet de copier un entier dans une variable uniforme.
            *
            * Un appel à bind() doit être effectué avant l'appel de cette fonction.
            * \param location : Location de la variable uniforme obtenue avec getUniformLocation().
            * \param value : Valeur à copier dans la variable uniforme.
            */
            void setUniform(GLuint location, int value) const;

            /*!
            * \brief Permet de déterminer l'unité de texture utilisée par une variable uniforme de type Sampler2D
            *
            * Un appel à bind() doit être effectué avant l'appel de cette fonction.
            * Cette fonction est identique a setUniform(location, [int]), cependant son nom la rend plus explicite.
            * \param location : Location de la variable uniforme obtenue avec getUniformLocation().
            * \param textureUnit : Unité de texture à utiliser par le Sampler2D.
            */
            void setUniformTexture(GLuint location, GLint textureUnit) const;
        private:
            virtual void destroy(void);
            void compileShader(GLuint &shader, GLenum type, const std::string& file, bool isFile);
    };
}

#endif // OGLSHADERS_H
