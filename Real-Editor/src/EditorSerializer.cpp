#include "EditorSerializer.h"

#include <imgui.h>
#include <pugixml.hpp>
#include <string>


namespace Real
{
  static void serializeVector4(const Vec4& vec, pugi::xml_node& parent, const char* x_name = "X", const char* y_name = "Y", const char* z_name = "Z", const char* w_name = "W")
  {
    parent.append_child(x_name).append_child(pugi::node_pcdata).set_value(std::to_string(vec.x).c_str());
    parent.append_child(y_name).append_child(pugi::node_pcdata).set_value(std::to_string(vec.y).c_str());
    parent.append_child(z_name).append_child(pugi::node_pcdata).set_value(std::to_string(vec.z).c_str());
    parent.append_child(w_name).append_child(pugi::node_pcdata).set_value(std::to_string(vec.w).c_str());
  }

  EditorSerializer::EditorSerializer(EditorLayer* editor)//const ARef<EditorLayer>& editor)
    : m_context(editor)
  {
  }

  void EditorSerializer::setContext(EditorLayer* editor)//const ARef<EditorLayer>& editor)
  {
    m_context = editor;
  }

  bool EditorSerializer::serialize(const char* path)
  {
    pugi::xml_document doc;
    auto editor = doc.append_child("Editor");

    //colors
    {
      pugi::xml_node color_schemes = editor.append_child("ColorScheme");
      for(auto iter = m_context->m_colors.begin(); iter != m_context->m_colors.end(); ++iter)
      {
        auto color = color_schemes.append_child("Color");
        auto For = color.append_child("For"); 
        For.append_child(pugi::node_pcdata).set_value(iter->first.c_str());

        serializeVector4(iter->second * 255.0f, color, "Red", "Green", "Blue", "Alpha");
      }
    }
 
    //Windows
    {
      pugi::xml_node windows = editor.append_child("Windows");

      pugi::xml_node stats_window = windows.append_child("StatsWindow");
      auto state = stats_window.append_child("Active");
      const auto stats_window_ptr = (StatsWindow*)m_context->m_helper_windows.at("Stats").get();
      bool is_active = stats_window_ptr->is_active;
      
      std::string val = is_active == true ? "True" : "False";
      state.append_child(pugi::node_pcdata).set_value(val.c_str());
    }

     //imgui
    {
      pugi::xml_node imgui = editor.append_child("ImGui");
      pugi::xml_node imgui_ini = imgui.append_child("ImGuiIni");
      auto path = imgui_ini.append_child("Path");
      auto ini_path = path.append_child(pugi::node_pcdata).set_value(m_context->m_imgui_ini_path.c_str());
    }
    
    //Fonts
    {
      pugi::xml_node fonts = editor.append_child("Fonts");
      pugi::xml_node default_font = fonts.append_child("Default");
      auto default_path = default_font.append_child("Path").append_child(pugi::node_pcdata).set_value(m_context->m_default_font.c_str());

      auto default_size = default_font.append_child("Size").append_child(pugi::node_pcdata).set_value(std::to_string(m_context->m_default_font_size).c_str());
    }
    return doc.save_file(path, PUGIXML_TEXT("  "));
  }

  bool EditorSerializer::deSerialize(const char* path)
  {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path, pugi::parse_default|pugi::parse_declaration);
    if (!result)
    {
      REAL_CORE_ERROR("Parse Error at: {0}, offset: {1}",result.description(), result.offset);
      return false;
    }
    
    pugi::xml_node editor = doc.child("Editor");

    //Colors
    {
      bool empty = false;
      pugi::xml_node color_schemes = editor.child("ColorScheme");
      for(pugi::xml_node color : color_schemes.children("Color"))
      {
        std::string type = color.child_value("For");
        std::string red = color.child_value("Red");
        std::string green = color.child_value("Green");
        std::string blue = color.child_value("Blue");
        std::string alpha = color.child_value("Alpha");
        REAL_TRACE("type: {0}, red: {1}, green: {2}, blue: {3}, alpha: {4}", 
            type, red, green, blue, alpha);
        if(type.empty() || red.empty() || green.empty() || blue.empty() || alpha.empty())
          empty = true;
        if(!empty)
        {
          Vec4 normalized_color = {std::stof(red), std::stof(green), std::stof(blue), std::stof(alpha)};
          normalized_color /= 255.0f;
          REAL_TRACE("normalized color: {0}", normalized_color);
          m_context->m_colors.at(type) = normalized_color;
        }
      }
    }

    //Windows 
    //For now its just Active/Not maybe in future it will also hold the imgui.ini file data but for now they are Separated 
    {
      pugi::xml_node windows = editor.child("Windows");

      pugi::xml_node stats_window = windows.child("StatsWindow");
      std::string state = stats_window.child_value("Active");
      REAL_TRACE("StatsWindow: Active: {0}", state);
      const auto stats_window_ptr = (StatsWindow*)m_context->m_helper_windows.at("Stats").get();
      if(!state.empty())
        stats_window_ptr->is_active = (state == "True");
    }
    //ImGui 
    {
      pugi::xml_node imgui = editor.child("ImGui");
      pugi::xml_node imgui_ini = imgui.child("ImGuiIni");
      std::string ini_path = imgui_ini.child_value("Path");
      REAL_TRACE("Ini Path: {0}", ini_path);
      if(!ini_path.empty())
      {
        m_context->m_imgui_ini_path.clear();
        m_context->m_imgui_ini_path = ini_path;
      }
    }
    
    //Fonts
    {
      pugi::xml_node fonts = editor.child("Fonts");
      pugi::xml_node default_font = fonts.child("Default");
      std::string default_path = default_font.child_value("Path");
      std::string default_size = default_font.child_value("Size");
      REAL_TRACE("Default Font: Path: {0}, Size: {1}", default_path, default_size);
      if(!default_path.empty())
      {
        m_context->m_default_font.clear();
        m_context->m_default_font = default_path;
      }
      if(!default_size.empty())
        m_context->m_default_font_size = std::stoul(default_size);
    }
    return true;
  }
}
