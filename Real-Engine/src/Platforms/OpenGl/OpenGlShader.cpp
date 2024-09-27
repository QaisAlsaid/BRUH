#include "pch.h"
#include "Platforms/OpenGl/OpenGlShader.h"
#include "Real-Engine/Core/Core.h"
#include "Platforms/OpenGl/OpenGlCore.h"
#include "Real-Engine/Render/API/RendererCapabilities.h"

//TODO: all this code is MVP and you should 
//replace it ASAP
enum ToksEnum
{
  REAL_MAX_TEXTUERS,
  REAL_CURRENT_SWITCH_CASE,
  REAL_VERTEX,
  REAL_FRAGMENT,
  REAL_SHADER_TYPE,
  REAL_SAMPLE_TEMPLATE,
  REAL_MAX_TEXTUER_SWITCH,
  REAL_NONE_TOK
};
//toks that can be replaced with ${}
static std::unordered_map<std::string, ToksEnum> var_toks;
//toks that have # before 
static std::unordered_map<std::string, ToksEnum> hash_toks;
//toks that used as macro to do stuff
static std::unordered_map<std::string, ToksEnum> macro_toks;
//all toks for ease of use
static std::unordered_map<std::string, ToksEnum> stand_alone_toks;

static void initToks()
{
  //TODO : remove stand_alone_toks and make ordere for eval
  //like: first check for hash_toks if found handel then countinue the loop and if not check for macro_toks and last check for var_toks
  stand_alone_toks["#REAL_SHADER_TYPE"]         = ToksEnum::REAL_SHADER_TYPE;
  stand_alone_toks["#REAL_SAMPLE_TEMPLATE"]     = ToksEnum::REAL_SAMPLE_TEMPLATE;
  stand_alone_toks["#REAL_MAX_TEXTUER_SWITCH"]  = ToksEnum::REAL_MAX_TEXTUER_SWITCH;
 
  var_toks["REAL_MAX_TEXTUERS"]        = ToksEnum::REAL_MAX_TEXTUERS;
  var_toks["REAL_CURRENT_SWITCH_CASE"] = ToksEnum::REAL_CURRENT_SWITCH_CASE;
  var_toks["REAL_VERTEX"]              = ToksEnum::REAL_VERTEX;
  var_toks["REAL_FRAGMENT"]            = ToksEnum::REAL_FRAGMENT;
 
  hash_toks["#REAL_SHADER_TYPE"]        = ToksEnum::REAL_SHADER_TYPE;
  hash_toks["#REAL_SAMPLE_TEMPLATE"]    = ToksEnum::REAL_SAMPLE_TEMPLATE;
 
  macro_toks["#REAL_MAX_TEXTUER_SWITCH"]            = ToksEnum::REAL_MAX_TEXTUER_SWITCH;
}

//TODO: move to a string class or something
static std::string replaceVar(std::string target, int var_val)
{
  if(target.find('$') == std::string::npos)
  {
    REAL_CORE_ASSERT_MSG(false, "No '$' found in target: " + target);
    return "";
  }
  auto pos = target.find('$');
  auto end_pos = target.find('}');
  target.replace(pos, end_pos - pos + 1, std::to_string(var_val));
  return target;
}

void writeMaxTextuerSwitch(std::string& line, const std::string& sample_template, int max_tux)
{
  REAL_CORE_ASSERT_MSG(sample_template != "", "#REAL_MAX_TEXTUER_SWITCH can't be used without given REAL_SAMPLE_TEMPLATE first");
  std::stringstream ss;
  auto pos = line.find("REAL_MAX_TEXTUER_SWITCH");
  const auto on = line.substr(pos + 24, line.size() - 2);
  ss << "switch("<< on <<"\n";
  ss << "{\n";
  for(uint8_t i = 0; i < max_tux; ++i)
  {
    ss << "case "<< std::to_string(i) <<":\n";
    auto rst = replaceVar(sample_template, i);
    ss << rst << '\n';
    ss << "break;\n";
  }
  ss << "}\n";
  line = ss.str();
}

