#ifdef _WIN32

#include "pch.h"
#include "Platforms/Windows/WindowsInput.h"
#include "Karen/Core/App.h"
#include <GLFW/glfw3.h>


namespace Karen
{
  bool WindowsInput::isKeyPressedImpl(Keyboard key)
  {
    auto* window = (GLFWwindow*)App::get()->getWindow().getNativeWindow();
    auto  status = glfwGetKey(window, (int)key);
    return (status == GLFW_PRESS || status == GLFW_REPEAT);
  }

  float WindowsInput::getMousePosImpl()
  {
    auto* window = (GLFWwindow*)App::get()->getWindow().getNativeWindow();
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return Vec2((float)x, (float)y);
  }

  bool WindowsInput::isMouseButtonPressedImpl(Mouse button)
  {
    auto* window = (GLFWwindow*)App::get()->getWindow().getNativeWindow();
    auto status = glfwGetMouseButton(window, (int)button);
    return (status == GLFW_PRESS);
  }

  bool WindowsInput::isKeyReleasedImpl(Keyboard key)
  {
    auto* window = (GLFWwindow*)App::get()->getWindow().getNativeWindow();
    auto status = glfwGetKey(window, (int)key);
    return status == GLFW_RELEASE;
  }

  bool WindowsInput::isMouseButtonReleasedImpl(Mouse button)
  {
    auto* window = (GLFWwindow*)App::get()->getWindow().getNativeWindow();
    auto status = glfwGetMouseButton(window, (int)button);
    return status == GLFW_RELEASE;
  }
}

#endif //_WIN32
