#ifndef INPUT_H
#define INPUT_H

#include "pch.h"
#include "Karen/Core/Core.h"
#include "Karen/Core/Math/math.h"

namespace Karen
{
  class KAREN_API Input
  {
  public:
    Input(){}
    static void create(Input* instance);
    inline static bool isKeyPressed(int key_code)             {return stat_instance->isKeyPressedImpl(key_code);}
    inline static bool isMouseButtonPressed(int button_code)  {return stat_instance->isMouseButtonPressedImpl(button_code);}
    //TODO:Vec2
    inline static Vec2 getMousePos()
    {return stat_instance->getMousePosImpl();}
    inline static bool isKeyReleased(int key_code)            {return stat_instance->isKeyReleasedImpl(key_code);}
    inline static bool isMouseButtonReleased(int button_code) {return stat_instance->isMouseButtonReleasedImpl(button_code);}
  protected:
    virtual bool isKeyPressedImpl(int key_code)             = 0;
    virtual bool isMouseButtonPressedImpl(int button_code)  = 0;
    virtual Vec2 getMousePosImpl()                          = 0;
    virtual bool isKeyReleasedImpl(int key_code)            = 0;
    virtual bool isMouseButtonReleasedImpl(int button_code) = 0;
  public:
    static Input* stat_instance;
  };
}
#endif //INPUT_H
