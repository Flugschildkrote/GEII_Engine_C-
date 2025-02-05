#include "Obj_Loader.h"

#include "Mtl_Loader.h"
#include "Scene.h"
#include "Object.h"
#include "Shape.h"

#ifndef OBJ_LOADER_DEBUG
    #if OBJ_LOADER_DEBUG_ENABLE == 1 && GEII_DEBUG == 1
        #define OBJ_LOADER_DEBUG(x) x
        #include "Log.h"
    #else
        #define OBJ_LOADER_DEBUG(x)
    #endif // OBJ_LOADER_DEBUG_ENABLE
#endif // OBJ_LOADER_DEBUG

namespace GEII
{
    OBJ_LOADER_DEBUG(static Logger Obj_Loader_Logger("Obj_Loader"));

    Obj_Loader::Obj_Loader(void) : mCurrentLib(nullptr), mComputeNormals(false){ }

    Obj_Loader::Obj_Loader(const std::string &file, Scene* scene) : mCurrentLib(nullptr), mComputeNormals(false){
        loadFile(file, scene);
    }

    Obj_Loader::~Obj_Loader(void){ }

    void Obj_Loader::clearData(void){
        mMtlLoader.clearData();
        mCurrentLib.reset();
        mVertice.clear();
        mNormals.clear();
        mUVs.clear();
        mFaces.clear();
        mComputeNormals = false;
        mObjets.clear();
    }

    void Obj_Loader::loadFile(const std::string &file, Scene *scene)
    {
        std::ifstream inputStream(file);
        if(!inputStream.is_open()){
            OBJ_LOADER_DEBUG(Obj_Loader_Logger.Error("Failed to open \""+file+"\""));
            throw std::runtime_error("Failed to open \""+file+"\"");
        }

        clearData();
        OBJ_LOADER_DEBUG(Obj_Loader_Logger.Info("Loading obj \"", file, "\"."));
        std::stringstream stream;
        stream << inputStream.rdbuf();
        inputStream.close();


        std::string cmd, buffer;
        unsigned int v=0, vt=0, vn=0, f=0;
        // Donner une taille maximum aux tableaux
        stream >> cmd;
        std::getline(stream, buffer);
        bool faceFlag = false;
        while(!stream.eof()){
            while(cmd == "f"){
                f ++;
                faceFlag = true;
                stream >> cmd;
                std::getline(stream, buffer);
                if(stream.eof())
                    break;
            }
            if(faceFlag){
                faceFlag = false;
            }

            if(cmd == "v"){
                v++;
            }else if(cmd == "vn"){
                vn++;
            }else if(cmd == "vt"){
                vt++;
            }
            stream >> cmd;
            std::getline(stream, buffer);
        }
        mVertice.reserve(v);
        mNormals.reserve(vn);
        mUVs.reserve(vt);
        mFaces.reserve(f*5);

        stream.clear();
        stream.seekg(0);

        glm::vec3 tmpV3f;
        glm::vec2 tmpV2f;
        stream >> cmd;
        std::string shapeName = "";
        bool shapeFlag = false;
        while(!stream.eof()){
            while(cmd == "f"){
                cmd_f(stream);
                stream >> cmd;
                shapeFlag = true;
                if(stream.eof())
                    break;
            }

            if(shapeFlag){
                Shape_sptr shape = createGeometricPrimitive();
                Object_sptr object = std::make_shared<Object>(shape, mCurrentLib);
                OBJ_LOADER_DEBUG(Obj_Loader_Logger.Success("Shape \"", shapeName, "\" loaded."));
                shapeName = "";
                mObjets.push_back(object);
                if(scene != nullptr){
                    scene->addObject(object);
                }
                mFaces.clear();
                shapeFlag = false;
            }

            if(cmd == "v"){
                stream >> tmpV3f.x >> tmpV3f.y >> tmpV3f.z;
                mVertice.push_back(tmpV3f);
            }else if(cmd == "vn"){
                stream >> tmpV3f.x >> tmpV3f.y >> tmpV3f.z;
                mNormals.push_back(tmpV3f);
            }else if(cmd == "vt"){
                stream >> tmpV2f.x >> tmpV2f.y;
                mUVs.push_back(tmpV2f);
            }else if(cmd == "usemtl"){//Une selection de materiaux
                std::string matName;
                stream >> matName;
                unsigned int i;
                for(i=0; i < mMtlLoader.getMtlLib().size(); i++){
                    if(mMtlLoader.getMtlLib()[i].materialName == matName){
                        mCurrentLib = mMtlLoader.getMtlLib()[i].material;
                        break;
                    }
                }
                if(i == mMtlLoader.getMtlLib().size()){
                    throw std::runtime_error("Failed to find material \""+matName+"\" in \""+mMtlLoader.getFileName()+"\"");
                }
            }else if(cmd == "mtllib"){//Un fichier pour les materiaux
                std::string materialPath;
                stream >> materialPath;
                std::string fileName = geGetFileRep(file)+materialPath;
                mMtlLoader.loadLib(fileName);
            }else if(cmd == "o"){
                stream >> shapeName;
            }else{
                std::getline(stream, buffer);
            }
            stream >> cmd;
         }
        OBJ_LOADER_DEBUG(Obj_Loader_Logger.Success("\"", file, "\" loaded."));
    }

