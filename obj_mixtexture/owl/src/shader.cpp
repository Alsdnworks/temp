#include "shader.h"

ShaderUPtr Shader::CreateFromFile(const std::string& filename, GLenum shaderType) {
  auto shader = std::unique_ptr<Shader>(new Shader());
  //셰이더로드실패로 loadfile이 bool타입의 null포인터를 리턴하면 shader는 삭제됨
  //성공하면 다음 클래스인 shader로 이동함
  if (!shader->LoadFile(filename, shaderType))
    return nullptr;
  return std::move(shader);
}
//소멸자ㅣ mshader가 오류발생시 그걸 제거해준다
Shader::~Shader(){
    if(m_shader){
     glDeleteShader(m_shader);
    }
}

bool Shader::LoadFile(const std::string& filename, GLenum shaderType) {
    //LoadTextFile은 common.h서 정의됨
  auto result = LoadTextFile(filename);
  if (!result.has_value())
    return false;
//코드를 가져온다 이해안되니까 4-3 26분참조
  auto& code = result.value();
  const char* codePtr = code.c_str();
  //int32t=unsignedint
  int32_t codeLength = (int32_t)code.length();

  // create and compile shader

m_shader = glCreateShader(shaderType);
glShaderSource(m_shader, 1, (const GLchar* const*)&codePtr, &codeLength);
//컴파일호출
glCompileShader(m_shader);

  // check compile error
  int success = 0;
  //shader'iv'인티져,벡터(포인터)->포인터로 형식으로 인티져를
  glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[1024];
    glGetShaderInfoLog(m_shader, 1024, nullptr, infoLog);
    SPDLOG_ERROR("failed to compile shader: \"{}\"", filename);
    SPDLOG_ERROR("reason: {}", infoLog);
    //리턴하면 10번째 줄에서 널포인트를 리턴- 메모리 할당해제
    return false;
  }
  return true;
}

