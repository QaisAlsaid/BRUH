#ifndef INPUT_H
#define INPUT_H

#include "pch.h"
#include "Karen/Core/Core.h"
#include "Karen/Core/Math/math.h"
#include "ButtonsAndKeyCodes.h"


namespace Karen
{
  class KAREN_API Input
  {
  public:
    Input() = default;
    static void create(Input* instance);
    static bool isKeyPressed(Karen::Keyboard key);
    static bool isMouseButtonPressed(Karen::Mouse button);
    static Vec2 getMousePos();
    static bool isKeyReleased(Karen::Keyboard key_code);
    static bool isMouseButtonReleased(Karen::Mouse button_code);
  };
}
#endif //INPUT_H
