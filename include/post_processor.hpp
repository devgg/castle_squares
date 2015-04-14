#pragma once

#include <GL/glew.h>
#include "game.hpp"

class post_processor {
	GLuint vao;
	GLuint vbo;

	GLuint fbo;
	GLuint cbo;
public:
	~post_processor() {
		//glDeleteFramebuffers(1, &fbo);
	}

	post_processor(GLsizei  screen_width, GLsizei screen_height) {
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glGenTextures(1, &cbo);
		glBindTexture(GL_TEXTURE_2D, cbo);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_width, screen_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cbo, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "framebuffer creation error";
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		GLfloat vertices[] = {
			-.75f, -.75f, 0.f, 0.f,
			-.75f, .75f, 0.f, 1.f,
			.75f, .75f, 1.f, 1.f,

			-.75f, -.75f, 0.f, 0.f,
			.75f, .75f, 1.f, 1.f,
			.75f, -.75f, 1.f, 0.f
		};

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
	}

	void render(game &game, program_manager &program_manager) {

		//glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glUseProgram(program_manager.main_shader_program);
		glm::mat4 view = glm::lookAt(
			glm::vec3(game.players[0].x_new, game.camera_position_y, 1.f),
			glm::vec3(game.players[0].x_new, game.camera_position_y, 0.f),
			glm::vec3(0.0f, 1.0f, 0.0f)
			);
		glUniformMatrix4fv(program_manager.main_view, 1, GL_FALSE, glm::value_ptr(view));
		game.render();
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glDisable(GL_DEPTH_TEST);
		//glUseProgram(program_manager.post_shader_program);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, cbo);
		//glBindVertexArray(vao);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void render() {
		glBindVertexArray(vao);
	}
};