static std::pair<std::string, ToksEnum> findStandAloneTok(const std::string& line)
{
  for(auto iter = stand_alone_toks.begin(); iter != stand_alone_toks.end(); ++iter)
  {
    const auto pos = line.find(iter->first);
    if(pos != std::string::npos)
    {
      return *iter;
    }
  }
  return std::make_pair(std::string(""), ToksEnum::REAL_NONE_TOK);
}

static std::pair<std::string, ToksEnum> findVarToks(const std::string& line)
{
  for(auto iter = var_toks.begin(); iter != var_toks.end();  ++iter)
  {
    const auto pos = line.find(iter->first);
    if(pos != std::string::npos)
    {
      return *iter;
    }
  }
  return std::make_pair(std::string(""), ToksEnum::REAL_NONE_TOK);
}

namespace Real
{
  template<char Remove> bool BothAre(char lhs, char rhs)
  {
    return lhs == rhs && lhs == Remove;
  }

  void split(const std::string& str, std::vector<std::string>& cont, char sep = ' '); 
 

  OpenGlShader::OpenGlShader()
  {
    //FIXME: initToks(); should be called one time on Renderer init;
    initToks();
  }

  OpenGlShader::OpenGlShader(const std::string& p)
  {
    initToks();
    loadFromFile(p);
  }

  OpenGlShader::~OpenGlShader()
  {
    REAL_PROFILE_FUNCTION();
    glCall(glDeleteProgram(m_program_id));
  }

  void OpenGlShader::bind() const
  {
    REAL_PROFILE_FUNCTION();
    glCall(glUseProgram(m_program_id));
  }

  void OpenGlShader::unbind() const
  {
    REAL_PROFILE_FUNCTION();
    glCall(glUseProgram(0));
  }

  void OpenGlShader::loadFromFile(const std::string& fp)
  {
    std::string vs, fs;
    preprocessShader(fp, vs, fs);
    if(compileShaders(vs, fs))
      if(createProgram())
        cacheUniforms(vs, fs);
      else {REAL_CORE_WARN("Error Creating Shader Program No Uniforms Will Be Cached");}
    else {REAL_CORE_WARN("Error Compiling Shader No Uniforms Will Be Cached");}
  }

