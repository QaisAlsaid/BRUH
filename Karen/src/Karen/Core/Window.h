#ifndef WINDOW_H
#define WINDOW_H
#include "Karen/Core/Core.h"
#include "Karen/Core/Events/Events.h"

struct GLFWwindow;

namespace Karen
{
  static const std::string DefaultTitle = "Karen Engine";
  static const unsigned int DefaultWidth = 1280, DefaultHeight = 720;
  struct WindowSpecs
  {
   std::string title;
   unsigned int width, height;

    WindowSpecs(const std::string& title = DefaultTitle,
    unsigned int width = DefaultWidth, unsigned int height = DefaultHeight)
      : title(title), width(width), height(height) {}
  };


  class Window
  {
  public:
    using EventCallbackFunc = std::function<void(Event&)>;
    virtual ~Window()       {};
    virtual void onUpdate() = 0;

    virtual unsigned int getWidth()  const = 0;
    virtual unsigned int getHeight() const = 0;

    virtual void setEventCallbackFunction(const EventCallbackFunc& func)       = 0;
    virtual bool isVsync()                                               const = 0;
    virtual void setVsync(bool vsync)                                          = 0;


    static Window* create(const WindowSpecs& specs = WindowSpecs());
    virtual void* getNativeWindow() const = 0;
  };
}
#endif //WINDOW_H
