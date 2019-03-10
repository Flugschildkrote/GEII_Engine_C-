#ifndef SQUARE_H
#define SQUARE_H

/*!
* \file Square.h
* \author Alexandre Brunet
* \date 25/02/2019
*/

#include "Shape.h"

namespace GEII
{
    /*!
    * \class Square
    * \brief Classe dérivée de Shape, permet de générer un Carré
    */
    class Square : public Shape
    {
    public:
        /*!
        * \brief Génère un carré le long de l'axe X (normale dans le sens de l'axe Z)
        * \param width : largeur du carré
        * \param height : hauteur du carré
        */
        Square(float width, float height);
        virtual ~Square(void);

        /*!
        * \brief Permet d'acceder à la largeur du carré
        * \return Largeur du carré
        */
        float getWidth(void) const { return mWidth; }

        /*!
        * \brief Permet d'acceder à la hauteur du carré
        * \return Hauteur du carré
        */
        float getHeight(void) const { return mHeight; }
    protected :
        float mWidth, mHeight;

        Square(void) = delete;
    };
}
#endif // SQUARE_H
