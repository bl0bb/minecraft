#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "maths.h"

class Shader {
public:
  unsigned int ID;
  std::string vertex_name;
  std::string fragment_name;

  Shader(std::string vertex_name, std::string fragment_name): vertex_name(vertex_name), fragment_name(fragment_name) {
    std::string vShaderCode;
    std::fstream vertexFile("assets/shaders/" + vertex_name, std::ios::in);
    if (vertexFile.is_open()) {
      std::stringstream buffer;
      buffer << vertexFile.rdbuf();
      vShaderCode = buffer.str();
    } else {
      std::cout << "COULD NOT OPEN SHADER FILE: " + vertex_name << std::endl;
    }

    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    const char *vertex_c_str = vShaderCode.c_str();
    glShaderSource(vertex, 1, &vertex_c_str, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, vertex_name, "VERTEX");


    std::string fShaderCode;
    std::fstream fragmentFile("assets/shaders/" + fragment_name, std::ios::in);
    if (fragmentFile.is_open()) {
      std::stringstream buffer;
      buffer << fragmentFile.rdbuf();
      fShaderCode = buffer.str();
    } else {
      std::cout << "COULD NOT OPEN SHADER FILE: " + fragment_name << std::endl;
    }

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    const char *frag_c_str = fShaderCode.c_str();
    glShaderSource(fragment, 1, &frag_c_str, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, fragment_name, "FRAGMENT");

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "program", "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    printf("Shader '%s, %s' compiled with ID %u \n", vertex_name.c_str(), vertex_name.c_str(), ID);
  }

  void use() {
    glUseProgram(ID);
  }

  GLuint getUniformLocation(std::string name) {
    return glGetUniformLocation(ID, name.c_str());
  }

  void setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
  }

  void setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  }

  void setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
  }

  void setVec2(const std::string &name, const Vec2<f32> &value) const {
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }

  void setVec2(const std::string &name, float x, float y) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
  }

  void setVec3(const std::string &name, const Vec3<f32> &value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }

  void setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
  }

  void setIVec3(const std::string &name, const Vec3<i32> &value) const {
     glUniform3iv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }

//   void setVec4(const std::string &name, const Vec4<f32> &value) const { 
//     glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
//   }

  void setVec4(const std::string &name, float x, float y, float z, float w)  {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
  }

//   void setMat2(const std::string &name, const Mat2<f32> &mat) const {
//     glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
//   }

//   void setMat3(const std::string &name, const Mat3<f32> &mat) const {
//     glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
//   }

  void setMat4(const std::string &name, const f32* mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, mat);
  }

private:
  void checkCompileErrors(GLuint shader, std::string name, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR in: " << type << " in shader '" << name << "'\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        exit(1);
      }
    } else {
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if(!success) {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << " in shader '" << name << "'\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        exit(1);
      }
    }
  }
};

#endif