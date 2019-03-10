#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

/*!
* \file Obj_Loader.h
* \brief Lecture des fichiers .obj
* \author Alexandre Brunet
* \date 23/01/2019
*/

#include "geHeader.h"
#include "Mtl_Loader.h"

#define EPS6 1E-6f

namespace GEII
{
    /*!
    * \class Obj_Loader
    * \brief Classe gérant la lecture des fichiers .obj
    *
    * Les meshs lus sont stockés dans des objets Shape accessibles via getShapes()\n
    * Les Shapes sont gardées en mémoire jusqu'au chargement d'un nouveau fichier via loadFile() ou un appel à clearData().\n
    */
    class Obj_Loader
    {
    public:
        /*!
        * \brief Constructeur\n
        * Constructeur par défaut : crée un objet vide sans charger de fichier.
        */
        Obj_Loader(void);

        /*!
        * \brief Constructeur\n
        * Charge les données formes et les matériaux contenues dans le fichier.\n
        * Les données chargées sont regroupées sous des objets Objet accesibles via getObjets().
        * Il est possible d'accéder aux objets Shape et Material en passant par ces objets.
        * Si le pointeur scene n'est pas nul (nullptr ou 0), les objets seront ajoutés à la Scene.\n
        * Lance une exception si il y à une erreur lors de la lecture du fichier.\n
        * \param file : nom du fichier .obj à lire (il faut aussi inclure le chemin du fichier).
        * \param scene : pointeur sur un objet Scene. Si non nul, les objets seront automatiquement ajoutés à cette scene.
        */
        Obj_Loader(const std::string &file, Scene* scene = nullptr);

        /*!
        * \brief Destructeur
        */
        virtual ~Obj_Loader(void);

        /*!
        * \brief Chargement d'un fichier .obj\n
        * Charge les données formes et les matériaux contenues dans le fichier.\n
        * Les données chargées sont regroupées sous des objets Objet accesibles via getObjets().
        * Il est possible d'accéder aux objets Shape et Material en passant par ces objets.
        * Si le pointeur scene n'est pas nul (nullptr ou 0), les objets seront ajoutés à la Scene.\n
        * Lance une exception si il y à une erreur lors de la lecture du fichier.\n
        * \param file : nom du fichier .obj à lire (il faut aussi inclure le chemin du fichier).
        * \param scene : pointeur sur un objet Scene. Si non nul, les objets seront automatiquement ajoutés à cette scene.
        */
        void loadFile(const std::string &file, Scene* scene = nullptr);

        /*!
        * \brief Efface les données\n
        */
        void clearData(void);

        /*!
        * \brief Retourne les objets chargés lors de la dernière lecture.\n
        * \return Un tableau contenant les objets chargées lors de la dernière lecture.
        */
        const std::vector<Object_sptr>& getObjects(void) const { return mObjets; }

    private:
        void cmd_f(std::stringstream &stream);
        bool isConvexPolygon(unsigned int *array_p, unsigned int arraySize);
        Shape_sptr createGeometricPrimitive(void);
        void generateNormals(void);

        std::vector<glm::vec3> mVertice;
        std::vector<glm::vec3> mNormals;
        std::vector<glm::vec2> mUVs;
        std::vector<glm::ivec3> mFaces;
        Mtl_Loader mMtlLoader;
        Material_sptr mCurrentLib;
        bool mComputeNormals;
        std::vector<Object_sptr> mObjets;
    };
}
#endif // OBJ_LOADER_H
