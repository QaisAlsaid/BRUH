#ifndef INPUT_H
#define INPUT_H

#include "pch.h"
#include "Real-Engine/Core/Core.h"
#include "Real-Engine/Core/Math/math.h"
#include "ButtonsAndKeyCodes.h"


namespace Real
{
  class REAL_API Input
  {
  public:
    Input() = default;
    static void create(Input* instance);
    static bool isKeyPressed(Real::Keyboard key);
    static bool isMouseButtonPressed(Real::Mouse button);
    static Vec2 getMousePos();
    static bool isKeyReleased(Real::Keyboard key_code);
    static bool isMouseButtonReleased(Real::Mouse button_code);
  };
}
#endif //INPUT_H
