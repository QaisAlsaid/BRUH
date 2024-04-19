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
    inline static bool isKeyPressed(Karen::Keyboard key)          {return s_instance->isKeyPressedImpl(key);}
    inline static bool isMouseButtonPressed(Karen::Mouse button)  {return s_instance->isMouseButtonPressedImpl(button);}
    inline static Vec2 getMousePos()
    {return s_instance->getMousePosImpl();}
    inline static bool isKeyReleased(Karen::Keyboard key_code)         {return s_instance->isKeyReleasedImpl(key_code);}
    inline static bool isMouseButtonReleased(Karen::Mouse button_code) {return s_instance->isMouseButtonReleasedImpl(button_code);}
  protected:
    virtual bool isKeyPressedImpl(Karen::Keyboard key)             = 0;
    virtual bool isMouseButtonPressedImpl(Karen::Mouse button)     = 0;
    virtual Vec2 getMousePosImpl()                                 = 0;
    virtual bool isKeyReleasedImpl(Karen::Keyboard key)            = 0;
    virtual bool isMouseButtonReleasedImpl(Karen::Mouse button)    = 0;
  public:
    static Input* s_instance;
  };
}
#endif //INPUT_H
