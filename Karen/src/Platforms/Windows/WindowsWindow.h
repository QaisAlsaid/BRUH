#ifndef WINDOWS_WINDOW_H
#define WINDOWS_WINDOW_H
#ifdef _WIN32

#include "Karen/Core/Window.h"
#include "Karen/Render/API/Context.h"

struct GLFWwindow;

namespace Karen
{
  class WindowsWindow : public Window
  {
  public:
    WindowsWindow(const WindowSpecs& specs);
    ~WindowsWindow() override;
    inline unsigned int getWidth()  const override {return m_data.width;}
    inline unsigned int getHeight() const override {return m_data.height;}
  
    inline void setEventCallbackFunction(const EventCallbackFunc& func) override 
    {
      m_data.event_callback_func = func;
    }

    void onUpdate() override;
    void setVsync(bool vsync) override;
    inline bool isVsync() const override
    {
      return m_data.vsync;
    }
    inline void* getNativeWindow() const override
    {
      return m_window;
    }
  private:
    void init(const WindowSpecs& specs);
    void close();
  private:
    struct WindowData
    {
      std::string title;
      unsigned int width, height;
      bool vsync;

      EventCallbackFunc event_callback_func;
    };

    GLFWwindow* m_window;
    WindowData  m_data;
    Context*    m_context;
  };
}

#endif //_WIN32
#endif //WINDOWS_WINDOW_H
