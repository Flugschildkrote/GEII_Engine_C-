#include "Mtl_Loader.h"
#include "Material.h"
#include "Texture2D.h"

#ifndef MTL_LOADER_DEBUG
    #if MTL_LOADER_DEBUG_ENABLE == 1 && GEII_DEBUG == 1
        #define MTL_LOADER_DEBUG(x) x
        #include "Log.h"
    #else
        #define MTL_LOADER_DEBUG(x)
    #endif // MTL_LOADER_DEBUG_ENABLE
#endif // MTL_LOADER_DEBUG

namespace GEII
{
    MTL_LOADER_DEBUG(static Logger Mtl_Loader_Logger("Mtl_Loader"));

    Mtl_Loader::Mtl_Loader() :mFile(""), mMtlList(){

    }

    Mtl_Loader::Mtl_Loader(const std::string &fileName) :mFile(fileName), mMtlList()
    {
        loadLib(fileName);
    }


    Mtl_Loader::~Mtl_Loader()
    {
        //dtor
    }

    void Mtl_Loader::loadLib(const std::string &fileName){
        MTL_LOADER_DEBUG(Mtl_Loader_Logger.Info("Loading : \"", fileName, "\"."));

        clearData();

        std::string myFileRep = geGetFileRep(fileName); // On prend le répertoire de travail

        // On tente d'ouvrir le fichier
        std::ifstream stream(fileName);
        if(!stream.is_open()){
            MTL_LOADER_DEBUG(Mtl_Loader_Logger.Error("Failed to open \"", fileName, "\"."));
            throw std::runtime_error("Failed to open \""+ fileName+ "\".");
        }

        glm::vec3 matDiffuseColor, matSpecularColor;
        float matSpecularExponent, matOpacity;
        Texture_sptr matDiffuseTexture, matNormalMap;

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
                // Si un materiau à été lu, on le créé, et on commence à enregistrer le nouveau materiau
                // Puis on reset les paramètres
                if(materialID != 0){
                    MtlMaterial material;
                    material.material = std::make_shared<GEII::Material>(matDiffuseColor, matSpecularColor, matSpecularExponent, matOpacity, matDiffuseTexture, matNormalMap, true);
                    material.materialName = materialName;
                    mMtlList.push_back(material);

                    matDiffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
                    matSpecularColor = matDiffuseColor;
                    matSpecularExponent = 1.0f;
                    matOpacity = 1.0f;
                    matDiffuseTexture.reset();
                    matNormalMap.reset();
                }
                stream >> materialName;
                materialID++;
            }else if(instruction == "Ns"){
                stream >> matSpecularExponent;
            }else if(instruction == "Kd"){
                stream >> matDiffuseColor.x >> matDiffuseColor.y >> matDiffuseColor.z;
            }else if(instruction == "Ks"){
                stream >> matSpecularColor.x >> matSpecularColor.y >> matSpecularColor.z;
            }else if(instruction == "map_Kd"){
                std::string map_Kd;
                stream >> map_Kd;
                matDiffuseTexture = std::make_shared<Texture2D>(myFileRep+map_Kd, false);
               // std::cout << instruction << " = \"" << textureInfo.sourceFile << "\". SceneID = " << textSceneID << std::endl;
            }else if(instruction == "map_bump"){
                std::string map_Bump;
                stream >> map_Bump;
                matNormalMap = std::make_shared<Texture2D>(myFileRep+map_Bump, false);
               // std::cout << instruction << " = \"" << textureInfo.sourceFile << "\". SceneID = " << textSceneID << std::endl;
            }else if(instruction == "d"){
                stream >> matOpacity;
                //std::cout << instruction << " = " << constructInfo.transparency_Alpha << std::endl;
            }else{
                std::getline(stream, instruction);
            }
        }
        stream.close();

        // Crée le dernier materiau lu
        Material_sptr createdMaterial = std::make_shared<GEII::Material>(matDiffuseColor, matSpecularColor, matSpecularExponent, matOpacity, matDiffuseTexture, matNormalMap, true);
        MtlMaterial material;
        material.material = createdMaterial;
        material.materialName = materialName;
        mMtlList.push_back(material);

        MTL_LOADER_DEBUG
        (
            Mtl_Loader_Logger.Success("\"",fileName, "\" loaded.");
            std::string info = "Content of \"" + fileName + "\" :\n";
            std::vector<std::string> materialsStr;
            std::string materialStr = "";
            for(const MtlMaterial& mat : mMtlList){
                materialStr += "\t\"" + mat.materialName + "\" :\n";
                Material_sptr tmpMaterial = mat.material;
                materialStr += ("\t\tDiffuse Color : " + geToString(tmpMaterial->getDiffuseColor()) + "\n");
                materialStr += ("\t\tSpecular Color : " + geToString(tmpMaterial->getSpecularColor())+ "\n");
                materialStr += ("\t\tSpecular Exponent : " + std::to_string(tmpMaterial->getSpecularExponent())+ "\n");
                materialStr += ("\t\tOpacity : " + std::to_string(tmpMaterial->getOpacity())+ "\n");
                materialStr += ("\t\tIsLightSensitive : " + std::to_string(tmpMaterial->isLightSensitive())+ "\n");
                materialStr += ("\t\tDiffuse Texture : " + (tmpMaterial->getDiffuseTexture() ? tmpMaterial->getDiffuseTexture()->getName() :  "Non") + "\n");
                materialStr += ("\t\tNormal Map pointer : " + (tmpMaterial->getNormalMap() ? tmpMaterial->getNormalMap()->getName() : "Non") + "\n\n");
                materialsStr.push_back(materialStr);
            }
            std::string number = "Number of materials : " + std::to_string(mMtlList.size()) + "\n\n";
            Mtl_Loader_Logger.Info(info, materialStr, number);
        )
    }

    void Mtl_Loader::clearData(void){
        mFile.clear();
        mMtlList.clear();
    }

    const std::string Mtl_Loader::toString(void) const{
        std::string content = "Content of \"" + mFile + "\"\n--------------------------------------------\n";
        for(MtlMaterial material: mMtlList){
            content += ("Name = " + material.materialName + "\n");
        }
        return content+"\n";
    }

    const std::string Mtl_Loader::getFileName(void) const { return mFile; }
    std::vector<MtlMaterial> Mtl_Loader::getMtlLib(void) const{ return mMtlList; }
}
