#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "geHeader.h"
#include "GLFW/glfw3.h"

#define KEY_ARRAY_SIZE GLFW_KEY_LAST+1
#define MOUSE_BUTTON_ARRAY_SIZE GLFW_MOUSE_BUTTON_LAST+1

#if defined(DEBUG) && defined(ENABLE_DEBUG_KEY_INPUT)
#define DEBUG_KEY_INPUT(stream) \
    std::string keyState; \
    switch(action){ \
    case GLFW_PRESS: \
        keyState = "pressed"; \
        break; \
    case GLFW_RELEASE : \
        keyState = "released"; \
        break;\
    default:\
        keyState = "unknonw state"; \
        break; \
    }\
    const char *name  = glfwGetKeyName(key, scancode);\
    std::string keyName = (name == NULL) ? "Unknown Name" : name; \
    stream << "Key \"" << keyName << "\" " <<  keyState  << " : \nKey Code : " << key << "\nScan Code : " << scancode <<  "\nUsed code : " << keyValue << "\n\n";
#else
    #define DEBUG_KEY_INPUT(stream)
#endif // ENABLE_DEBUG_KEY_INPUT

#if defined(DEBUG) && defined(ENABLE_DEBUG_MOUSE_BUTTON_INPUT)
    #define DEBUG_MOUSE_BUTTON_INPUT(stream) \
        std::string butState; \
        switch(action){ \
        case GLFW_PRESS: \
            butState = "pressed"; \
            break; \
        case GLFW_RELEASE : \
            butState = "released"; \
            break;\
        default:\
            butState = "unknonw state"; \
            break; \
        }\
        stream << "Mouse button " << button << " " <<  butState << "\n\n";
#else
    #define DEBUG_MOUSE_BUTTON_INPUT(stream)
#endif // ENABLE_DEBUG_MOUSE_BUTTON_INPUT

#if defined(DEBUG) && defined(ENABLE_DEBUG_MOUSE_MOVEMENT)
    #define DEBUG_MOUSE_MOVEMENT(stream) \
        stream << "Mouse moved : Pos[" << manager->mCursorX << "; " << manager->mCursorY << "], Move[ " << manager->mCursorMoveX << "; " << manager->mCursorMoveY << "]\n";
#else
    #define DEBUG_MOUSE_MOVEMENT(stream)
#endif // defined


#if defined(DEBUG) && defined(ENABLE_DEBUG_WINDOW_RESIZED)
    #define DEBUG_WINDOW_RESIZED(stream) stream << "Window Resized : ["  << width << "; " << height << "]\n";
#else
    #define DEBUG_WINDOW_RESIZED(stream)
#endif // ENABLE_DEBUG_WINDOW_RESIZED

enum class KeyMode : unsigned char{
    KEYCODE,
    SCANCODE
};

using InputManagerFlag = int;
constexpr InputManagerFlag INPUT_FLAG_NONE = 0x00;
constexpr InputManagerFlag WIND0W_RESIZED_FLAG = 0x01;
constexpr InputManagerFlag MOUSE_MOVED_FLAG = 0x02;
constexpr InputManagerFlag MOUSE_BUTTON_CLICKED_FLAG = 0x04;
constexpr InputManagerFlag KEY_CLICKED_FLAG = 0x08;
constexpr InputManagerFlag INPUT_FLAG_ALL = 0xFF;

class InputManager
{
public:
    InputManager(KeyMode mode = KeyMode::KEYCODE);
    virtual ~InputManager();

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void mouseMovedCallback(GLFWwindow* window, double xpos, double ypos);
    static void windowResizedCallback(GLFWwindow* window, int width, int height);

    bool getKey(int key) const { return mKeys[key]; }
    bool getKeyClicked(int key)const { return mClickedKeys[key]; }
    bool getKeyModifier(int mod) const { return static_cast<bool>(mKeyModifiers & mod); }
    bool getMouseButton(int button) const { return mMouseButtons[button]; }
    bool getMouseButtonClicked(int button) const { return mClickedMouseButtons[button]; }
    int getMouseX(void) const { return mCursorX; }
    int getMouseY(void) const { return mCursorY; }
    int getMouseMoveX(void) const { return mCursorMoveX; }
    int getMouseMoveY(void) const { return mCursorMoveY; }


    bool getFlagState(InputManagerFlag flag);
    void resetFlags(InputManagerFlag flags);
    //bool getChar(char key){ return mKeys[key]; }

    KeyMode getKeyMode(void) const { return mKeyMode; }

    void setKeyMode(KeyMode mode){ mKeyMode = mode; }
protected:
    std::array<bool, KEY_ARRAY_SIZE> mKeys;
    std::array<bool, KEY_ARRAY_SIZE> mClickedKeys;
    std::array<bool, MOUSE_BUTTON_ARRAY_SIZE> mMouseButtons;
    std::array<bool, MOUSE_BUTTON_ARRAY_SIZE> mClickedMouseButtons;

    uint8_t mKeyModifiers;
    KeyMode mKeyMode;

    int mCursorX, mCursorY, mCursorMoveX, mCursorMoveY;
    InputManagerFlag mFlags;

};


#endif // INPUTMANAGER_H