    void Obj_Loader::cmd_f(std::stringstream &stream){    //Lecture des valeurs
        std::string line;
        std::getline(stream, line);

        // On enlève les premiers et derniers espaces
        line = line.substr(line.find_first_not_of(" "));
        line = line.substr(0, line.find_last_not_of(" ")+1);
     ///   std::cout << line;

        unsigned int v[16], vt[16], vn[16];
        unsigned int vCount=0, vtCount=0, vnCount=0;

        while(line.size() != 0){
            // traitement
            const char* line_cstr = line.c_str();
            if(sscanf(line_cstr, "%u/%u/%u", v+vCount, vt+vtCount, vn+vnCount) == 3){//Vertex/Texture/Normal
                vCount++;
                vtCount++;
                vnCount++;
            }else if(sscanf(line_cstr, "%u//%u", v+vCount, vn+vnCount) == 2){//Vertex//Normal
                vt[vCount] = 0;
                vCount++;
                vnCount++;
            }else if(sscanf(line_cstr, "%u/%u", v+vCount, vt+vtCount) == 2){//Vertex/Texture)
                vn[vCount] = 0;
                vCount++;
                vtCount++;
            }else if(sscanf(line_cstr, "%u", v+vCount) == 1){//Vertex
                vn[vCount] = 0;
                vt[vCount] = 0;
                vCount++;
            }

               // on enlève le premier vertex pour traiter le prochain vertex de la face
            bool spaceFound = false;
            for(unsigned int i(0); i<line.size(); i++){
                if(line[i] == ' '){
                    spaceFound = true;
                }else if(spaceFound){
                    line = line.substr(i);
                    break;
                }
            }
            if(!spaceFound){
                line.clear();
            }
        }

        // On a finit de stocker les données de la face
        if(vCount==3){ //Un triangle
            //Ajout a la liste
            glm::ivec3 face;
            face.x=v[0];    face.y=vt[0];   face.z=vn[0];
            mFaces.push_back(face);
            face.x=v[1];    face.y=vt[1];   face.z=vn[1];
            mFaces.push_back(face);
            face.x=v[2];    face.y=vt[2];   face.z=vn[2];
            mFaces.push_back(face);
           /// std::cout << " : Tris";
        }else if(vCount==4) { //Un carre
            //Ajout a la liste
            glm::ivec3 face;
            // Triangle 1
            face.x=v[0];    face.y=vt[0];   face.z=vn[0];
            mFaces.push_back(face);
            face.x=v[1];    face.y=vt[1];   face.z=vn[1];
            mFaces.push_back(face);
            face.x=v[2];    face.y=vt[2];   face.z=vn[2];
            mFaces.push_back(face);
            // Triangle 2
            face.x=v[0];    face.y=vt[0];   face.z=vn[0];
            mFaces.push_back(face);
            face.x=v[2];    face.y=vt[2];   face.z=vn[2];
            mFaces.push_back(face);
            face.x=v[3];    face.y=vt[3];   face.z=vn[3];
            mFaces.push_back(face);
           /// std::cout << " : Square";
        }else{
           /// std::cout << " : Poly " << vCount;
            //Test si c'est un polygone convexe
            if(!isConvexPolygon(v,vCount)){
              ///  std::cout << " Concave" << std::endl;
                throw std::runtime_error("Obj_Loader:cmd_f : failed to process face : vertice < 4 or polygon is not convex.");
            }

            glm::ivec3 face;
            for(unsigned int i=0; i<(vCount-2); i++)
            {
                face.x=v[0];    face.y=vt[0];   face.z=vn[0];
                mFaces.push_back(face);
                face.x=v[i+1];    face.y=vt[i+1];   face.z=vn[i+1];
                mFaces.push_back(face);
                face.x=v[i+2];    face.y=vt[i+2];   face.z=vn[i+2];
                mFaces.push_back(face);
            }
          ///  std::cout << " : Convex";
        }
      ///  std::cout << std::endl;
    }