  bool OpenGlShader::preprocessShader(const std::string& fp, std::string& vs, std::string& fs)
  {
    ShaderType sh_ty = ShaderType::Vertex;
    int max_tux = RendererCapabilities::getMaxTextureUints();
    std::string src, line, sample_template;
    std::stringstream src_ss[2];
    std::ifstream fstream(fp);
    if(!fstream) 
    {
      REAL_CORE_ERROR("Cant open file {0}", fp);
      return false;
    }
    while(std::getline(fstream, line))
    {
      auto sa_pair = findStandAloneTok(line);
      auto var_pair = findVarToks(line);
      if(sa_pair.first != "")
      {
        switch(sa_pair.second)
        {
          case ToksEnum::REAL_SHADER_TYPE:
            if(line.find("REAL_VERTEX") != std::string::npos) sh_ty = ShaderType::Vertex;
            else if(line.find("REAL_FRAGMENT") != std::string::npos) sh_ty = ShaderType::Fragment;
            break;
          case ToksEnum::REAL_SAMPLE_TEMPLATE:
            sample_template = line.substr(22);
            REAL_CORE_TRACE("sample_template: {0}", sample_template);
            break;
          case ToksEnum::REAL_MAX_TEXTUER_SWITCH:
            REAL_TRACE("CALLING replace from MAX tux switch line {0}", line);
            writeMaxTextuerSwitch(line, sample_template, max_tux);
            REAL_TRACE("Line: {0} added after tok", line);
            src_ss[(int)sh_ty] << line << '\n';
            break;
          default:
            REAL_CORE_WARN("Tok: {0} have no such use like: {1}, it will be ignored", sa_pair.first, line);
        }
      }
      else if(var_pair.first != "")
      {
        switch(var_pair.second)
        {
          case ToksEnum::REAL_MAX_TEXTUERS:
            line = replaceVar(line, max_tux);
            src_ss[(int)sh_ty] << line << '\n';
            break;
          default:
            REAL_WARN("Tok: {0} Don't have a value", var_pair.first);
            break;
        }
      }
      else
      {
        src_ss[(int)sh_ty] << line << '\n';
      }
    }
    vs = src_ss[(int)ShaderType::Vertex].str();
    fs = src_ss[(int)ShaderType::Fragment].str();
    REAL_CORE_TRACE("Vertex : {0},   Fragment : {1}", vs, fs);
    return true;
  }
  bool OpenGlShader::compileShaders(const std::string& vs, const std::string& fs)
  {
    REAL_PROFILE_FUNCTION();
    bool _status = true;
    const char *v_cstr = vs.c_str(), *f_cstr = fs.c_str();
    int status_v, status_f;
    {
      REAL_PROFILE_SCOPE("VERTEX_SHADER + FRAGMENT_SHADER Creating");
      m_vs_id = glCall(glCreateShader(GL_VERTEX_SHADER));
      m_fs_id = glCall(glCreateShader(GL_FRAGMENT_SHADER));
    }
    {
      REAL_PROFILE_SCOPE("Vertex Shader Compiling");
      glCall(glShaderSource(m_vs_id, 1, &v_cstr, nullptr));
      glCall(glCompileShader(m_vs_id));
      glCall(glGetShaderiv(m_vs_id, GL_COMPILE_STATUS, &status_v));
      if(status_v == GL_FALSE)
      {
        std::string shader_type;
        int mlength;
        glCall(glGetShaderiv(m_vs_id, GL_INFO_LOG_LENGTH, &mlength));
        char* message = (char*)alloca(mlength * sizeof(char));
        glCall(glGetShaderInfoLog(m_vs_id, mlength, &mlength, message));

		    REAL_CORE_ERROR("ERROR Compiling Vertex Shader: {0}", message);
        _status = false;
      }
    }
    {
      REAL_PROFILE_SCOPE("FRAGMENT_SHADER Compiling");
      glCall(glShaderSource(m_fs_id, 1, &f_cstr, nullptr));
      glCall(glCompileShader(m_fs_id));
      glCall(glGetShaderiv(m_fs_id, GL_COMPILE_STATUS, &status_f));
      if(status_f == GL_FALSE)
      {
        int mlength;
        glCall(glGetShaderiv(m_fs_id, GL_INFO_LOG_LENGTH, &mlength));
        char* message = (char*)alloca(mlength * sizeof(char));
        glCall(glGetShaderInfoLog(m_fs_id, mlength, &mlength, message));

		    REAL_CORE_ERROR("ERROR Compiling Fragment Shader: {0}", message);
        _status = false;
      }
    }
    return _status;
  }

  bool OpenGlShader::createProgram()
  {
    REAL_PROFILE_FUNCTION();
    bool _status = true;
    int status;
    m_program_id = glCall(glCreateProgram());
    glCall(glAttachShader(m_program_id, m_vs_id));
    glCall(glAttachShader(m_program_id, m_fs_id));
    glCall(glLinkProgram(m_program_id));
    glCall(glGetProgramiv(m_program_id, GL_LINK_STATUS, &status));
	  if(status == GL_FALSE)
	  {
	  	int mlength;
	  	glCall(glGetProgramiv(m_program_id, GL_INFO_LOG_LENGTH, &mlength));
	  	char* message = (char*) alloca(mlength * sizeof(char));
	  	glCall(glGetProgramInfoLog(m_program_id, mlength, &mlength, message));
	  	m_program_id = 0;
	  	REAL_CORE_ERROR("ERROR Linking Shader Program: {0}", message);
	    _status = false;
    }
    return _status;
  }

