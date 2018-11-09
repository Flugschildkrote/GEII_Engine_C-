#ifndef MTL_LOADER_H
#define MTL_LOADER_H

#include "geHeader.h"

struct MtlMaterial{
    std::string materialName;
    Material_sptr material;
};

class Mtl_Loader
{
public:
    Mtl_Loader(void);
    Mtl_Loader(const std::string &fileName, Scene* scene);
    virtual ~Mtl_Loader(void);

    void loadLib(const std::string &fileName, Scene* scene);
    void clearData(void);

    const std::string toString(void) const;
    const std::string getFile(void) const;
    std::vector<MtlMaterial> getMtlLib(void) const;
private:
    std::string mFile;
    std::vector<MtlMaterial> mMtlList;
};

#endif // MTL_LOADER_H
