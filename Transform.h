#ifndef TRANSFORM_H
#define TRANSFORM_H

/*!
* \file Transform.h
* \author Alexandre Brunet
* \date 18/02/2019
*/

#include "geHeader.h"

namespace GEII
{
    /*!
    * \enum SpaceReference
    * \brief Spécifie la référence lors d'une opération de transformation sur un Objet de type Transform
    */
    enum class SpaceReference{
        WORLD, /*!< Utilisé lors d'une transformation par rapport aux axes du monde */
        LOCAL  /*!< Utilisé lors d'une transformation par rapport aux axes locaux de l'objet */
    };

    constexpr glm::vec3 AXIS_FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
    constexpr glm::vec3 AXIS_BACK = -AXIS_FRONT;
    constexpr glm::vec3 AXIS_LEFT = glm::vec3(-1.0f, 0.0f, 0.0f);
    constexpr glm::vec3 AXIS_RIGHT = -AXIS_LEFT;
    constexpr glm::vec3 AXIS_UP = glm::vec3(0.0f, 1.0f, 0.0f);
    constexpr glm::vec3 AXIS_DOWN = -AXIS_UP;

    /*!
    * \class Transform
    * \brief Gère les transformations des objets
    *
    * Permet d'éffectuer les opérations de base sur les objets (translation et rotation)
    */
    class Transform
    {
    public:
        Transform(void);
        virtual ~Transform(void);

        /*!
        * \brief Effectue une translation du Transform selon la référence choisie.
        * \param translation : Vecteur 3D indiquant la direction et la distance de la translation à éffectuer.
        * \param spaceRef : voir SpaceReference
        */
        void translate(const glm::vec3 &translation, SpaceReference spaceRef = SpaceReference::LOCAL);

        /*!
        * \brief Effectue une rotation du Transform selon la référence choisie.
        * \param angle : angle de rotation en radians.
        * \param axis : Axe autour du quel sera effectué la rotation.
        * \param spaceRef : voir SpaceReference
        */
        void rotate(float angle, const glm::vec3 &axis, SpaceReference spaceRef = SpaceReference::LOCAL);

        /*!
        * \brief Transforme un axel local en axe global (espace local du transform -> espace monde)
        * \param localAxis : Axe à transformer
        * \return localAxis transposé dans l'espace monde.
        */
        glm::vec3 getWorldAxis(const glm::vec3 &localAxis) const { return mTransformations*glm::vec4(localAxis, 0.0f); };

        /*!
        * \brief Permet d'acceder à la position du transform dans le monde
        * \return Position du transform dans l'espace monde.
        */
        glm::vec3 getPosition(void) const { return glm::vec3(mTransformations*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)); }
       // bool getTrackingState(void) const { return mTrackingEnabled; }

        /*!
        * \brief Permet d'acceder à la matrice de transformations transform.
        * \return Matrice (4x4) de tansformation du transform. (par rapport au point (0,0,0)).
        */
        glm::mat4 getTransformations(void) const { return mTransformations; }

        /*!
        * \brief Définit une nouvelle position pour le transform.
        * \param pos : Nouvelle position du transform.
        */
        void setPosition(const glm::vec3 &pos);

        /*!
        * \brief Définit une nouvelle transformation pour le transform.
        * \param transformations : Nouvelle transformation du transform.
        */
        void setTransformations(const glm::mat4 &transformations){ mTransformations = transformations; }
    private:
        //void updateTracking(void);

        glm::mat4 mTransformations;

        // Tracking informations
        /*bool mTrackingEnabled;
        glm::vec3 mTrackingPoint;*/
    };
}
#endif // TRANSFORM_H
