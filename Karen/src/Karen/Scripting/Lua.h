#pragma once
#include "Karen/Core/App.h"
#include <functional>
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>


struct DynamicObject {
	std::unordered_map<std::string, sol::object> entries;

	void dynamic_set(std::string key, sol::stack_object value) {
		auto it = entries.find(key);
		if (it == entries.cend()) {
			entries.insert(
			     it, { std::move(key), std::move(value) });
		}
		else {
			std::pair<const std::string, sol::object>& kvp
			     = *it;
			sol::object& entry = kvp.second;
			entry = sol::object(std::move(value));
		}
	}

	sol::object dynamic_get(std::string key) {
		auto it = entries.find(key);
		if (it == entries.cend()) {
			return sol::lua_nil;
		}
		return it->second;
	}
};



namespace Karen
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
    static sol::table karen;
  };
}
