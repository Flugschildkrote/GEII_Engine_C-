#include <iostream>
#include <windows.h>
#include "GL/glew.h"
#include "OGL_Buffer.h"
#include "GL_ErrorHandler.h"
#include "OGL_Shader.h"
#include "Shape.h"
#include "geException.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/constants.hpp"
#include "ImageLoader.h"
#include "OGL_Texture.h"
#include "Mtl_Loader.h"
#include "Scene.h"
#include "Obj_Loader.h"
#include "Render.h"
#include "Camera.h"
/*#include "MathUtil.h"
#include "Vector2.h"
#include "Vector3.h"*/
#include "MainApplication.h"

using namespace std;

int main(int argc, char **argv)
{
    MainApplication myApp("EngineGEII [C++]", 800, 600);
    myApp.run();

    return 0;
}
