#ifndef LINUX_INPUT_H
#define LINUX_INPUT_H

#include "Karen/Core.h"
#include "Karen/Input.h"

namespace Karen
{
  class LinuxInput : public Input
  {
  protected:
    bool isKeyPressedImpl(int key_code)             override;
    bool isMouseButtonPressedImpl(int button_code)  override;
    bool isKeyReleasedImpl(int key_code)            override;
    bool isMouseButtonReleasedImpl(int button_code) override;          
    float getMousePosImpl()       override;
  };
}

#endif //LINUX_INPUT_H
