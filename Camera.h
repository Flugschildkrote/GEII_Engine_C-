#ifndef CAMERA_H
#define CAMERA_H

/*!
* \file Camera.h
* \author Alexandre Brunet
* \date 18/02/2019
*/

#include "geHeader.h"
#include "Transform.h"

namespace GEII
{
    /*!
    * \enum CameraType
    * \brief Utilisé pour définir le type de Camera utilisé.
    */
    enum CameraType : uint8_t{
        CAMERA_TYPE_ORTHOGRAPHIC = 0, /*!< Utilisé pour une caméra orthographique */
        CAMERA_TYPE_PERSPECTIVE = 1   /*!< Utilisé pour une caméra utilisant la perspective */
    };

    /*!
    * \class Camera
    * \brief Objet gérant les paramètres pour la "vue" lors d'un rendu
    */
    class Camera
    {
    public:
        /*!
        * \brief Constructeur
        *
        * Lors de la création de la caméra, un Transform sera créé automatiquement.
        * Ce transform contiendra les informations sur la position de la caméra. Il pourra être changé par la suite.
        */
        Camera(void);

        /*!
        * \brief Destructeur
        */
        virtual ~Camera(void);

        /*!
        * \return Le type de la caméra.
        */
        CameraType getType(void) const { return mType; }

        /*!
        * \brief Retourne l'ouverture verticale de la caméra
        * \return Le fov de la caméra (en radians)
        *
        * Le Fov est l'angle "d'ouverture" vertical de la caméra.
        * Il est exprimé par défaut en radians
        */
        float getFov(void) const { return mFov; }

        /*!
        * \brief Retourne la distance maximale d'affichage de la caméra
        *
        * Tout objet étant plus éloigné de cette valeur avec la caméra ne sera pas déssiné.
        * \return Distance maximale d'affichage de la caméra
        */
        float getNear(void) const { return mZNear; }

        /*!
        * \brief Retourne la distance minimale d'affichage de la caméra
        *
        * Tout objet étant plus proche de la caméra que cette valeur ne sera pas déssiné.
        * \return Distance minimale d'affichage de la caméra
        */
        float getFar(void) const { return mZFar; }

        /*!
        * \brief Retourne la résolution de la caméra sur l'axe X. (Horizontale)
        * \return Résolution horizontale de la caméra
        */
        unsigned int getXRes(void) const { return mXres; }

        /*!
        * \brief Retourne la résolution de la caméra sur l'axe Y. (Verticale)
        * \return Résolution verticale de la caméra.
        */
        unsigned int getYRes(void) const { return mYres; }

        /*!
        * \brief Retourne le Transform gérant la position de la caméra.
        *
        * Le transform est retourné sous forme de pointeur partagé. (Il s'utilise comme un pointeur classique).
        * \return Transform contenant les informations sur les transformations de la caméra.
        */
        Transform_sptr getTransform(void) const { return mTransform; }

        /*!
        * \brief Retourne la matrice de Projection.
        *
        * La matrice de projection contient les informations sur le ratio, l'angle de vue (Fov) et les distances d'affichage min et max de la caméra.
        * Une utilisation classique de cette matrice pour l'affichage d'un objet est la suivante : MatriceFinale = MatriceProjection * MatriceVue * MatriceModèle3D
        * \return Matrice de Projection.
        */
        glm::mat4 getProjectionMatrix(void) const;

        /*!
        * \brief Retourne la matrice de Vue.
        *
        * La matrice de vue contient les informations sur la position et l'orientation de la caméra
        * Une utilisation classique de cette matrice pour l'affichage d'un objet est la suivante : MatriceFinale = MatriceProjection * MatriceVue * MatriceModèle3D
        * \return Matrice de Vue.
        */
        glm::mat4 getViewMatrix(void) const;

        /*!
        * \brief Retourne de ratio de la caméra.
        *
        * Le ratio est donné par le rapport de RésolutionCaméraX / RésolutionCaméraY
        * \return Le ratio de la caméra.
        */
        float getRatio(void) const { return static_cast<float>(mXres)/static_cast<float>(mYres); }

        /*!
        * \brief Permet de définir le type de la caméra (Perspective ou Orthographique)
        *
        * Une caméra en mode perspective déformera les objets selon un angle de vue, de sorte à ce que les objets lointains paraissent plus petits.
        * Une caméra en mode orthographique réalisera une vue en perspective cavalière des objets.
        * \param type : Type de projection de la caméra.
        */
        void setType(CameraType type);

        /*!
        * \brief Permet de définir le champ de vue de la caméra (Fov)
        *
        * Ce paramètre n'est pas utilisé si la caméra est en mode Orthographique.
        * \param fov : Champ de vue de la caméra.
        */
        void setFov(float fov);

        /*!
        * \brief Permet de définir la distance minimale d'affichage de la caméra.
        *
        * Un fragment d'une distance avec la caméra inférieure à near ne sera pas affiché.
        * \param near : Distance minimale d'affichage de la caméra.
        */
        void setNear(float near);

        /*!
        * \brief Permet de définir la distance maximale d'affichage de la caméra.
        *
        * Un fragment d'une distance avec la caméra supérieure à far ne sera pas affiché.
        * \param far : Distance maximale d'affichage de la caméra.
        */
        void setFar(float far);

        /*!
        * \brief Permet de définir la résolution Horizontale de la caméra.
        *
        * Si la caméra est en mode Perspective seul le ratio est utilisé. Ce paramètre n'a donc pas une influence directe.
        * Si la caméra est en mode Orthographique, cette valeur corespondra à la largeur d'affichage de la caméra.
        *
        * \param xRes : Résolution Horizontale de la caméra
        */
        void setXRes(unsigned int xRes);

        /*!
        * \brief Permet de définir la résolution Verticale de la caméra.
        *
        * Si la caméra est en mode Perspective seul le ratio est utilisé. Ce paramètre n'a donc pas une influence directe.
        * Si la caméra est en mode Orthographique, cette valeur corespondra à la hauteur d'affichage de la caméra.
        *
        * \param yRes : Résolution Verticale de la caméra
        */
        void setYRes(unsigned int yRes);

         /*!
        * \brief Permet de définir le transform à utiliser pour la caméra
        *
        * Le transform est sous la forme d'un pointeur partagé. Il s'utilise comme un pointeur classique.
        * Il n'est pas conseillé de mettre un Pointeur Partagé ne contenant pas de pointeur valide.
        *
        * \param transform_sptr : Pointeur Partagé d'un objet Transform à utiliser pour la caméra
        */
        void setTransform(const Transform_sptr &transform_sptr);
    private:
        CameraType mType;
        float mFov, mZNear, mZFar;
        unsigned int mXres, mYres;
        Transform_sptr mTransform;
        glm::mat4 mView, mProjection;
    };
}
#endif // CAMERA_H
