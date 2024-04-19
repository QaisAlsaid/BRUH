#ifndef WINDOWS_INPUT_H
#define WINDOWS_INPUT_H

#include "Karen/Core/Core.h"
#include "Karen/Core/Input.h"

namespace Karen
{
  class WindowsInput : public Input
  {
  protected:
    bool isKeyPressedImpl(Karen::Keyboard key)          override;
    bool isMouseButtonPressedImpl(Karen::Mouse button)  override;
    bool isKeyReleasedImpl(Karen::Keyboard key)         override;
    bool isMouseButtonReleasedImpl(Karen::Mouse button) override;          
    Vec2 getMousePosImpl()                              override;
  };
}

#endif //WINDOWS_INPUT_H
