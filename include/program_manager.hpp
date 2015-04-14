#pragma once

#include <GL/glew.h>
#include <fstream>

#include <sstream>
#include <iostream>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class program_manager {
	const std::string shader_location = "shaders/";
	const std::string vertex_shader_postfix = ".vert";
	const std::string geometry_shader_postfix = ".geom";
	const std::string fragment_shader_postfix = ".frag";
	const std::string view = "view";
	const std::string model = "model";
	const std::string projection = "proj";

	const std::string main_shader = "main";
	const std::string post_shader = "post";
	const std::vector<std::string> main_frag_data_names;
	const std::vector<std::string> post_frag_data_names;

	

public:
	GLuint main_shader_program;
	GLuint post_shader_program;

	GLint main_view;
	GLint main_proj;

	~program_manager() {
		//glDeleteProgram(main_shader_program);
		//glDeleteProgram(post_shader_program);
	}

	explicit program_manager() :
		main_frag_data_names({ "frag_color" }),
		post_frag_data_names({ "frag_color" }),
		main_shader_program(create_programm(main_shader, main_frag_data_names, false)),
		main_view(glGetUniformLocation(main_shader_program, view.c_str())),
		main_proj(glGetUniformLocation(main_shader_program, projection.c_str())),
		post_shader_program(create_programm(post_shader, post_frag_data_names, false))
		
	{
		glUseProgram(main_shader_program);
		glm::mat4 proj = glm::perspective(0.9272952f, 1000.0f / 600.0f, 0.1f, 10.0f);
		glUniformMatrix4fv(main_proj, 1, GL_FALSE, glm::value_ptr(proj));
			
	}


	GLuint create_programm(std::string shader_name, std::vector<std::string> frag_data_names, bool use_geometry) {
		GLuint shader_program = glCreateProgram();

		GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		load_shader(vertex_shader, shader_location + shader_name + vertex_shader_postfix);
		glAttachShader(shader_program, vertex_shader);

		GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		load_shader(fragment_shader, shader_location + shader_name + fragment_shader_postfix);
		glAttachShader(shader_program, fragment_shader);

		GLuint geometry_shader;
		if (use_geometry) {
			geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
			load_shader(geometry_shader, shader_location + shader_name + geometry_shader_postfix);
			glAttachShader(shader_program, geometry_shader);
		}

		
		for (unsigned int i = 0; i < frag_data_names.size(); i++) {
			glBindFragDataLocation(shader_program, i, frag_data_names[i].c_str());
		}
		glLinkProgram(shader_program);

		GLint is_linked = GL_FALSE;
		glGetProgramiv(shader_program, GL_LINK_STATUS, &is_linked);
		if (is_linked == GL_FALSE) {
			std::cout << "error while linking shader_program: " + shader_name;
		}

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		if (use_geometry) {
			glDeleteShader(geometry_shader);
		}
		glUseProgram(shader_program);

		return shader_program;
	}

	void load_shader(GLuint &shader, std::string path) {
		std::ifstream stream(path);
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

	


};