#ifndef MTL_LOADER_H
#define MTL_LOADER_H

/*!
* \file Mtl_Loader.h
* \brief Lecture des fichiers .mtl
* \author Alexandre Brunet
* \date 23/01/2019
*/

#include "geHeader.h"

namespace GEII
{
    /*!
    * \struct MtlMaterial
    * \brief Ensemble d'un materiau et son nom dans le fichier .obj
    */

    struct MtlMaterial{
        std::string materialName; /*!< nom du materiau dans le fichier .obj */
        Material_sptr material;   /*!< Shared pointer contenant le materiau */
    };

    /*!
    * \class Mtl_Loader
    * \brief Classe gérant la lecture des fichiers .mtl\n
    *
    * Les materiaux chargés sont gardés en mémoire jusqu'à effacement.\n
    * Les materiaux sont effacés lors du chargement d'un nouveau matériau ou d'un appel à clearData().
    */

    class Mtl_Loader
    {
    public:
        /*!
        * \brief Constructeur\n
        * Constructeur par défaut : crée un objet vide sans charger de fichier.
        */
        Mtl_Loader(void);

        /*!
        * \brief Constructeur\n
        * Constructeur prennant en paramètre le nom d'un fichier '.mtl'.\n
        * Lis le fichier et stocke les matériaux et leurs nom dans mMtlList.\n
        * Lance une exception si il y à une erreur lors de la lecture du fichier.\n
        * \param fileName : nom du fichier .mtl à lire (il faut aussi inclure le chemin du fichier).
        */
        Mtl_Loader(const std::string& fileName);

        /*!
        * \brief Destructeur
        */
        virtual ~Mtl_Loader(void);

        /*!
        * \brief Charge un fichier .mtl\n
        * Efface les matériaux déja présents, lis le fichier et stocke les matériaux et leurs nom dans mMtlList.\n
        * Lance une exception si il y à une erreur lors de la lecture du fichier.\n
        * \param fileName : nom du fichier .mtl à lire (il faut aussi inclure le chemin du fichier).
        */
        void loadLib(const std::string& fileName);

        /*!
        * \brief Efface les données présentes en mémoire (MtlMaterial chargés et nom du dernier fichier chargé).
        */
        void clearData(void);

        /*!
        * \brief Convertit en String\n
        * Représente les données stockées sous forme de chaine de caractères.
        * \return String contenant la représentation des données.
        */
        const std::string toString(void) const;

        /*!
        * \brief Nom du dernier fichier chargé\n
        * La chaine est vide si aucun fichier n'a été lu précédemment.
        * \return Nom du dernier fichier .mtl chargé.
        */
        const std::string getFileName(void) const;

        /*!
        * \brief Liste des materiaux chargés.
        * \return Liste des MtlMaterial en mémoire.
        */
        std::vector<MtlMaterial> getMtlLib(void) const;
    private:
        std::string mFile; /*!< Nom du dernier fichié chargé */
        std::vector<MtlMaterial> mMtlList; /*!< Liste des MtlMaterial chargés lors de la dernière lecture */
    };
}
#endif // MTL_LOADER_H
