#include "pch.h"
#include "LinuxInput.h"
#include "Karen/Core/App.h"
#include <GLFW/glfw3.h>

namespace Karen
{
  bool LinuxInput::isKeyPressedImpl(int key_code)
  {
    auto* window = (GLFWwindow*)App::get()->getWindow().getNativeWindow();
    auto  status = glfwGetKey(window, key_code);
    return (status == GLFW_PRESS || status == GLFW_REPEAT);
  }

  Vec2 LinuxInput::getMousePosImpl()
  {
    auto* window = (GLFWwindow*)App::get()->getWindow().getNativeWindow();
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return Vec2((float)x, (float)y);
  }

  bool LinuxInput::isMouseButtonPressedImpl(int button_code)
  {
    auto* window = (GLFWwindow*)App::get()->getWindow().getNativeWindow();
    auto status = glfwGetMouseButton(window, button_code);
    return (status == GLFW_PRESS);
  }

  bool LinuxInput::isKeyReleasedImpl(int key_code)
  {
    auto* window = (GLFWwindow*)App::get()->getWindow().getNativeWindow();
    auto status = glfwGetKey(window, key_code);
    return status == GLFW_RELEASE;
  }

  bool LinuxInput::isMouseButtonReleasedImpl(int button_code)
  {
    auto* window = (GLFWwindow*)App::get()->getWindow().getNativeWindow();
    auto status = glfwGetMouseButton(window, button_code);
    return status == GLFW_RELEASE;
  }
}
