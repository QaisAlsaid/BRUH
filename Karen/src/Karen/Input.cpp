#include "pch.h"
#include "Karen/Input.h"


namespace Karen
{
  Input* Input::stat_instance = nullptr;

  void Input::create(Input* instance)
  {
    KAREN_CORE_ASSERT(!stat_instance, "Input Has been created");
    stat_instance = instance;
  }
}
