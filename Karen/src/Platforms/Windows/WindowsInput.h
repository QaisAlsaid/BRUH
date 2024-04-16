#ifndef WINDOWS_INPUT_H
#define WINDOWS_INPUT_H
#ifdef _WIN32

#include "Karen/Core/Core.h"
#include "Karen/Core/Input.h"

namespace Karen
{
  class WindowsInput : public Input
  {
  protected:
    bool isKeyPressedImpl(int key_code)             override;
    bool isMouseButtonPressedImpl(int button_code)  override;
    bool isKeyReleasedImpl(int key_code)            override;
    bool isMouseButtonReleasedImpl(int button_code) override;          
    Vec2 getMousePosImpl()       override;
  };
}

#endif //_WIN32
#endif //WINDOWS_INPUT_H
