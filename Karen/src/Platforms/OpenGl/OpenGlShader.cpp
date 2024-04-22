#include "pch.h"
#include "Karen/Core/Log.h"
#include "Platforms/OpenGl/OpenGlShader.h"
#include "Karen/Core/Utils/FileLoader.h"
#include "Karen/Core/Core.h"
#include "Platforms/OpenGl/OpenGlCore.h"


namespace Karen
{
  template<char Remove> bool BothAre(char lhs, char rhs)
  {
    return lhs == rhs && lhs == Remove;
  }

  void split(const std::string& str, std::vector<std::string>& cont, char sep = ' '); 
 

  OpenGlShader::OpenGlShader()
  {
  }

  OpenGlShader::OpenGlShader(const std::string& vp, const std::string& fp)
  {
    loadFromFile(vp, fp);
  }

  OpenGlShader::~OpenGlShader()
  {
    KAREN_PROFILE_FUNCTION();
    glCall(glDeleteProgram(m_program_id));
  }

  void OpenGlShader::bind() const
  {
    KAREN_PROFILE_FUNCTION();
    glCall(glUseProgram(m_program_id));
  }

  void OpenGlShader::unbind() const
  {
    KAREN_PROFILE_FUNCTION();
    glCall(glUseProgram(0));
  }

  void OpenGlShader::loadFromFile(const std::string& vp, const std::string& fp)
  {
    std::string vs, fs;
    {
      KAREN_PROFILE_SCOPE("FileLoader::LoadFromFile(VERTEX_SHADER, FRAGMENT_SHADER)");
      vs = FileLoader::LoadFromFile(vp);
      fs = FileLoader::LoadFromFile(fp);
    }
    if(compileShaders(vs, fs))
      if(createProgram())
        cacheUniforms(vs, fs);
      else {KAREN_CORE_WARN("Error Creating Shader Program No Uniforms Will Be Cached");}
    else {KAREN_CORE_WARN("Error Compiling Shader No Uniforms Will Be Cached");}
  }

  bool OpenGlShader::compileShaders(std::string& vs, std::string& fs)
  {
    KAREN_PROFILE_FUNCTION();
    bool _status = true;
    const char *v_cstr = vs.c_str(), *f_cstr = fs.c_str();
    int status_v, status_f;
    {
      KAREN_PROFILE_SCOPE("VERTEX_SHADER + FRAGMENT_SHADER Creating");
      m_vs_id = glCall(glCreateShader(GL_VERTEX_SHADER));
      m_fs_id = glCall(glCreateShader(GL_FRAGMENT_SHADER));
    }
    {
      KAREN_PROFILE_SCOPE("Vertex Shader Compiling");
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

		    KAREN_CORE_ERROR("ERROR Compiling Vertex Shader: {0}", message);
        _status = false;
      }
    }
    {
      KAREN_PROFILE_SCOPE("FRAGMENT_SHADER Compiling");
      glCall(glShaderSource(m_fs_id, 1, &f_cstr, nullptr));
      glCall(glCompileShader(m_fs_id));
      glCall(glGetShaderiv(m_fs_id, GL_COMPILE_STATUS, &status_f));
      if(status_f == GL_FALSE)
      {
        int mlength;
        glCall(glGetShaderiv(m_fs_id, GL_INFO_LOG_LENGTH, &mlength));
        char* message = (char*)alloca(mlength * sizeof(char));
        glCall(glGetShaderInfoLog(m_fs_id, mlength, &mlength, message));

		    KAREN_CORE_ERROR("ERROR Compiling Fragment Shader: {0}", message);
        _status = false;
      }
    }
    return _status;
  }

  bool OpenGlShader::createProgram()
  {
    KAREN_PROFILE_FUNCTION();
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
	  	KAREN_CORE_ERROR("ERROR Linking Shader Program: {0}", message);
	    _status = false;
    }
    return _status;
  }

  void OpenGlShader::cacheUniforms(std::string& vs, std::string& fs)
  {
    KAREN_PROFILE_FUNCTION();
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
      KAREN_CORE_TRACE("Uniform: Name: {0} Location: {1} Type: {2}", key, val.location, ShaderDataTypeToString(val.type));
    }
  }

  void OpenGlShader::findUniformsAndData(const std::vector<std::string>& t)
  {
    KAREN_PROFILE_FUNCTION();
    for(size_t i = 0; i < t.size(); ++i)
    {
      if(t.at(i) == "uniform")
      {
        uint8_t type_location = 0;
        std::string name;
        UniformData data;
        if(GLES)
        {
          name = t.at(i + 3);
          type_location = 2;
        }
        else
        {
          name = t.at(i + 2);
          type_location = 1;
        }
        data.type = getTypeFromString(t.at(i + type_location));
        glCall(auto loc = glGetUniformLocation(m_program_id, name.c_str()));
        data.location = loc;
        m_uniforms[name] = data;
      }
    }
  } 

  void OpenGlShader::setUniform(const std::string& n, const Mat4& v)
  {
    KAREN_PROFILE_FUNCTION();
    if(m_uniforms.find(n) != m_uniforms.end())
    {
      glCall(glUniformMatrix4fv(m_uniforms.at(n).location, 1, 0, glm::value_ptr(v)))
    }
  }
  
  void OpenGlShader::setUniform(const std::string& n, const Vec4& v)
  {
    KAREN_PROFILE_FUNCTION();
    if(m_uniforms.find(n) != m_uniforms.end())
    {
      glCall(glUniform4fv(m_uniforms.at(n).location, 1, glm::value_ptr(v)))
    }
  }


  void OpenGlShader::setUniform(const std::string& n, int v)
  {
    KAREN_PROFILE_FUNCTION();
    if(m_uniforms.find(n) != m_uniforms.end())
    {
      glCall(glUniform1i(m_uniforms.at(n).location, v));
    }
  }

  void OpenGlShader::setUniform(const std::string& n, const Vec2& v)
  {
    KAREN_PROFILE_FUNCTION();
    if(m_uniforms.find(n) != m_uniforms.end())
    {
      glCall(glUniform2fv(m_uniforms.at(n).location, 1, glm::value_ptr(v)));
    }
  }

  void OpenGlShader::setUniform(const std::string& n, const int* v, uint32_t c)
  {
    std::stringstream vv;
    for(int i = 0; i < c; ++i)
      vv << std::to_string(i) << ", ";
    KAREN_CORE_INFO("name: {0} values: {1}, count: {2}", n, vv.str(), c);
    if(m_uniforms.find(n) != m_uniforms.end())
    {
      glCall(glUniform1iv(m_uniforms.at(n).location, c, v));
    }
  }

  void split(const std::string& str, std::vector<std::string>& cont, char sep) 
  {
    KAREN_PROFILE_FUNCTION();
    std::stringstream ss(str);
    std::string token;
    while(ss >> token)
    {
      token.erase(std::remove(token.begin(), token.end(), ';'), token.end());
      cont.push_back(token);
    }
  }
}
