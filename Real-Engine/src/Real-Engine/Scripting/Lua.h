#ifndef KR_LUA_H
#define KR_LUA_H


#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

namespace Real
{
  class Lua 
  {
  public:
    static void init();
    inline static sol::state& get() { return lua; }
  private:
    static void initCore();
    static void initMath();
    static void initRenderer();
    static void initScripting();
    static void initScene();
  private:
    static sol::state lua;
    static sol::table real;
  };
}

#endif //KR_LUA_H