    bool Obj_Loader::isConvexPolygon(unsigned int *array_p, unsigned int arraySize){
        if(arraySize<4)
        {
            throw std::runtime_error("Obj_Loader:isConvexPolygon : arraySize < 4 not handled");
            return 0;
        }
        //Calcul la 1er normale
        glm::vec3 pt0,pt1,pt2;
        glm::vec3 v0,v1,vn1,vn2;
        pt0 = mVertice[array_p[arraySize-1]-1];
        pt1 = mVertice[array_p[0]-1];
        pt2 = mVertice[array_p[1]-1];
        v0 = pt0-pt1;
        v1 = pt2-pt1;
        vn1 = glm::cross(v0,v1);
        glm::normalize(vn1);

        //Pour toutes les autres configurations
        for(unsigned int i=1; i<arraySize; i++){
            pt0 = mVertice[array_p[i-1]-1];
            pt1 = mVertice[array_p[i]-1];
            pt2 = mVertice[array_p[(i+1)%arraySize]-1];
            v0 = pt0 - pt1;
            v1 = pt2 - pt1;
            vn2 = glm::cross(v0,v1);
            glm::normalize(vn2);
            if((vn1.x-vn2.x)>EPS6 || (vn1.y-vn2.y)>EPS6 || (vn1.z-vn2.z)>EPS6)
                return false;
        }
        return true;
    }

    Shape_sptr Obj_Loader::createGeometricPrimitive(void){
        if(mNormals.size() == 0 || mComputeNormals){
            mComputeNormals = true;
          ///  std::cout << "Generating normals : " << std::endl;
            generateNormals();
        }

        if(mUVs.size() == 0){
            glm::vec2 uv(0.0f, 0.0f);
            mUVs.push_back(uv);
        }

      ///  std::cout << "Number of Vertex : " << mFaces.size() << std::endl;
        unsigned int sizedata= mFaces.size();
        unsigned int size=0;
        unsigned int i,j,k;
        int idxV, idxVn, idxVt, idx;
        //Allocation de tableaux pour stocker les vertices, normales, UV
        std::vector<float> vertArray(sizedata*3);
        std::vector<float> normalsArray(sizedata*3);
        std::vector<float> UvsArray(sizedata*2);
        //Allocation de la table des index
        std::vector<unsigned int> indexArray(sizedata);
        //Pour toutes les combinaisons V/Vt/Vn
        //- Cherche dans la table de LUT si elle existe
        //  ->Oui: renseigne la table des indices
        //  ->Non: ajoute la combinaison aux tableaux vertices, normales, UV et renseigne la table des indices
        int bidx;
        for(i=0; i<sizedata; i++){
            idxV= mFaces[i].x;
            idxVt= mFaces[i].y;
            idxVn= mFaces[i].z;
            bidx=0;
            for(j=i; j>0; j--){
                k=j-1;
                if(idxV==mFaces[k].x && idxVt==mFaces[k].y && idxVn==mFaces[k].z){
                    bidx=1;
                    idx=indexArray[k];
                    break;
                }
            }
            if(bidx) //On a deja les donnees->indique juste l'indice
                indexArray[i]=idx;
            else    //On n'a pas les donnees->ajoute aux tableaux
            {
                idxV--;
                vertArray[size*3]=  mVertice[idxV].x;
                vertArray[size*3+1]=mVertice[idxV].y;
                vertArray[size*3+2]=mVertice[idxV].z;

                idxVn--;
                normalsArray[size*3]=  mNormals[idxVn].x;
                normalsArray[size*3+1]=mNormals[idxVn].y;
                normalsArray[size*3+2]=mNormals[idxVn].z;

                idxVt--;
                UvsArray[size*2]=  mUVs[idxVt].x;
                UvsArray[size*2+1]=mUVs[idxVt].y;

                indexArray[i]=size;
                size++;
            }
        }

        Shape_sptr shape = std::make_shared<Shape>(vertArray, UvsArray, normalsArray, indexArray, GL_TRIANGLES);
        return shape;
    }

    void Obj_Loader::generateNormals(void){
        //Pour chaque face, on genere une normale
        unsigned int facesCount =mFaces.size()/3;
       /// std::cout << facesCount << std::endl;
        mNormals.reserve(facesCount);
        glm::vec3 v1,v2,vn;
        glm::vec3 p1,p2,p3;
        for(unsigned int i=0; i<facesCount; i++)
        {
            p1 = mVertice.at(mFaces[i*3].x-1);
            p2 = mVertice[mFaces[i*3+1].x-1];
            p3 = mVertice[mFaces[i*3+2].x-1];

            v1 = p3-p1;
            v2 = p2-p1;
            vn = glm::normalize(glm::cross(v2,v1));
            //Ajout a la liste
            mNormals.push_back(vn);
            //Mettre a jour les indices
            mFaces[i*3].z=mNormals.size();
            mFaces[i*3+1].z=mNormals.size();
            mFaces[i*3+2].z=mNormals.size();
        }
    }
}
