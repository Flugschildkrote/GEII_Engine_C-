#ifndef MATERIAL_H
#define MATERIAL_H

/*!
* \file Material.h
* \author Alexandre Brunet
* \date 25/02/2019
*/

#include "geHeader.h"

namespace GEII
{
    /*!
    * \class Material
    * \brief Gère les couleurs et les options d'éclairage des objets.
    */
    class Material
    {
    public:
        /*!
        * \brief Créé un materiau blanc.
        */
        Material(void);

        /*!
        * \brief Créé un materiau personnalisé.
        * \param diffuseColor : couleur du materiau à la lumière blanche
        * \param specularColor : courleur renvoyée par les reflets
        * \param specularExponent : plus ce maramètre est élevé, plus le matériau sera brillant
        * \param opacity : Opacité du matériau
        * \param diffuseTexture : Texture à afficher sur l'objet
        * \param normalMap : Texture de déformation des normales, permet d'augmenter le niveau de détail de l'objet
        * \param lightSensitive : Si l'objet est affecté par la lumière ou non. Si non, son intensité lors du rendu ne dépendra pas de des lumière mais de diffuseColor
        */
        Material(const glm::vec3 &diffuseColor, const glm::vec3 &specularColor, float specularExponent, float opacity,
                 const Texture_sptr& diffuseTexture, const Texture_sptr& normalMap, bool lightSensitive = true);

        /*!
        * \brief Créé un materiau de couleur blanche, avec une texture
        * \param diffuseTexture : Texture à afficher sur l'objet
        * \param lightSensitive : Si l'objet est affecté par la lumière ou non. Si non, son intensité lors du rendu ne dépendra pas de des lumière mais de sa couleur diffuse.
        */
        Material(const Texture_sptr& diffuseTexture, bool lightSensitive = true);

        virtual ~Material(void);

        /*!
        * \brief Indique si le materiau est transparent.
        * \return boleen : Materiau transparent ou non.
        */
        bool isTransparent(void) const;

        /*!
        * \brief Permet d'acceder à la couleur diffuse du materiau (couleur lorsqu'il est exposé ç une lumière blanche (Textures exclues)).
        * \return Couleur diffuse du materiau.
        */
        glm::vec3 getDiffuseColor(void) const { return mDiffuseColor; }

        /*!
        * \brief Permet d'acceder à la couleur spéculaire du materiau (couleur des reflets lorsqu'il est exposé ç une lumière blanche (Textures exclues)).
        * \return Couleur spéculaire du materiau.
        */
        glm::vec3 getSpecularColor(void) const { return mSpecularColor; }

        /*!
        * \brief Permet d'acceder à la "puissance" spéculaire.
        * \return "Puissance spéculaire" su materiau.
        */
        float getSpecularExponent(void) const { return mSpecularExponent; }

        /*!
        * \brief Permet d'acceder à l'opacité du materiau (Inverse de la transparence, compris entre 0 et 1).
        * \return Opacité du materiau.
        */
        float getOpacity(void) const { return mOpacity; }

        /*!
        * \brief Permet d'acceder à la sensibilité du materiau à la lumière. Un materiau insensible à la lumière ne sera pas affecté par celle ci.
        * \return Sensibilité du materiau à la lumière.
        */
        bool isLightSensitive(void) const { return mLightSensitive; }

        /*!
        * \brief Permet d'acceder à la texture diffuse du materiau (Texture affichée sur l'objet)
        * \return Texture diffuse du materiau.
        */
        Texture_sptr getDiffuseTexture(void) const { return mDiffuseTexture; }

        /*!
        * \brief Permet d'acceder à la texture de normales du materiau (Texture affichée sur l'objet)
        * \return Texture diffuse du materiau.
        */
        Texture_sptr getNormalMap(void) const { return mNormalMap; }

        /*!
        * \brief Définit la couleur diffuse du materiau.
        * \param diffuseColor : Couleur diffuse du materiau.
        */
        void setDiffuseColor(const glm::vec3& diffuseColor)  { mDiffuseColor = diffuseColor; }

        /*!
        * \brief Définit la couleur spéculaire du materiau.
        * \param specularColor : Couleur spéculaire du materiau.
        */
        void setSpecularColor(const glm::vec3& specularColor)  { mSpecularColor = specularColor; }

        /*!
        * \brief Définit la puissance spéculaire du materiau.
        * \param specularExponent : Puissance spéculaire du materiau.
        */
        void setSpecularExponent(float specularExponent) { mSpecularExponent = specularExponent; }

        /*!
        * \brief Définit l'opacité du materiau.
        * \param opacity : Opacité du materiau.
        */
        void setOpacity(float opacity) { mOpacity = opacity; }

        /*!
        * \brief Définit la sensibilité à la lumière du materiau.
        * \param lightSensitive : Sensibilité à la lumière du materiau.
        */
        void setLightSensitive(bool lightSensitive) { mLightSensitive = lightSensitive; }

        /*!
        * \brief Définit la texture diffuse du materiau.
        * \param texture : Texture diffuse du materiau.
        */
        void setDiffuseTexture(const Texture_sptr& texture) { mDiffuseTexture = texture; }

        /*!
        * \brief Définit la texture de normales du materiau.
        * \param normalMap : Texture de normales du materiau.
        */
        void setNormalMap(const Texture_sptr& normalMap) { mNormalMap = normalMap; }

    protected:
        glm::vec3 mDiffuseColor;
        glm::vec3 mSpecularColor;
        float mSpecularExponent;
        float mOpacity;
        bool mLightSensitive;
        Texture_sptr mDiffuseTexture;
        Texture_sptr mNormalMap;
    };
}

#endif // MATERIAL_H
