#include "pch.h"

#ifdef REAL_PLATFORM_WINDOWS

#include "WindowsWindow.h"
#include "Platforms/OpenGl/OpenGlContext.h"
#include "Real-Engine/Core/Input.h"

#include <GLFW/glfw3.h>


static bool s_is_glfw_init = false;
static void glfwerrorCallback(int code, const char* message)
{
  REAL_CORE_CRITICAL("GLFW ERROE: {0} : {1}", code, message);
}
namespace Real 
{
  Window* Window::create(const WindowSpecs& specs)
  { 
    return new WindowsWindow(specs);
  }

  WindowsWindow::WindowsWindow(const WindowSpecs& specs)
  {
    init(specs);
  }

  WindowsWindow::~WindowsWindow()
  {
    close();
  }
    
  void WindowsWindow::init(const WindowSpecs& specs)
  {
    REAL_PROFILE_FUNCTION();
    m_data.title  = specs.title;
    m_data.width  = specs.width;
    m_data.height = specs.height;
    
    if(!s_is_glfw_init)
    {
      int glfw_init_status = glfwInit();
      REAL_CORE_ASSERT_MSG(glfw_init_status, "Failed Initializing GLFW Window")
      s_is_glfw_init = true;
      glfwSetErrorCallback(glfwerrorCallback);
    }
    m_window = glfwCreateWindow(int(specs.width), int(specs.height), specs.title.c_str(), nullptr, nullptr);
    m_context = new OpenGlContext(m_window);
    m_context->init();
    glfwSetWindowUserPointer(m_window, &m_data);
    REAL_CORE_INFO("Window Created size: {0}, {1} Title: {2}",specs.width, specs.height, specs.title);

    glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
    {
      auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
      data.width = width;
      data.height = height;
      WindowResizeEvent event(width, height);
      data.event_callback_func(event);
    });

    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
    {
      auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
      WindowClosedEvent event;
      data.event_callback_func(event);
    });

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int actions, int mods)
    {
      auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
      switch(actions)
      {
        case GLFW_PRESS:
        {
          KeyPressedEvent event(key, 0);
          data.event_callback_func(event);
          break;
        }
        case GLFW_REPEAT:
        {
          KeyPressedEvent event(key, 1);
          data.event_callback_func(event);
          break;
        }
        case GLFW_RELEASE:
        {
          KeyReleasedEvent event(key);
          data.event_callback_func(event);
          break;
        }
      }
    });

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int actions, int mods)
    {
      auto& data = *(WindowData*) glfwGetWindowUserPointer(window);
      switch(actions)
      {
        case GLFW_PRESS:
        {
          MouseButtonPressed event(button);
          data.event_callback_func(event);
          break;
        }
        case GLFW_RELEASE:
        {
          MouseButtonReleased event(button);
          data.event_callback_func(event);
          break;
        }
      }
    });

    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset)
    {
      auto& data = *(WindowData*) glfwGetWindowUserPointer(window);
      MouseScrolledEvent event((float)xoffset, (float)yoffset);
      data.event_callback_func(event);
    });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double x, double y)
    {
      auto& data = *(WindowData*) glfwGetWindowUserPointer(window);
      MouseMovedEvent event((float)x, (float)y);
      data.event_callback_func(event);
    });
  }

  void WindowsWindow::close()
  {
    glfwDestroyWindow(m_window);
  }

  void WindowsWindow::onUpdate()
  {
    glfwPollEvents();
    m_context->swapBuffers();
  }

  void WindowsWindow::setVsync(bool vsync)
  {
    vsync == true ? glfwSwapInterval(1) : glfwSwapInterval(0);
    m_data.vsync = vsync;
  }
}

#endif //REAL_PLATFORM_WINDOWS
