#include "pch.h"
#include "LinuxInput.h"
#include "Karen/Core/App.h"
#include <GLFW/glfw3.h>

namespace Karen
{
  bool LinuxInput::isKeyPressedImpl(Keyboard key)
  {
    auto* window = (GLFWwindow*)App::get()->getWindow().getNativeWindow();
    auto  status = glfwGetKey(window, (int)key);
    return (status == GLFW_PRESS || status == GLFW_REPEAT);
  }

  Vec2 LinuxInput::getMousePosImpl()
  {
    auto* window = (GLFWwindow*)App::get()->getWindow().getNativeWindow();
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return Vec2((float)x, (float)y);
  }

  bool LinuxInput::isMouseButtonPressedImpl(Mouse button)
  {
    auto* window = (GLFWwindow*)App::get()->getWindow().getNativeWindow();
    auto status = glfwGetMouseButton(window, (int)button);
    return (status == GLFW_PRESS);
  }

  bool LinuxInput::isKeyReleasedImpl(Keyboard key)
  {
    auto* window = (GLFWwindow*)App::get()->getWindow().getNativeWindow();
    auto status = glfwGetKey(window, (int)key);
    return status == GLFW_RELEASE;
  }

  bool LinuxInput::isMouseButtonReleasedImpl(Mouse button)
  {
    auto* window = (GLFWwindow*)App::get()->getWindow().getNativeWindow();
    auto status = glfwGetMouseButton(window, (int)button);
    return status == GLFW_RELEASE;
  }
}
