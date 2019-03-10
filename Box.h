#ifndef BOX_H
#define BOX_H

/*!
* \file Box.h
* \author Alexandre Brunet
* \date 25/02/2019
*/

#include "Shape.h"

namespace GEII
{
    /*!
    * \enum BoxUVPattern
    * \brief Définit le type d'UV à générer lors de la création d'un objet Box
    */
    enum class BoxUVPattern{
        FullFaces, /*!< La texture sera entière sur chaque face du cube */
        Skybox /*!< Le texture sera générée de façon a pouvoir dessiner une Skybox */
    };

    /*!
    * \class Box
    * \brief Classe dérivée de Shape, permet de construire une boîte
    */
    class Box : public Shape
    {
    public:
        /*!
        * \brief Construit une boite selon une taille donnée
        * \param width : Largeur de la boite
        * \param height : Hauteur de la boite
        * \param depth : Profondeur de la boite
        * \param uvPattern : voir BoxUVPattern
        */
        Box(float width, float height, float depth, BoxUVPattern uvPattern = BoxUVPattern::FullFaces);

        /*!
        * \brief Construit un Cube selon une taille donnée
        * \param bSize : Longueur des cotés du cube
        * \param uvPattern : voir BoxUVPattern
        */
        Box(float bSize, BoxUVPattern uvPattern = BoxUVPattern::FullFaces);
        virtual ~Box(void);

        /*!
        * \brief Permet d'accéder à la largeur de la boite
        * \return Largeur de la boite
        */
        float getWidth(void) const { return mWidth; }

        /*!
        * \brief Permet d'accéder à la hauteur de la boite
        * \return Hauteur de la boite
        */
        float getHeight(void) const { return mHeight; }

        /*!
        * \brief Permet d'accéder à la profondeur de la boite
        * \return Profondeur de la boite
        */
        float getDepth(void) const { return mDepth; }

         /*!
        * \brief Permet d'accéder à l'UVPattern de la boite
        * \return UVPattern de la boite
        */
        BoxUVPattern getUVPattern(void) const { return mUVPattern; }
    protected :
        Box(void) = delete;

        float mWidth, mHeight, mDepth;
        BoxUVPattern mUVPattern;
    };
}

#endif // BOX_H
