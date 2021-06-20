#include "Application.h"
#include "Input.h"

namespace Nebula{
    Input* Input::s_Input = new Input;

    bool Input::IsKeyPressedInt(KeyCode key) 
    {
        auto win = (GLFWwindow*)(Application::Get()->GetWindow()->GetNativeWindow());
        auto state = glfwGetKey(win, (int)key);
        return state == GLFW_PRESS || state == GLFW_REPEAT;

    }
    bool Input::IsMouseButtonPressedInt(MouseCode key) 
    { 
        auto win = (GLFWwindow*)(Application::Get()->GetWindow()->GetNativeWindow());
        auto state = glfwGetMouseButton(win, (int)key);
        return state == GLFW_PRESS;
    }

    Vec2f Input::GetMousePosInt() 
    { 
        auto win = (GLFWwindow*)(Application::Get()->GetWindow()->GetNativeWindow());
        double x, y;
        glfwGetCursorPos(win, &x, &y);
        return Vec2f((float)x, (float)y);
    }
}