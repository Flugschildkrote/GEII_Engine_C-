#ifndef CONE_H
#define CONE_H

/*!
* \file Cone.h
* \author Alexandre Brunet
* \date 25/02/2019
*/

#include "Shape.h"

namespace GEII
{
    /*!
    * \class Cone
    * \brief Classe dérivée de Shape, permet de générer un Cone
    */
    class Cone : public Shape
    {
    public:
        /*!
        * \brief Génère un Cone
        * \param edgesCount : Nombre de cotés de la base du cone
        * \param radius : Rayon de la base du cone
        * \param height : Hauteur du cone
        */
        Cone(unsigned int edgesCount, float radius, float height);
        virtual ~Cone(void);

        /*!
        * \brief Permet d'accéder à la hauteur du cone
        * \return Hauteur du cone;
        */
        float getHeight(void) const { return mHeight; }

        /*!
        * \brief Permet d'accéder au rayon du cone
        * \return Rayon du cone;
        */
        float getRadius(void) const { return mRadius; }

        /*!
        * \brief Permet d'accéder au nombre de cotés du cone
        * \return Nombre de cotés du cone
        */
        unsigned int getEdgesCount(void) const { return mEdgesCount; }
    protected:
        float mRadius, mHeight;
        unsigned int mEdgesCount;
    };
}

#endif // CONE_H
