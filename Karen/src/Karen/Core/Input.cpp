#include "pch.h"
#include "Karen/Core/Input.h"


namespace Karen
{
  Input* Input::s_instance = nullptr;

  void Input::create(Input* instance)
  {
    KAREN_CORE_ASSERT_MSG(!s_instance, "Input Has been created");
    s_instance = instance;
  }
}
