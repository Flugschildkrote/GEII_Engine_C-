#include "Shape.h"
#include <vector>
#include <iostream>
#include <exception>
#include <cmath>
#include "geException.h"
#include "glm/gtc/constants.hpp"

Shape::Shape(const ShapeCreateInfo &createInfo) : mVbo(nullptr), mIbo(nullptr), mVerticeCount(0), mIndicesCount(0){
    mDrawMode = GL_TRIANGLES;
    try{
        switch(createInfo.sourceType){
        case ShapeSource::BOX :
            loadBox(createInfo.half_x, createInfo.half_y, createInfo.half_z);
            break;
        case ShapeSource::SPHERE :
            loadSphere(createInfo.stack, createInfo.slice, createInfo.radius);
            break;
        case ShapeSource::CYLINDER :
            loadCylinder(createInfo.slice, createInfo.radius, createInfo.height);
            break;
        case ShapeSource::CONE :
            loadCone(createInfo.slice, createInfo.radius, createInfo.height);
            break;
        case ShapeSource::TORUS :
            loadTorus(createInfo.stack,createInfo.slice, createInfo.radius, createInfo.internalRadius);
            break;
        case ShapeSource::FROM_BUFFER :
            if(createInfo.vertice == nullptr || createInfo.UVs == nullptr || createInfo.normals == nullptr || createInfo.indices == nullptr){
                throw geException("nullptr array.");
            }
            load(*createInfo.vertice, *createInfo.UVs, *createInfo.normals, *createInfo.indices);
            break;
        case ShapeSource::SQUARE :
            loadSquare(createInfo.half_x, createInfo.half_y);
            break;
        case ShapeSource::SKYBOX :
            loadSkybox(createInfo.half_x);
            break;
        default :
            break;
        }
    }catch(const geException &e){
        throw(e.extend("Failed to create Shape"));
    }
}

Shape::~Shape(void){
    // Buffer are destroyed (smart_ptr)
}

