#ifdef _WIN32

#include "pch.h"
#include "Platforms/Windows/WindowsInput.h"
#include "Karen/Core/App.h"
#include <GLFW/glfw3.h>


namespace Karen
{
  bool WindowsInput::isKeyPressedImpl(int key_code)
  {
    auto* window = (GLFWwindow*)App::get()->getWindow().getNativeWindow();
    auto  status = glfwGetKey(window, key_code);
    return (status == GLFW_PRESS || status == GLFW_REPEAT);
  }

  float WindowsInput::getMousePosImpl()
  {
    auto* window = (GLFWwindow*)App::get()->getWindow().getNativeWindow();
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return Vec2((float)x, (float)y);
  }

  bool WindowsInput::isMouseButtonPressedImpl(int button_code)
  {
    auto* window = (GLFWwindow*)App::get()->getWindow().getNativeWindow();
    auto status = glfwGetMouseButton(window, button_code);
    return (status == GLFW_PRESS);
  }

  bool WindowsInput::isKeyReleasedImpl(int key_code)
  {
    auto* window = (GLFWwindow*)App::get()->getWindow().getNativeWindow();
    auto status = glfwGetKey(window, key_code);
    return status == GLFW_RELEASE;
  }

  bool WindowsInput::isMouseButtonReleasedImpl(int button_code)
  {
    auto* window = (GLFWwindow*)App::get()->getWindow().getNativeWindow();
    auto status = glfwGetMouseButton(window, button_code);
    return status == GLFW_RELEASE;
  }
}

#endif //_WIN32
