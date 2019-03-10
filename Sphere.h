#ifndef SPHERE_H
#define SPHERE_H

/*!
* \file Sphere.h
* \author Alexandre Brunet
* \date 25/02/2019
*/

#include "Shape.h"

namespace GEII
{
    /*!
    * \class Sphere
    * \brief Classe dérivée de Shape, permet de générer une sphère
    */
    class Sphere : public Shape
    {
        public:
            /*!
            * \brief Génère une sphère
            * \param radius : Rayon de la sphère
            * \param vRingCount : Nombre d'anneaux verticaux composant la sphère
            * \param hRingCount : Nombre d'anneaux horizontaux composant la sphère
            */
            Sphere(float radius, int vRingCount, int hRingCount);
            virtual ~Sphere(void);

            /*!
            * \brief Permet d'accèder au rayon de la sphère
            * \return Rayon de la sphère
            */
            float getRadius(void) const { return mRadius; }


            /*!
            * \brief Permet d'accèder au nombre d'anneaux verticaux de la sphère
            * \return Nombre d'anneaux verticaux de la sphère
            */
            int getVRingCount(void) const { return mVRingCount; }

            /*!
            * \brief Permet d'accèder au nombre d'anneaux horizontaux de la sphère
            * \return Nombre d'anneaux horizontaux de la sphère
            */
            int getHRingCount(void) const { return mHRingCount; }
        protected:
            const float mRadius;
            const int mVRingCount, mHRingCount;
    };
}

#endif // SPHERE_H