void Shape::loadBox(float width, float height, float depth){
    float x = (width > 0.0f ? width : -width);
    float y = (height > 0.0f ? height : -height);
    float z = (depth > 0.0f ? depth : -depth);

    std::vector<float> verticeList = {
        /* Face Avant*/   -x,-y, z,      x,-y, z,       x, y, z,          -x, y, z,
        /* Face Droite*/   x,-y, z,      x,-y,-z,       x, y,-z,           x, y, z,
        /* Face Arri�re*/  x,-y,-z,     -x,-y,-z,      -x, y,-z,           x, y,-z,
        /* Face Gauche*/  -x,-y,-z,     -x,-y, z,      -x, y, z,          -x, y,-z,
        /* Face Up*/      -x, y, z,      x, y, z,       x, y,-z,          -x, y,-z,
        /* Face Bas*/      x,-y, z,     -x,-y, z,      -x,-y,-z,           x,-y,-z
    };

    std::vector<float> normalsList = {
        /* Face Avant*/    0, 0, 1,      0, 0, 1,       0, 0, 1,           0, 0, 1,
        /* Face Droite*/   1, 0, 0,      1, 0, 0,       1, 0, 0,           1, 0, 0,
        /* Face Arri�re*/  0, 0,-1,      0, 0,-1,       0, 0,-1,           0, 0,-1,
        /* Face Gauche*/  -1, 0, 0,     -1, 0, 0,      -1, 0, 0,          -1, 0, 0,
        /* Face Up*/       0, 1, 0,      0, 1, 0,       0, 1, 0,           0, 1, 0,
        /* Face Bas*/      0,-1, 0,      0,-1, 0,       0,-1, 0,           0,-1, 0
    };

     std::vector<float> textureUVList = {
        /* Face Avant*/    0, 0,     1, 0,     1, 1,     0, 1,
        /* Face Droite*/   0, 0,     1, 0,     1, 1,     0, 1,
        /* Face Arri�re*/  0, 0,     1, 0,     1, 1,     0, 1,
        /* Face Gauche*/   0, 0,     1, 0,     1, 1,     0, 1,
        /* Face Up*/       0, 0,     1, 0,     1, 1,     0, 1,
        /* Face Bas*/      0, 0,     1, 0,     1, 1,     0, 1
    };


    std::vector<unsigned int> indicesList = {
        1, 0, 3,     3, 2, 1,
        5, 4, 7,     7, 6, 5,
        9, 8,11,    11,10, 9,
       13,12,15,    15,14,13,
       17,16,19,    19,18,17,
       21,20,23,    23,22,21,
    };

    /** Tangent / bitangent calculation*/
    {
       /* glm::vec3 P1 = glm::vec3(verticeList[1*3], verticeList[1*3+1], verticeList[1*3+2]) - glm::vec3(verticeList[0*3], verticeList[0*3+1], verticeList[0*3+2]);
        glm::vec3 P2 = glm::vec3(verticeList[0*3], verticeList[0*3+1], verticeList[0*3+2]) - glm::vec3(verticeList[3*3], verticeList[3*3+1], verticeList[3*3+2]);

      /* vec2 P1 = {3, 3};  // Delta P1
        Vec2 P2 = {-1, 2}; // Delta P2*/

       /* float X1 = textureUVList[1*2] - textureUVList[0*2];
       // float X1 = -0.75;  // Delta UV1.x
        float X2 = textureUVList[0*2] - textureUVList[3*2];
       // float X2 = -0.5;   // Delta UV2.x
        float Y1 = textureUVList[1*2+1] - textureUVList[0*2+1];
        //float Y1 = 0.75;   // Delta UV1.y
        float Y2 = textureUVList[0*2+1] - textureUVList[3*2+1];
       // float Y2 = -0.25;  // Delta UV2.y

        //Vec2 T = (P1-(Y1*P2/Y2))/(X1-Y1*X2/Y2);
        //Vec2 B = (P2-X2*T)/Y2;
        glm::vec3 T = (P1-(Y1*P2/Y2))/(X1-Y1*X2/Y2);
        glm::vec3 B = (P2-X2*T)/Y2;
        std::cout << "DeltaP1 = " << geToString(P1) << std::endl;
        std::cout << "T = " << geToString(T) << std::endl;
        std::cout << "B = " << geToString((P2-X2*T)/Y2) << std::endl;*/
    }

    return load(verticeList, textureUVList, normalsList, indicesList);
}

