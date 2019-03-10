#include "Cylinder.h"

namespace GEII
{
    Cylinder::Cylinder(unsigned int edgesCount, float radius, float height) : Shape(GL_TRIANGLES), mRadius(radius), mHeight(height), mEdgesCount(edgesCount)
    {
        //Nombre de vertex
        unsigned int size=edgesCount*6+4;
        //Allocation de tableaux pour stocker les vertices, normales, UV
        std::vector<float> verticeArray, normalsArray, uvArray;
        std::vector<unsigned int> indiceArray;

        verticeArray.resize(size*3);
        normalsArray.resize(size*3);
        uvArray.resize(size*2);
        indiceArray.resize(edgesCount*4*3);
        //vertice; normale; UV
        unsigned int i;
        double dt = 2.0f*PI/(double)(edgesCount);
        float phi;
        float cphi,sphi;
        unsigned int iVN=0, iUV=0;
        unsigned int os1=edgesCount*2+1;
        unsigned int os2=os1*2;
        float h2=height/2.0f;
        float V1=radius/(2.0f*radius+height);
        float V2=(radius+height)/(2.0f*radius+height);
        float Ustep=1.0f/(float)(edgesCount),U;
        float U12=Ustep*0.5f;

        for(i=0; i<(edgesCount+1); i++){
            phi=dt*i;
            cphi=cos(phi); sphi=sin(phi);
            U=Ustep*(float)(i);
            //UP
            verticeArray[iVN]=cphi*radius;          verticeArray[iVN+1]=h2;           verticeArray[iVN+2]=sphi*radius;
            normalsArray[iVN]=0.0f;                 normalsArray[iVN+1]=1.0f;         normalsArray[iVN+2]=0.0f;
            uvArray[iUV]=U;                         uvArray[iUV+1]=V1;
            //DOWN
            verticeArray[iVN+os1*3]=cphi*radius;    verticeArray[iVN+os1*3+1]=-h2;    verticeArray[iVN+os1*3+2]=sphi*radius;
            normalsArray[iVN+os1*3]=0.0f;           normalsArray[iVN+os1*3+1]=-1.0f;  normalsArray[iVN+os1*3+2]=0.0f;
            uvArray[iUV+os1*2]=U;                   uvArray[iUV+os1*2+1]=V2;
            //SIDE
            verticeArray[iVN+os2*3]=cphi*radius;    verticeArray[iVN+os2*3+1]=h2;     verticeArray[iVN+os2*3+2]=sphi*radius;
            normalsArray[iVN+os2*3]=cphi;           normalsArray[iVN+os2*3+1]=0.0f;   normalsArray[iVN+os2*3+2]=sphi;
            uvArray[iUV+os2*2]=U;                   uvArray[iUV+os2*2+1]=V1;
            verticeArray[iVN+os2*3+3]=cphi*radius;  verticeArray[iVN+os2*3+4]=-h2;    verticeArray[iVN+os2*3+5]=sphi*radius;
            normalsArray[iVN+os2*3+3]=cphi;         normalsArray[iVN+os2*3+4]=0.0f;   normalsArray[iVN+os2*3+5]=sphi;
            uvArray[iUV+os2*2+2]=U;                 uvArray[iUV+os2*2+3]=V2;
            //CENTER UP & DOWN
            if(i!=edgesCount)
            {
                verticeArray[iVN+3]=0.0f;         verticeArray[iVN+4]=h2;           verticeArray[iVN+5]=0.0f;
                normalsArray[iVN+3]=0.0f;         normalsArray[iVN+4]=1.0f;         normalsArray[iVN+5]=0.0f;
                uvArray[iUV+2]=U12+U;          uvArray[iUV+3]=0.0f;

                verticeArray[iVN+os1*3+3]=0.0f;   verticeArray[iVN+os1*3+4]=-h2;    verticeArray[iVN+os1*3+5]=0.0f;
                normalsArray[iVN+os1*3+3]=0.0f;   normalsArray[iVN+os1*3+4]=-1.0f;  normalsArray[iVN+os1*3+5]=0.0f;
                uvArray[iUV+os1*2+2]=U12+U;          uvArray[iUV+os1*2+3]=1.0f;
            }
            iVN+=6;
            iUV+=4;
        }
        //Nombre d'indice
        unsigned int iI=0;
        for(i=0; i<edgesCount; i++){
            //UP
            indiceArray[iI]=i*2;          indiceArray[iI+1]=i*2+1;      indiceArray[iI+2]=i*2+2;
            //DOWN
            indiceArray[iI+3]=i*2+os1;    indiceArray[iI+4]=i*2+os1+1;  indiceArray[iI+5]=i*2+os1+2;
            //SIDE
            indiceArray[iI+6]=i*2+os2;    indiceArray[iI+7]=i*2+os2+1;  indiceArray[iI+8]=i*2+os2+2;
            indiceArray[iI+9]=i*2+os2+2;  indiceArray[iI+10]=i*2+os2+1; indiceArray[iI+11]=i*2+os2+3;

            iI+=12;
        }

        load(verticeArray, uvArray, normalsArray, indiceArray);
    }


    Cylinder::~Cylinder(void)
    {
        //dtor
    }
}