  void OpenGlShader::cacheUniforms(std::string vs, std::string fs)
  {
    REAL_PROFILE_FUNCTION();
    std::vector<std::string> vs_toks, fs_toks;
    vs.erase(std::unique(vs.begin(), vs.end(), BothAre<' '>), vs.end());
    fs.erase(std::unique(fs.begin(), fs.end(), BothAre<' '>), fs.end());
   
    vs_toks.reserve(vs.size()/2);
    fs_toks.reserve(fs.size()/2);

    split(vs, vs_toks);
    split(fs, fs_toks);

    findUniformsAndData(vs_toks);
    findUniformsAndData(fs_toks);

    for(auto const& [key, val] : m_uniforms)
    {
      REAL_CORE_TRACE("Uniform: Name: {0} Location: {1} Type: {2}", key, val.location, ShaderDataTypeToString(val.type));
    }
  }

  void OpenGlShader::findUniformsAndData(const std::vector<std::string>& t)
  {
    REAL_PROFILE_FUNCTION();
    for(size_t i = 0; i < t.size(); ++i)
    {
      if(t.at(i) == "uniform")
      {
        uint8_t type_location = 0;
        std::string name;
        UniformData data;
        
        name = t.at(i + 2);
        if(name.find('[') != std::string::npos)
        {
          auto pos = name.find('[');
          name = name.substr(0, pos);
        }
        type_location = 1;

        data.type = getTypeFromString(t.at(i + type_location));
        glCall(auto loc = glGetUniformLocation(m_program_id, name.c_str()));
        data.location = loc;
        m_uniforms[name] = data;
      }
    }
  } 

  void OpenGlShader::setUniform(const std::string& n, const Mat4& v)
  {
    REAL_PROFILE_FUNCTION();
    if(m_uniforms.find(n) != m_uniforms.end())
    {
      glCall(glUniformMatrix4fv(m_uniforms.at(n).location, 1, 0, glm::value_ptr(v)))
    }
  }
  
  void OpenGlShader::setUniform(const std::string& n, const Vec4& v)
  {
    REAL_PROFILE_FUNCTION();
    if(m_uniforms.find(n) != m_uniforms.end())
    {
      glCall(glUniform4fv(m_uniforms.at(n).location, 1, glm::value_ptr(v)))
    }
  }


  void OpenGlShader::setUniform(const std::string& n, int v)
  {
    REAL_PROFILE_FUNCTION();
    if(m_uniforms.find(n) != m_uniforms.end())
    {
      glCall(glUniform1i(m_uniforms.at(n).location, v));
    }
  }

  void OpenGlShader::setUniform(const std::string& n, const Vec2& v)
  {
    REAL_PROFILE_FUNCTION();
    if(m_uniforms.find(n) != m_uniforms.end())
    {
      glCall(glUniform2fv(m_uniforms.at(n).location, 1, glm::value_ptr(v)));
    }
  }

  void OpenGlShader::setUniform(const std::string& n, const int* v, uint32_t c)
  {
#if 0
    std::stringstream vv;
    for(int i = 0; i < c; ++i)
      vv << std::to_string(i) << ", ";
    REAL_CORE_INFO("name: {0} values: {1}, count: {2}", n, vv.str(), c);
#endif
    if(m_uniforms.find(n) != m_uniforms.end())
    {
      glCall(glUniform1iv(m_uniforms.at(n).location, c, v));
    }
  }

  void split(const std::string& str, std::vector<std::string>& cont, char sep) 
  {
    REAL_PROFILE_FUNCTION();
    std::stringstream ss(str);
    std::string token;
    while(ss >> token)
    {
      token.erase(std::remove(token.begin(), token.end(), ';'), token.end());
      cont.push_back(token);
    }
  }
}
