#include "Sphere.h"

namespace GEII
{
    Sphere::Sphere(float radius, int vRingCount, int hRingCount) : Shape(GL_TRIANGLES), mRadius(radius), mVRingCount(hRingCount), mHRingCount(vRingCount)
    {
        unsigned int size=(hRingCount+1)*vRingCount;

        std::vector<float> verticeArray;
        std::vector<float> normalsArray;
        std::vector<float> uvArray;
        std::vector<unsigned int> indiceArray;

        //Allocation de tableaux pour stocker les vertices, normales, UV
        verticeArray.resize(size*3);
        normalsArray.resize(size*3);
        uvArray.resize(size*2);
        indiceArray.resize(hRingCount*(vRingCount-1)*6);

        float dp = PI/(float)(vRingCount-1);
        float dt = 2.0f*PI/(float)(hRingCount);
        //vertice; normale; UV
        float phi, theta;
        float cphi,sphi;
        float X,Y,Z;
        unsigned int indexVN=0, indexUV=0;
        for(int i=0; i<vRingCount; i++)
        {
            phi=i*dp;
            cphi=cos(phi); sphi=sin(phi);
            for(int j=0; j<=hRingCount; j++)
            {
                theta=j*dt;
                X=cos(theta)*sphi;
                Y=sin(theta)*sphi;
                Z=cphi;

                verticeArray[indexVN]=radius*X;
                normalsArray[indexVN++]=X;
                verticeArray[indexVN]=radius*Y;
                normalsArray[indexVN++]=Y;
                verticeArray[indexVN]=radius*Z;
                normalsArray[indexVN++]=Z;
                uvArray[indexUV++]=j/(double)(hRingCount);
                uvArray[indexUV++]=1.0f-i/(double)(vRingCount-1);
            }
        }

        //Pole nord et sud
        unsigned int indexI=0;
        unsigned int indexOffset=hRingCount*3;
        for(int i=0; i< hRingCount; i++){
            indiceArray[indexI]=i;
            indiceArray[indexI+indexOffset]=(hRingCount+1)*(vRingCount-2)+i+1;
            indexI++;
            indiceArray[indexI]=(hRingCount+1)+i;
            indiceArray[indexI+indexOffset]=(hRingCount+1)*(vRingCount-2)+i;
            indexI++;
            indiceArray[indexI]=(hRingCount+1)+i+1;
            indiceArray[indexI+indexOffset]=(hRingCount+1)*(vRingCount-1)+i;
            indexI++;
        }
        //Le reste
        indexOffset+=hRingCount*3;
        for(int j=1; j<vRingCount-1; j++){
            for(int i=0; i<hRingCount; i++){
                indiceArray[indexOffset+(i*6)] = (hRingCount+1)*j+i;
                indiceArray[indexOffset+(i*6)+1] = (hRingCount+1)*(j+1)+i;
                indiceArray[indexOffset+(i*6)+2] = (hRingCount+1)*(j+1)+i+1;

                indiceArray[indexOffset+(i*6)+3] = (hRingCount+1)*j+i;
                indiceArray[indexOffset+(i*6)+4] = (hRingCount+1)*(j+1)+i+1;
                indiceArray[indexOffset+(i*6)+5] = (hRingCount+1)*(j)+i+1;
            }
            indexOffset+=hRingCount*6;
        }
        load(verticeArray, uvArray, normalsArray, indiceArray);
    }

    Sphere::~Sphere(void){ }
}
