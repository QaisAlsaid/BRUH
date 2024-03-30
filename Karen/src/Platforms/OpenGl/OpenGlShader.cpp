#include "pch.h"
#include "Platforms/OpenGl/OpenGlShader.h"
#include "Karen/CommanUtils/FileLoader.h"
#include "Karen/Core.h"
#include <glad/glad.h>


namespace Karen
{
  OpenGlShader::OpenGlShader()
  {
  }

  OpenGlShader::OpenGlShader(const std::string& vp, const std::string& fp)
  {
    loadFromFile(vp, fp);
  }

  OpenGlShader::~OpenGlShader()
  {
    glDeleteProgram(m_program_id);
  }

  void OpenGlShader::bind() const
  {
    glUseProgram(m_program_id);
  }

  void OpenGlShader::unbind() const
  {
    glUseProgram(0);
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
    m_vs_id = glCreateShader(GL_VERTEX_SHADER);
    m_fs_id = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(m_vs_id, 1, &v_cstr, nullptr);
    glCompileShader(m_vs_id);
    glGetShaderiv(m_vs_id, GL_COMPILE_STATUS, &status_v);
    if(status_v == GL_FALSE)
    {
      std::string shader_type;
      int mlength;
      glGetShaderiv(m_vs_id, GL_INFO_LOG_LENGTH, &mlength);
      char* message = (char*)alloca(mlength * sizeof(char));
      glGetShaderInfoLog(m_vs_id, mlength, &mlength, message);

		  KAREN_CORE_ERROR("ERROR Compiling Vertex Shader: {0}", message);
    }

    glShaderSource(m_fs_id, 1, &f_cstr, nullptr);
    glCompileShader(m_fs_id);
    glGetShaderiv(m_fs_id, GL_COMPILE_STATUS, &status_f);
    if(status_f == GL_FALSE)
    {
      int mlength;
      glGetShaderiv(m_fs_id, GL_INFO_LOG_LENGTH, &mlength);
      char* message = (char*)alloca(mlength * sizeof(char));
      glGetShaderInfoLog(m_fs_id, mlength, &mlength, message);

		  KAREN_CORE_ERROR("ERROR Compiling Fragment Shader: {0}", message);
    }
  }

  void OpenGlShader::createProgram()
  {
    int status;
    m_program_id = glCreateProgram();
    glAttachShader(m_program_id, m_vs_id);
    glAttachShader(m_program_id, m_fs_id);
    glLinkProgram(m_program_id);
    glGetProgramiv(m_program_id, GL_LINK_STATUS, &status);
	  if(status == GL_FALSE)
	  {
	  	int mlength;
	  	glGetProgramiv(m_program_id, GL_INFO_LOG_LENGTH, &mlength);
	  	char* message = (char*) alloca(mlength * sizeof(char));
	  	glGetProgramInfoLog(m_program_id, mlength, &mlength, message);
	  	m_program_id = 0;
	  	KAREN_CORE_ERROR("ERROR Linking Shader Program: {0}", message);
	  }
  }
}