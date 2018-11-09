#ifndef SHAPE_H
#define SHAPE_H

#include "geHeader.h"
#include <vector>
#include "OGL_Buffer.h"
#include <memory>

constexpr float PI = 3.14159265358979323846264338327950288f;

class Shape{
public:
    Shape(const ShapeCreateInfo &createInfo);
    virtual ~Shape(void);

    void loadBox(float width, float height, float depth);
    void loadSkybox(float half_size);

    void loadSquare(float w, float h);
    void loadSphere(unsigned int ring, unsigned int slice, float radius);
    void loadCylinder(unsigned int slice, float radius, float heigh);
    void loadCone(unsigned int edgesCount, float radius, float height);
    void loadTorus(unsigned int sideCount, unsigned int slice, float radius, float radiusBis);

    void draw(void);
private:
    void load(const std::vector<float> &vertice, const std::vector<float> &textUV, const std::vector<float> &normals, const std::vector<unsigned int> &indices);

    OGL_Buffer_uptr mVbo; // Vertex
    OGL_Buffer_uptr mIbo;

    unsigned int mVerticeCount;
    unsigned int mIndicesCount;
};

#endif // SHAPE_H
