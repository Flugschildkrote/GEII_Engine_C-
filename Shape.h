#ifndef SHAPE_H
#define SHAPE_H

/*!
* \file Shape.h
* \author Alexandre Brunet
* \date 25/02/2019
*/

#include "geHeader.h"
#include <vector>
#include "OGL_Buffer.h"
#include <memory>

namespace GEII
{
    constexpr float PI = 3.14159265358979323846264338327950288f;

    /*!
    * \class Shape
    * \brief Gère les modèles 3D
    */
    class Shape{
    public:
        /*!
        * \brief Permet de construire un modèle 3D, que l'on pourra dessiner avec OpenGL
        *
        * Un contexte OpenGL valide doit être actif lors de toute la durée de vie de cet objet (destruction incluse)
        *
        * \param vertice : Tableau contenant toutes les position des points (1 point = 3 cases (x,y,z))
        * \param textUV : Tableau contenant toutes les coordonnées UV des points (1 point = 2 cases(U,V))
        * \param normals : Tableau contenant toutes les normales des points (1 point = 3 cases (x,y,z))
        * \param indices : Tableau contenant la liste des indices permettant de définir l'ordre des points lors du dessin.
        * \param drawMode : Mode de dessin OpenGL. Doit être une des constantes suivantes :\n
        * - GL_POINTS\n
        * - GL_LINE_STRIP\n
        * - GL_LINE_LOOP\n
        * - GL_LINES\n
        * - GL_LINE_STRIP_ADJACENCY\n
        * - GL_LINES_ADJACENCY\n
        * - GL_TRIANGLE_STRIP\n
        * - GL_TRIANGLE_FAN\n
        * - GL_TRIANGLES\n
        * - GL_TRIANGLE_STRIP_ADJACENCY\n
        * - GL_TRIANGLES_ADJACENCY
        */
        Shape(const std::vector<float> &vertice, const std::vector<float> &textUV, const std::vector<float> &normals, const std::vector<unsigned int> &indices, GLenum drawMode);

        virtual ~Shape(void);

        /*!
        * \brief Effectue un dessin de l'objet contenu.
        */
        void draw(void);
    protected :
        Shape(GLenum drawMode);
        /*!
        * \brief Crée les ressources OpenGL nécéssaires afin de pouvoir dessiner l'objet.
        */
        void load(const std::vector<float> &vertice, const std::vector<float> &textUV, const std::vector<float> &normals, const std::vector<unsigned int> &indices);

        GEII::OGL_Buffer_uptr mVbo; /*!< VBO contenant les points de l'objet */
        GEII::OGL_Buffer_uptr mIbo; /*!< IBO contenant les indices pour dessiner l'objet */
        GLenum mDrawMode; /*!< Mode de dessin OpenGL */

        unsigned int mVerticeCount; /*!< Nombre de Vertex du modèle.*/
        unsigned int mIndicesCount; /*!< Nombre d'indices contenus dans l'IBO */
    };
}

/*  void loadCamera(float fov , float ratio, float zNear, float zFar){
        // formes : un cube déformé selon l'angle
        glm::vec3 nearPlane_LD, nearPlane_RD,nearPlane_LU, nearPlane_RU, farPlane_LD, farPlane_RD,farPlane_LU, farPlane_RU;

        float phi = fov/360.0f*glm::pi<float>(); // semi Angle vertical en radians
        float cosPhi = std::cos(phi); // ratio de l'angle
        float sinPhi = std::sin(phi); // ratio de l'angle

        // cos(rad)*factor = near -> factor = near/cos(rad);
        float factor = zNear/cosPhi;
        float y1 = sinPhi*factor;
        factor = zFar/cosPhi;
        float y2 = sinPhi*factor;
        float x1 = y1/ratio;
        float x2 = y2/ratio;*/

        // std::vector<float> verticeList = {
         //   /* Face Avant*/   -x1,-y1, zNear,    x1,-y1, zNear,       x1, y1, zNear,         -x1, y1, zNear,
         //   /* Face Droite*/   x1,-y1, zNear,    x2,-y2, zFar,       x1, y1, zNear,           x2, y2, zFar,
         //   /* Face Arrière*/  x2,-y2,-zFar,     -x2,-y2,-zFar,      -x2, y2,-zFar,           x2, y2,-zFar,
         //   /* Face Gauche*/  -x1,-y1, zNear,   -x2,-y2, zFar,       -x1, y1, zNear,         -x2, y2, zFar,
        //    /* Face Up*/      -x, y, z,      x, y, z,       x, y,-z,          -x, y,-z,
         //   /* Face Bas*/      x,-y, z,     -x,-y, z,      -x,-y,-z,           x,-y,-z
       // };

       //  std::vector<float> verticeList = {
         //   /* Face Avant*/   -x1,-y1, -zNear,    x1,-y1, -zNear,       x1, y1, -zNear,         -x1, y1, -zNear,
         //   /* Face Arrière*/ -x2,-y2, -zFar,     x2,-y2, -zFar,        x2, y2, -zFar,          -x2, y2, -zFar,
         //                   0.0f, 0.0f, 0.0f
         //   /* Face Gauche*/  -x1,-y1, zNear,   -x2,-y2, zFar,       -x1, y1, zNear,         -x2, y2, zFar,
        //    /* Face Up*/      -x, y, z,      x, y, z,       x, y,-z,          -x, y,-z,
         //   /* Face Bas*/      x,-y, z,     -x,-y, z,      -x,-y,-z,           x,-y,-z*/
       /* };

        std::vector<unsigned int> indice = {
            0,1,  1,2,  2,3,  3,0,
            4,5,  5,6,  6,7,  7,4,
            2,6,  3,7,  0,4,  1,5,
            8,0,  8,3,  8,2,  8,1
        };

        std::vector<float> normals, uvs;
        mDrawMode = GL_LINES;
        load(verticeList, uvs, normals, indice);
    }*/

#endif // SHAPE_H
