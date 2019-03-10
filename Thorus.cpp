#include "Thorus.h"
namespace GEII
{
    Torus::Torus(unsigned int sideCount, unsigned int slice, float mainRadius, float secondaryRadius)
        : Shape(GL_TRIANGLES), mSideCount(sideCount), mSlice(slice), mMainRadius(mainRadius), mSecondaryRadius(secondaryRadius)
    {
        unsigned int size=(slice+1)*(sideCount+1);
        if(secondaryRadius < 0)
            secondaryRadius = -secondaryRadius;
        if(mainRadius < 0)
            mainRadius = -mainRadius;
        //Allocation de tableaux pour stocker les vertices, normales, UV
        std::vector<float> verticeArray, normalsArray, uvArray;
        std::vector<unsigned int> indiceArray;

        verticeArray.resize(size*3);
        normalsArray.resize(size*3);
        uvArray.resize(size*2);
        indiceArray.resize(size*6);
        //vertice; normale; UV
        double dp = 2.0f*PI/(double)(slice);
        double dt = 2.0f*PI/(double)(sideCount);
        float phi, theta;
        float cphi,sphi;
        float ctheta,stheta,Rrctheta;
        float X,Y,Z;
        float Nx,Ny,Nz;
        unsigned int i,j;
        unsigned int indexVN=0, indexUV=0;

        for(i=0; i<=slice; i++){
            phi=(float)i*dp;
            cphi=cos(phi);sphi=sin(phi);
            for(j=0; j<=sideCount; j++)
            {
               theta=dt*(float)j;
               ctheta=cos(theta);
               stheta=sin(theta);
               Rrctheta=(mainRadius+secondaryRadius*ctheta);
               X = Rrctheta*cphi;
               Y = Rrctheta*sphi;
               Z = secondaryRadius*stheta;
               Nx = cphi*ctheta;
               Ny = -sphi*ctheta;
               Nz = stheta;
               verticeArray[indexVN]=X;
               normalsArray[indexVN++]=Nx;
               verticeArray[indexVN]=Y;
               normalsArray[indexVN++]=Ny;
               verticeArray[indexVN]=Z;
               normalsArray[indexVN++]=Nz;

               uvArray[indexUV++]=j/(float)(sideCount);
               uvArray[indexUV++]=i/(float)(slice);
            }
        }

        unsigned int indexI=0;
        for(i=0; i<slice; i++){
            for(j=0; j<sideCount; j++){
                //1er triangle
                indiceArray[indexI++]=i*(sideCount+1)+j;
                indiceArray[indexI++]=i*(sideCount+1)+j+1;
                indiceArray[indexI++]=(i+1)*(sideCount+1)+j+1;
                //2eme triangle
                indiceArray[indexI++]=i*(sideCount+1)+j;
                indiceArray[indexI++]=(i+1)*(sideCount+1)+j+1;
                indiceArray[indexI++]=(i+1)*(sideCount+1)+j;
            }
        }
        load(verticeArray, uvArray, normalsArray, indiceArray);
    }

    Torus::~Torus()
    {
        //dtor
    }
}