void Shape::loadSkybox(float halfSize){
    float s = halfSize;
    DEBUG_BLOCK(if(s <= 0){ std::cerr << "Warning : Shape:loadSkybox(), size should be > 0" << std::endl; });
    std::vector<float> verticeList = {
        /* Face Avant*/   -s,-s, s,      s,-s, s,       s, s, s,          -s, s, s,
        /* Face Droite*/   s,-s, s,      s,-s,-s,       s, s,-s,           s, s, s,
        /* Face Arri�re*/  s,-s,-s,     -s,-s,-s,      -s, s,-s,           s, s,-s,
        /* Face Gauche*/  -s,-s,-s,     -s,-s, s,      -s, s, s,          -s, s,-s,
        /* Face Up*/      -s, s, s,      s, s, s,       s, s,-s,          -s, s,-s,
        /* Face Bas*/      s,-s, s,     -s,-s, s,      -s,-s,-s,           s,-s,-s
    };

    std::vector<float> normalsList = {
        /* Face Avant*/    0, 0, 1,      0, 0, 1,       0, 0, 1,           0, 0, 1,
        /* Face Droite*/   1, 0, 0,      1, 0, 1,       1, 0, 1,           1, 0, 0,
        /* Face Arri�re*/  0, 0,-1,      0, 0,-1,       0, 0,-1,           0, 0,-1,
        /* Face Gauche*/  -1, 0, 0,     -1, 0, 0,      -1, 0, 0,          -1, 0, 0,
        /* Face Up*/       0, 1, 0,      0, 1, 0,       0, 1, 0,           0, 1, 0,
        /* Face Bas*/      0,-1, 0,      0,-1, 0,       0,-1, 0,           0,-1, 0
    };

    float bias = 0.001;
     std::vector<float> textureUVList = {
        /* Face Avant*/    1.0f,(1.0f/3.0f)+bias,   0.75f, (1.0f/3.0f)+bias,    0.75f,(2.0f/3.0f)-bias,     1.0f,(2.0f/3.0f)-bias,
        /* Face Droite*/   0.75f,(1.0f/3.0f)+bias,  0.5f,(1.0f/3.0f)+bias,      0.5f, (2.0f/3.0f)-bias,     0.75f, (2.0f/3.0f)-bias,
        /* Face Arri�re*/  0.5f,(1.0f/3.0f),        0.25f, (1.0f/3.0f),         0.25f,(2.0f/3.0f),          0.5f,(2.0f/3.0f),
        /* Face Gauche*/   0.25f,(1.0f/3.0f)+bias,  0.0f, (1.0f/3.0f)+bias,     0.0f, (2.0f/3.0f)-bias,     0.25f,(2.0f/3.0f)-bias,
        /* Face Up*/       0.25f+bias,1.0f,         0.5f-bias,1.0f,             0.5f-bias, (2.0f/3.0f),     0.25f+bias, (2.0f/3.0f),
        /* Face Bas*/      0.5f -bias,0.0f,         0.25f+bias, 0.0f,           0.25f+bias, (1.0f/3.0f),    0.5f-bias,(1.0f/3.0f),
    };

    std::vector<unsigned int> indicesList = {
        1, 0, 3,     3, 2, 1,
        5, 4, 7,     7, 6, 5,
        9, 8,11,    11,10, 9,
       13,12,15,    15,14,13,
       17,16,19,    19,18,17,
       21,20,23,    23,22,21,
    };

    return load(verticeList, textureUVList, normalsList, indicesList);
}


void Shape::loadSquare(float w, float h){
    float x = (w > 0) ? w : -w;
    float y = (h > 0) ? h : -h;
    constexpr float z = 0.0f;
    std::vector<float> verticeList = {      -x,-y,z,            x,-y,z,             x,y,z,              -x,y,z};
    std::vector<float> normalsList = {      0.0f,0.0f,1.0f,     0.0f,0.0f,1.0f,     0.0f,0.0f,1.0f,     0.0f,0.0f,1.0f };
    std::vector<float> textureUVList = {    0,0,                1,0,                1,1,                0,1 };

    glm::vec3 T = glm::vec3(w, 0.0f, 0.0f);
    glm::vec3 B = glm::vec3(0.0f, h, 0.0f);

    #ifdef CALCUL_TBN
        vec2 P1 = {3, 3};  // Delta P1
        Vec2 P2 = {-1, 2}; // Delta P2
        float X1 = -0.75;  // Delta UV1.x
        float X2 = -0.5;   // Delta UV2.x
        float Y1 = 0.75;   // Delta UV1.y
        float Y2 = -0.25;  // Delta UV2.y

        Vec2 T = (P1-(Y1*P2/Y2))/(X1-Y1*X2/Y2);
        Vec2 B = (P2-X2*T)/Y2;
        std::cout << "T = " << toString(T) << std::endl;
        std::cout << "B = " << toString((P2-X2*T)/Y2) << std::endl;
    #endif // CALCUL_TBN

    std::vector<unsigned int> indicesList = { 1,0,3,     3,2,1 };
    return load(verticeList, textureUVList, normalsList, indicesList);
}

