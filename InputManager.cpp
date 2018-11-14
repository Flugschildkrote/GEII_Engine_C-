#include "InputManager.h"

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    InputManager* manager = reinterpret_cast<InputManager*>(glfwGetWindowUserPointer(window));
    int keyValue;
    manager->mKeyModifiers = static_cast<uint8_t>(mods);
    switch(manager->mKeyMode){
    case KeyMode::KEYCODE :
        keyValue = key;
        break;
    case KeyMode::SCANCODE :
        keyValue = scancode;
        break;
    default :
        break;
    }
    if(keyValue == GLFW_KEY_UNKNOWN)
        return;

    switch(action){
    case GLFW_PRESS:
        manager->mKeys[keyValue] = true;
        break;
    case GLFW_RELEASE :
        if(manager->mKeys[keyValue]){
            manager->mFlags |= MOUSE_BUTTON_CLICKED_FLAG;
            manager->mClickedKeys[keyValue] = true;
        }
        manager->mKeys[keyValue] = false;
        break;
    default:
        break;
    }
    DEBUG_KEY_INPUT(std::cerr);
}

void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
    InputManager* manager = reinterpret_cast<InputManager*>(glfwGetWindowUserPointer(window));
    switch(action){
    case GLFW_PRESS:
        manager->mMouseButtons[button] = true;
        break;
    case GLFW_RELEASE :
        if(manager->mMouseButtons[button]){
            manager->mFlags |= MOUSE_BUTTON_CLICKED_FLAG;
            manager->mClickedMouseButtons[button] = true;
        }
        manager->mMouseButtons[button] = false;
        break;
    default:
        break;
    }
    DEBUG_MOUSE_BUTTON_INPUT(std::cerr);
}

void InputManager::mouseMovedCallback(GLFWwindow* window, double xpos, double ypos){
    InputManager* manager = reinterpret_cast<InputManager*>(glfwGetWindowUserPointer(window));
    manager->mCursorMoveX = xpos-manager->mCursorX;
    manager->mCursorMoveY = ypos-manager->mCursorY;
    manager->mCursorX = xpos;
    manager->mCursorY = ypos;

    manager->mFlags |= MOUSE_MOVED_FLAG;
    DEBUG_MOUSE_MOVEMENT(std::cerr);

}

void InputManager::windowResizedCallback(GLFWwindow* window, int width, int height){
    InputManager* manager = reinterpret_cast<InputManager*>(glfwGetWindowUserPointer(window));
    manager->mFlags |= WIND0W_RESIZED_FLAG;
    DEBUG_WINDOW_RESIZED(std::cerr);
}



InputManager::InputManager(KeyMode mode) : mKeyMode(mode), mKeyModifiers(0), mCursorX(0), mCursorY(0), mCursorMoveX(0), mCursorMoveY(0), mFlags(INPUT_FLAG_NONE){
    mKeys.fill(false);
    mMouseButtons.fill(false);
    mClickedKeys.fill(false);
    mClickedMouseButtons.fill(false);
}

InputManager::~InputManager(void) { }

bool InputManager::getFlagState(InputManagerFlag flag){ return flag & mFlags; }
void InputManager::resetFlags(InputManagerFlag flags){
    if(flags & KEY_CLICKED_FLAG){ mClickedKeys.fill(false); }
    if(flags & MOUSE_BUTTON_CLICKED_FLAG) { mClickedMouseButtons.fill(false); }
    mFlags &= (~flags);
}
