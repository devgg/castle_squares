#include "utility.hpp"
#include <string>
#include <fstream>

#include <sstream>
#include <iostream>
#include <vector>

void compile_shader_from_file(GLuint& shader, const std::string& file_path) {
	std::ifstream stream(file_path);
	std::stringstream buffer;
	buffer << stream.rdbuf();
	stream.close();
	std::string shader_string = buffer.str();
	const char* shader_source = { shader_string.c_str() };
	glShaderSource(shader, 1, &shader_source, nullptr);
	glCompileShader(shader);
	GLint isCompiled = 0;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
		std::cout << errorLog.data();
		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(shader); // Don't leak the shader.
		return;
	}
}
