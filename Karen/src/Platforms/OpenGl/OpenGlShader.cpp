#include "Karen/Log.h"
#include "pch.h"
#include "Platforms/OpenGl/OpenGlShader.h"
#include "Karen/CommanUtils/FileLoader.h"
#include "Karen/Core.h"
#include <glad/glad.h>

//HACK:
#define glCall(x) x; \
  glPrintErr(#x, __FILE__, __LINE__);
void glPrintErr(const char* fn, const char* file, int line)
{
  while(auto err = glGetError())
  {
    KAREN_CORE_CRITICAL("OpenGl Error code {0}, File: {1}, Line: {2}, function: {3}",err, file, line, fn);

  }
}
namespace Karen
{
  OpenGlShader::OpenGlShader()
  {
  }

  OpenGlShader::OpenGlShader(const std::string& vp, const std::string& fp)
  {
    glCall(loadFromFile(vp, fp));
  }

  OpenGlShader::~OpenGlShader()
  {
    glCall(glDeleteProgram(m_program_id));
  }

  void OpenGlShader::bind() const
  {
    glCall(glUseProgram(m_program_id));
  }

  void OpenGlShader::unbind() const
  {
    glCall(glUseProgram(0));
  }

  void OpenGlShader::loadFromFile(const std::string& vp, const std::string& fp)
  {
    compileShaders(FileLoader::LoadFromFile(vp), FileLoader::LoadFromFile(fp));
    createProgram();
  }

  void OpenGlShader::compileShaders(const std::string& vs, const std::string& fs)
  {
    KAREN_CORE_TRACE("Vertex: {0}", vs);
    KAREN_CORE_TRACE("Fragment: {0}", fs);
    const char *v_cstr = vs.c_str(), *f_cstr = fs.c_str();
    int status_v, status_f;
    m_vs_id = glCall(glCreateShader(GL_VERTEX_SHADER));
    m_fs_id = glCall(glCreateShader(GL_FRAGMENT_SHADER));

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
    }

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
    }
  }

  void OpenGlShader::createProgram()
  {
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
	  }
  }
}
