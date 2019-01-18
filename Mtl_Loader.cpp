#include "Mtl_Loader.h"
#include "Scene.h"
#include "geException.h"
#include "Material.h"
#include <fstream>


Mtl_Loader::Mtl_Loader() :mFile(""), mMtlList()
{
    //ctor
}

Mtl_Loader::Mtl_Loader(const std::string &fileName, Scene* scene) :mFile(fileName), mMtlList()
{
    loadLib(fileName, scene);
}


Mtl_Loader::~Mtl_Loader()
{
    //dtor
}

void Mtl_Loader::loadLib(const std::string &fileName, Scene* scene){
    clearData();
    if(scene == nullptr){
        throw geException("Mtl_Loader:loadLib : Invalid scene pointer.");
    }
   // mMtlLib = TMtlParser mtldata;
    //Fixe la liste des materiaux

    std::string myFileRep = geGetFileRep(fileName); // On prend le répertoire de travail
    // On tente d'ouvrir le fichier
    std::ifstream stream(fileName);
    if(!stream.is_open()){
        throw geException("Failed to open file \""+fileName+"\"");
    }

    MaterialCreateInfo constructInfo;
    std::string materialName;

    unsigned int materialID = 0;
    while(!stream.eof()){
        std::string instruction;
        stream >> instruction;

        if(instruction == "#"){
            std::string commentary;
            std::getline(stream, commentary);
            //std::cout << "Instruction : " << instruction << " -> Skipping \"" << commentary << "\"" << std::endl;
        }else if(instruction == "newmtl"){
            if(materialID != 0){
                 // Créer un nouveau materiau depuis la scene
                try{
                    unsigned int id;
                    MtlMaterial material;
                    material.material = scene->getNewMaterial(constructInfo, &id);
                    material.materialName = materialName;
                    mMtlList.push_back(material);
                }catch(const geException &e){
                    throw e.extend("MtlLoader : ");
                }
            }
            stream >> materialName;
            //std::cout  << "\n" << instruction << " -> Material name :  \"" << materialName << "\"" << std::endl;
            materialID++;
            constructInfo = { };
        }else if(instruction == "Ns"){
            stream >> constructInfo.specularExponent_Ns;
           // std::cout << instruction << " = " <<  constructInfo.specularExponent_Ns << std::endl;
        }else if(instruction == "Kd"){
            stream >> constructInfo.ambiantColor_Kd.x >> constructInfo.ambiantColor_Kd.y >> constructInfo.ambiantColor_Kd.z;
           // std::cout << instruction << " = " << geToString(constructInfo.ambiantColor_Kd) << std::endl;
        }else if(instruction == "Ks"){
            stream >> constructInfo.specularColor_Ks.x >> constructInfo.specularColor_Ks.y >> constructInfo.specularColor_Ks.z;
            //std::cout << instruction << " = " << geToString(constructInfo.specularColor_Ks) << std::endl;
        }else if(instruction == "map_Kd"){
            std::string map_Kd;
            stream >> map_Kd;
            OGL_TextureCreateInfo textureInfo = {};
            textureInfo.fromFile_nFromBuffer = true;
            textureInfo.sourceFile = myFileRep+map_Kd;
            textureInfo.magFilter = GL_LINEAR;

            unsigned int textSceneID;
            try{
                constructInfo.texture = scene->getNewTexture(textureInfo, &textSceneID);
            }catch(const geException &e){
                throw e.extend("Mtl_Loader:loadLib : Failed to add texture to scene");
            }
           // std::cout << instruction << " = \"" << textureInfo.sourceFile << "\". SceneID = " << textSceneID << std::endl;
        }else if(instruction == "map_Bump"){
            std::string map_Bump;
            stream >> map_Bump;
            OGL_TextureCreateInfo textureInfo = {};
            textureInfo.fromFile_nFromBuffer = true;
            textureInfo.sourceFile = myFileRep+map_Bump;
            textureInfo.magFilter = GL_LINEAR;

            unsigned int textSceneID;
            try{
                constructInfo.normalMap = scene->getNewTexture(textureInfo, &textSceneID);
            }catch(const geException &e){
                throw e.extend("Mtl_Loader:loadLib : Failed to add texture to scene");
            }
           // std::cout << instruction << " = \"" << textureInfo.sourceFile << "\". SceneID = " << textSceneID << std::endl;
        }else if(instruction == "d"){
            stream >> constructInfo.transparency_Alpha;
            //std::cout << instruction << " = " << constructInfo.transparency_Alpha << std::endl;
        }else{
            std::getline(stream, instruction);
        }
    }
    stream.close();

    try{
        unsigned int id;
        Material_sptr createdMaterial = scene->getNewMaterial(constructInfo, &id);
        MtlMaterial material;
        material.material = createdMaterial;
        material.materialName = materialName;
        mMtlList.push_back(material);
    }catch(const geException &e){
        throw e.extend("MtlLoader : ");
    }
}

void Mtl_Loader::clearData(void){
    mFile.clear();
    mMtlList.clear();
}

const std::string Mtl_Loader::toString(void) const{
    std::string content = "Content of \"" + mFile + "\"\n--------------------------------------------\n";
    for(MtlMaterial material: mMtlList){
        content += ("Name = " + material.materialName + "\n"+ material.material->toString()+"\n");
    }
    return content+"\n";
}

const std::string Mtl_Loader::getFile(void) const { return mFile; }
std::vector<MtlMaterial> Mtl_Loader::getMtlLib(void) const{ return mMtlList; }
