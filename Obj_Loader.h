#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include "geHeader.h"
#include "Mtl_Loader.h"
#include <sstream>

#define EPS6 1E-6f

class Obj_Loader
{
public:
    Obj_Loader(void);
    Obj_Loader(const std::string &file, Scene* scene);
    virtual ~Obj_Loader(void);

    void loadFile(const std::string &file, Scene* scene);
    void clearData(void);

protected:
    void cmd_f(std::stringstream &stream);
    bool isConvexPolygon(unsigned int *array_p, unsigned int arraySize);
    void createGeometricPrimitive(Scene* scene);
    void generateNormals(void);

    std::vector<glm::vec3> mVertice;
    std::vector<glm::vec3> mNormals;
    std::vector<glm::vec2> mUVs;
    std::vector<glm::ivec3> mFaces;
    Mtl_Loader mMtlLoader;
    Material_sptr mCurrentLib;
    bool mComputeNormals;
private:
};

#endif // OBJ_LOADER_H