void Shape::loadSphere(unsigned int ring, unsigned int slice, float radius){
    if(radius < 0)
        radius = -radius;

    //Nombre de vertex
    unsigned int size=(slice+1)*ring;

    std::vector<float> verticeArray;
    std::vector<float> normalsArray;
    std::vector<float> uvArray;
    std::vector<unsigned int> indiceArray;

    //Allocation de tableaux pour stocker les vertices, normales, UV
    verticeArray.resize(size*3);
    normalsArray.resize(size*3);
    uvArray.resize(size*2);
    indiceArray.resize(slice*(ring-1)*6);

    float dp = PI/(float)(ring-1);
    float dt = 2.0f*PI/(float)(slice);
    //vertice; normale; UV
    float phi, theta;
    float cphi,sphi;
    float X,Y,Z;
    unsigned int indexVN=0, indexUV=0;
    for(unsigned int i=0; i<ring; i++)
    {
        phi=i*dp;
        cphi=cos(phi); sphi=sin(phi);
        for(unsigned int j=0; j<=slice; j++)
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
            uvArray[indexUV++]=j/(double)(slice);
            uvArray[indexUV++]=1.0f-i/(double)(ring-1);
        }
    }

    //Pole nord et sud
    unsigned int indexI=0;
    unsigned int indexOffset=slice*3;
    for(unsigned int i=0; i< slice; i++){
        indiceArray[indexI]=i;
        indiceArray[indexI+indexOffset]=(slice+1)*(ring-2)+i+1;
        indexI++;
        indiceArray[indexI]=(slice+1)+i;
        indiceArray[indexI+indexOffset]=(slice+1)*(ring-2)+i;
        indexI++;
        indiceArray[indexI]=(slice+1)+i+1;
        indiceArray[indexI+indexOffset]=(slice+1)*(ring-1)+i;
        indexI++;
    }
    //Le reste
    indexOffset+=slice*3;
    for(unsigned int j=1; j<ring-1; j++){
        for(unsigned int i=0; i<slice; i++){
            indiceArray[indexOffset+(i*6)] = (slice+1)*j+i;
            indiceArray[indexOffset+(i*6)+1] = (slice+1)*(j+1)+i;
            indiceArray[indexOffset+(i*6)+2] = (slice+1)*(j+1)+i+1;

            indiceArray[indexOffset+(i*6)+3] = (slice+1)*j+i;
            indiceArray[indexOffset+(i*6)+4] = (slice+1)*(j+1)+i+1;
            indiceArray[indexOffset+(i*6)+5] = (slice+1)*(j)+i+1;
        }
        indexOffset+=slice*6;
    }

    load(verticeArray, uvArray, normalsArray, indiceArray);
}

