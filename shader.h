
#ifndef SHADER_H
#define SHADER_H

#include <glad/gl.h> // include glad to get all the required OpenGL headers
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
  

class Shader
{
public:
   // the program ID
   unsigned int ID;
  
   // constructor reads and builds the shader
   Shader(const char* vertexPath, const char* fragmentPath);
   // use/activate the shader
   void use();
   // utility uniform functions
   int getUniform(const std::string &name);
};
  
#endif