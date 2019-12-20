#pragma once 
#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glm/glm.hpp"


class Shader
{
public:
	// the program ID
	unsigned int ID;

	// constructor reads and builds the shader
	Shader(){}
	Shader(const std::string &vertexPath, const std::string &fragmentPath, const std::string &geometryPath="");
	// use/activate the shader
	void use();
	// utility uniform functions
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setMat4fv(const std::string &name, GLsizei count,GLboolean transpose,const glm::mat4 &mat)const;
	void setVec2(const std::string &name, float x, float y) const;
	void setVec2(const std::string &name, const glm::vec2 &value) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
	void setVec3(const std::string &name, const glm::vec3 &value) const;
	void setUniformIndex(const std::string &name, int value) const;
private:
	void checkCompileErrors(unsigned int shader, std::string type);
};