void Shape::loadCylinder(unsigned int slice, float radius, float height){
    //Nombre de vertex
    unsigned int size=slice*6+4;
    //Allocation de tableaux pour stocker les vertices, normales, UV
    std::vector<float> verticeArray, normalsArray, uvArray;
    std::vector<unsigned int> indiceArray;

    verticeArray.resize(size*3);
    normalsArray.resize(size*3);
    uvArray.resize(size*2);
    indiceArray.resize(slice*4*3);
    //vertice; normale; UV
    unsigned int i;
    double dt = 2.0f*PI/(double)(slice);
    float phi;
    float cphi,sphi;
    unsigned int iVN=0, iUV=0;
    unsigned int os1=slice*2+1;
    unsigned int os2=os1*2;
    float h2=height/2.0f;
    float V1=radius/(2.0f*radius+height);
    float V2=(radius+height)/(2.0f*radius+height);
    float Ustep=1.0f/(float)(slice),U;
    float U12=Ustep*0.5f;

    for(i=0; i<(slice+1); i++){
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
        if(i!=slice)
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
    for(i=0; i<slice; i++){
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

void Shape::loadCone(unsigned int edgesCount, float radius, float height){
     //Nombre de vertex

    unsigned int size=edgesCount*4+2;
    //Allocation de tableaux pour stocker les vertices, normales, UV
    std::vector<float> verticeArray, normalsArray, uvArray;
    std::vector<unsigned int> indiceArray;

    verticeArray.resize(size*3);
    normalsArray.resize(size*3);
    uvArray.resize(size*2);
    indiceArray.resize(edgesCount*2*3);

    //vertice; normale; UV
    double dt = 2.0f*PI/(double)(edgesCount);
    float phi;
    float cphi,sphi;
    unsigned int i;
    unsigned int iVN=0, iUV=0;
    unsigned int os1=edgesCount*2+1;
    float h2=height/2.0f;
    float hn=height/sqrt(height*height+radius*radius);
    float rn=radius/sqrt(height*height+radius*radius);
    float V1=radius/(radius+height);
    float Ustep=1.0f/(float)(edgesCount),U;
    float U12=Ustep*0.5f;

    for(i=0; i<(edgesCount+1); i++){
        phi=dt*i;
        cphi=cos(phi); sphi=sin(phi);
        U=Ustep*(float)(i);
        //UP
        verticeArray[iVN]=cphi*radius;          verticeArray[iVN+1]=-h2;          verticeArray[iVN+2]=sphi*radius;
        normalsArray[iVN]=cphi*hn;              normalsArray[iVN+1]=rn;           normalsArray[iVN+2]=sphi*hn;
        uvArray[iUV]=U;                         uvArray[iUV+1]=V1;
        //DOWN
        verticeArray[iVN+os1*3]=cphi*radius;    verticeArray[iVN+os1*3+1]=-h2;    verticeArray[iVN+os1*3+2]=sphi*radius;
        normalsArray[iVN+os1*3]=0.0f;           normalsArray[iVN+os1*3+1]=-1.0f;  normalsArray[iVN+os1*3+2]=0.0f;
        uvArray[iUV+os1*2]=U;                   uvArray[iUV+os1*2+1]=V1;
        //CENTER UP & DOWN
        if(i!=edgesCount){
            verticeArray[iVN+3]=0.0f;           verticeArray[iVN+4]=h2;           verticeArray[iVN+5]=0.0f;
            normalsArray[iVN+3]=cphi*hn;        normalsArray[iVN+4]=rn;           normalsArray[iVN+5]=sphi*hn;
            uvArray[iUV+2]=U12+U;               uvArray[iUV+3]=0.0f;

            verticeArray[iVN+os1*3+3]=0.0f;     verticeArray[iVN+os1*3+4]=-h2;    verticeArray[iVN+os1*3+5]=0.0f;
            normalsArray[iVN+os1*3+3]=0.0f;     normalsArray[iVN+os1*3+4]=-1.0f;  normalsArray[iVN+os1*3+5]=0.0f;
            uvArray[iUV+os1*2+2]=U12+U;         uvArray[iUV+os1*2+3]=1.0f;
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
        iI+=6;
    }

    load(verticeArray, uvArray, normalsArray, indiceArray);
}

void Shape::loadTorus(unsigned int sideCount, unsigned int slice, float radius, float radiusBis){
    unsigned int size=(slice+1)*(sideCount+1);
    if(radius < 0)
        radius = -radius;
    if(radiusBis < 0)
        radiusBis = -radiusBis;
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
           Rrctheta=(radiusBis+radius*ctheta);
           X = Rrctheta*cphi;
           Y = Rrctheta*sphi;
           Z = radius*stheta;
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

void Shape::loadCamera(float fov , float ratio, float zNear, float zFar){
    // formes : un cube d�form� selon l'angle
    glm::vec3 nearPlane_LD, nearPlane_RD,nearPlane_LU, nearPlane_RU, farPlane_LD, farPlane_RD,farPlane_LU, farPlane_RU;

    float phi = fov/360.0f*glm::pi<float>(); // semi Angle vertical en radians
    float cosPhi = std::cos(phi); // ratio de l'angle
    float sinPhi = std::sin(phi); // ratio de l'angle

    // cos(rad)*factor = near -> factor = near/cos(rad);
    float factor = zNear/cosPhi;
    float y1 = sinPhi*factor;
    factor = zFar/cosPhi;
    float y2 = sinPhi*factor;
    float x1 = y1/ratio;
    float x2 = y2/ratio;

    // std::vector<float> verticeList = {
     //   /* Face Avant*/   -x1,-y1, zNear,    x1,-y1, zNear,       x1, y1, zNear,         -x1, y1, zNear,
     //   /* Face Droite*/   x1,-y1, zNear,    x2,-y2, zFar,       x1, y1, zNear,           x2, y2, zFar,
     //   /* Face Arri�re*/  x2,-y2,-zFar,     -x2,-y2,-zFar,      -x2, y2,-zFar,           x2, y2,-zFar,
     //   /* Face Gauche*/  -x1,-y1, zNear,   -x2,-y2, zFar,       -x1, y1, zNear,         -x2, y2, zFar,
    //    /* Face Up*/      -x, y, z,      x, y, z,       x, y,-z,          -x, y,-z,
     //   /* Face Bas*/      x,-y, z,     -x,-y, z,      -x,-y,-z,           x,-y,-z
   // };

     std::vector<float> verticeList = {
        /* Face Avant*/   -x1,-y1, -zNear,    x1,-y1, -zNear,       x1, y1, -zNear,         -x1, y1, -zNear,
        /* Face Arri�re*/ -x2,-y2, -zFar,     x2,-y2, -zFar,        x2, y2, -zFar,          -x2, y2, -zFar,
                        0.0f, 0.0f, 0.0f
     //   /* Face Gauche*/  -x1,-y1, zNear,   -x2,-y2, zFar,       -x1, y1, zNear,         -x2, y2, zFar,
    //    /* Face Up*/      -x, y, z,      x, y, z,       x, y,-z,          -x, y,-z,
     //   /* Face Bas*/      x,-y, z,     -x,-y, z,      -x,-y,-z,           x,-y,-z*/
    };

    std::vector<unsigned int> indice = {
        0,1,  1,2,  2,3,  3,0,
        4,5,  5,6,  6,7,  7,4,
        2,6,  3,7,  0,4,  1,5,
        8,0,  8,3,  8,2,  8,1
    };

    std::vector<float> normals, uvs;
    mDrawMode = GL_LINES;
    load(verticeList, uvs, normals, indice);
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
        int maxI = 0;
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
            std::cout << "Triangle : " << i/3+1 << std::endl;
            std::cout << "T = " << geToString(T) << std::endl;
            std::cout << "B = " << geToString((P2-X2*T)/Y2) << std::endl;
            std::cerr << index0 << ", " << index1 << ", " << index2 << std::endl;
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



    OGL_BufferCreateInfo createInfo = {};
    createInfo.data = nullptr;
    createInfo.sizeBytes = vboSize;
    createInfo.GL_target = GL_ARRAY_BUFFER;
    createInfo.GL_usage = GL_STATIC_DRAW;
    try{
        mVbo = OGL_Buffer::create_uptr(createInfo);

        createInfo.GL_target = GL_ELEMENT_ARRAY_BUFFER;
        createInfo.sizeBytes = iboSize;
        createInfo.data = indices.data();
        mIbo = OGL_Buffer::create_uptr(createInfo);
    }catch(const geException &e){
        if(createInfo.GL_target == GL_ARRAY_BUFFER){
            throw(e.extend("Failed to create Vertex Buffer"));
        }
        throw(e.extend("Failed to create Index Buffer"));
    }

    mVerticeCount = vertice.size();
    mIndicesCount = indices.size();

    mVbo->bind(true);
        mVbo->sendSubData(vertice.data(), vertBytes, 0, false);
        mVbo->sendSubData(normals.data(), normalsBytes, vertBytes, false);
        mVbo->sendSubData(tangent.data(), normalsBytes, vertBytes+normalsBytes, false);
        mVbo->sendSubData(bitengent.data(),normalsBytes,vertBytes+normalsBytes*2, false);
        mVbo->sendSubData(textUV.data(), uvBytes, vertBytes+normalsBytes*3, false);
    mVbo->bind(false);
}

void Shape::draw(void){
    mVbo->bind(true);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,((void*)NULL+sizeof(float)*mVerticeCount));
        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,0,((void*)NULL+(sizeof(float)*mVerticeCount*4)));
        glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,0,((void*)NULL+sizeof(float)*mVerticeCount*2));
        glVertexAttribPointer(4,3,GL_FLOAT,GL_FALSE,0,((void*)NULL+(sizeof(float)*mVerticeCount*3)));
//Selectionne le buffer d'index
        mIbo->bind(true);
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
        mIbo->bind(false);
    mVbo->bind(false);
}
