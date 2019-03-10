#ifndef CYLINDER_H
#define CYLINDER_H

/*!
* \file Cylinder.h
* \author Alexandre Brunet
* \date 25/02/2019
*/

#include "Shape.h"

namespace GEII
{
    /*!
    * \class Cylinder
    * \brief Classe dérivée de Shape, permet de générer un cylindre
    */
    class Cylinder : public Shape
    {
        public:
            /*!
            * \brief Génère un Cylindre
            * \param edgesCount : Nombre de cotés de la base du cylindre
            * \param radius : Rayon de la base du cylindre
            * \param height : Hauteur du cylindre
            */
            Cylinder(unsigned int edgesCount, float radius, float height);
            virtual ~Cylinder(void);

            /*!
            * \brief Permet d'accéder à la hauteur du cylindre
            * \return Hauteur du cylindre;
            */
            float getHeight(void) const { return mHeight; }


            /*!
            * \brief Permet d'accéder au rayon du cylindre
            * \return Rayon du cylindre;
            */
            float getRadius(void) const { return mRadius; }

            /*!
            * \brief Permet d'accéder au nombre de cotés du cylindre
            * \return Nombre de cotés du cylindre
            */
            unsigned int getEdgesCount(void) const { return mEdgesCount; }

        protected:
            float mRadius, mHeight;
            unsigned int mEdgesCount;
    };
}
#endif // CYLINDER_H
