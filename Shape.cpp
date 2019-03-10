#include "Shape.h"
#include <vector>
#include <iostream>
#include <exception>
#include <cmath>
#include "geException.h"
#include "glm/gtc/constants.hpp"


namespace GEII{
    Shape::Shape(GLenum drawMode) : mDrawMode(drawMode){ }

    Shape::Shape(const std::vector<float> &vertice, const std::vector<float> &textUV, const std::vector<float> &normals, const std::vector<unsigned int> &indices, GLenum drawMode)
        : mDrawMode(drawMode)
    {
        ENGINE_LOG_INFO("Creating Shape from buffers.");
        load(vertice, textUV, normals, indices);
        ENGINE_LOG_SUCCESS("Shape created from buffers.");
    }

    Shape::~Shape(void){
        // Buffer are destroyed (smart_ptr)
    }

    void Shape::load(const std::vector<float> &vertice, const std::vector<float> &textUV, const std::vector<float> &normals, const std::vector<unsigned int> &indices){
        size_t floatBytes = sizeof(float);
        unsigned int vertBytes = vertice.size()*floatBytes;
        unsigned int uvBytes = textUV.size()*floatBytes;
        unsigned int normalsBytes = normals.size()*floatBytes;
        unsigned int vboSize = vertBytes+uvBytes+3*normalsBytes;
        unsigned int iboSize = indices.size()*sizeof(unsigned int);

        std::vector<float> tangent(normals.size());
        std::vector<float> bitengent(normals.size());

        if(textUV.size()){
            unsigned int maxI = 0;
            for(unsigned int i(0); i < indices.size(); i++){
                if(indices[i] > maxI){
                    maxI = indices[i];
                }
            }
            std::vector<std::vector<glm::vec3>> tangentList(maxI+1);
            std::vector<std::vector<glm::vec3>> bitengentList(maxI+1);
            for(unsigned int i(0); i < indices.size(); i += 3){
                unsigned int index0 = indices[i], index1 = indices[i+1], index2 = indices[i+2];
                glm::vec3 P1 = glm::vec3(vertice[index0*3], vertice[index0*3+1], vertice[index0*3+2]) - glm::vec3(vertice[index1*3], vertice[index1*3+1], vertice[index1*3+2]);
                glm::vec3 P2 = glm::vec3(vertice[index1*3], vertice[index1*3+1], vertice[index1*3+2]) - glm::vec3(vertice[index2*3], vertice[index2*3+1], vertice[index2*3+2]);

              /* vec2 P1 = {3, 3};  // Delta P1
                Vec2 P2 = {-1, 2}; // Delta P2*/

                float X1 = textUV[index0*2] - textUV[index1*2];
               // float X1 = -0.75;  // Delta UV1.x
                float X2 = textUV[index1*2] - textUV[index2*2];
               // float X2 = -0.5;   // Delta UV2.x
                float Y1 = textUV[index0*2+1] - textUV[index1*2+1];
                //float Y1 = 0.75;   // Delta UV1.y
                float Y2 = textUV[index1*2+1] - textUV[index2*2+1];
               // float Y2 = -0.25;  // Delta UV2.y

                //Vec2 T = (P1-(Y1*P2/Y2))/(X1-Y1*X2/Y2);
                //Vec2 B = (P2-X2*T)/Y2;
                glm::vec3 T = (P1-(Y1*P2/Y2))/(X1-Y1*X2/Y2);
                glm::vec3 B = (P2-X2*T)/Y2;
               /* std::cout << "Triangle : " << i/3+1 << std::endl;
                std::cout << "T = " << geToString(T) << std::endl;
                std::cout << "B = " << geToString((P2-X2*T)/Y2) << std::endl;
                std::cerr << index0 << ", " << index1 << ", " << index2 << std::endl;*/
                tangentList[index0].push_back(T);           tangentList[index1].push_back(T);            tangentList[index2].push_back(T);
                bitengentList[index0].push_back(B);         bitengentList[index1].push_back(B);        bitengentList[index2].push_back(B);
            }


            // Moyenne des tangentes/ bitantgents par sommet
            for(unsigned int i(0); i < tangentList.size(); i++){
                glm::vec3 t = {0,0,0};
                glm::vec3 b = {0,0,0};
                for(unsigned int j(0); j < tangentList[i].size(); j++){
                    t += tangentList[i][j];
                    b += bitengentList[i][j];
                }
                t /= (float) tangentList[i].size();
                b /= (float) tangentList[i].size();

                tangent[i*3] = t.x;     tangent[i*3+1] = t.y;       tangent[i*3+2] = t.z;
                bitengent[i*3] = b.x;   bitengent[i*3+1] = b.y;     bitengent[i*3+2] = b.z;
            }
        }

        mVbo = std::unique_ptr<OGL_Buffer>(new OGL_Buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vboSize, nullptr));
        mIbo = std::unique_ptr<OGL_Buffer>(new OGL_Buffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, iboSize, indices.data()));

        mVerticeCount = vertice.size();
        mIndicesCount = indices.size();

        mVbo->bind();
            mVbo->sendSubData(vertice.data(), vertBytes, 0);
            mVbo->sendSubData(normals.data(), normalsBytes, vertBytes);
            mVbo->sendSubData(tangent.data(), normalsBytes, vertBytes+normalsBytes);
            mVbo->sendSubData(bitengent.data(),normalsBytes,vertBytes+normalsBytes*2);
            mVbo->sendSubData(textUV.data(), uvBytes, vertBytes+normalsBytes*3);
        mVbo->unbind();
    }

    void Shape::draw(void){
        mVbo->bind();
            glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
            glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,((void*)NULL+sizeof(float)*mVerticeCount));
            glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,0,((void*)NULL+(sizeof(float)*mVerticeCount*4)));
            glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,0,((void*)NULL+sizeof(float)*mVerticeCount*2));
            glVertexAttribPointer(4,3,GL_FLOAT,GL_FALSE,0,((void*)NULL+(sizeof(float)*mVerticeCount*3)));
    //Selectionne le buffer d'index
            mIbo->bind();
    //Active les attributs pour les shaders
                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
                glEnableVertexAttribArray(2);
                glEnableVertexAttribArray(3);
                glEnableVertexAttribArray(4);
    //Affiche tous les elements du buffer
                glDrawElements(mDrawMode, mIndicesCount , GL_UNSIGNED_INT, 0);
    //Desactive les attibuts (pour utiliser autre chose que les shaders, par exemple le mode immediat).
                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);
                glDisableVertexAttribArray(2);
                glDisableVertexAttribArray(3);
                glDisableVertexAttribArray(4);
            mIbo->unbind();
        mVbo->unbind();
    }
}
