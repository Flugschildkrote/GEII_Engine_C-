#ifndef THORUS_H
#define THORUS_H

/*!
* \file Thorus.h
* \author Alexandre Brunet
* \date 25/02/2019
*/

#include "Shape.h"

namespace GEII
{
    /*!
    * \class Torus
    * \brief Classe dérivée de Shape, permet de générer un tore
    */
    class Torus : public Shape
    {
    public:
        /*!
        * \brief Génère un tore
        * \param sideCount : Nombre de cotés du tore
        * \param slice : Nombre d'anneaux horizontaux composant le tore
        * \param mainRadius : Rayon principal du tore
        * \param secondaryRadius : Rayon secondaire du tore
        */
        Torus(unsigned int sideCount, unsigned int slice, float mainRadius, float secondaryRadius);
        virtual ~Torus(void);

        /*!
        * \brief Permet d'accéder au nombre de cotés du tore
        * \return Nombre de cotés du tore
        */
        unsigned int getSideCount(void) const { return mSideCount; }

        /*!
        * \brief Permet d'accéder au nombre d'anneaux horizontaux composant le tore
        * \return Nombre d'anneaux horizontaux composant le tore
        */
        unsigned int getSlice(void) const { return mSlice; }

        /*!
        * \brief Permet d'accéder au rayon principal du tore
        * \return Rayon principal du tore
        */
        float getMainRadius(void) const { return mMainRadius; }

        /*!
        * \brief Permet d'accéder au rayon secondaire du tore
        * \return Rayon secondaire du tore
        */
        float getSecondaryRadius(void) const { return mSecondaryRadius; }
    protected:
        unsigned int mSideCount, mSlice;
        float mMainRadius, mSecondaryRadius;
    };
}

#endif // THORUS_H
