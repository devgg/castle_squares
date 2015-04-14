#pragma once

#include "box.hpp"



class player : renderable {
public:
	const float horizontal_velocity_max = 1.f;
	const float vertical_velocity_max = 5.f;
	const float gravity = -9.81f;
	const float gravity_exponent = 10.f;
	const std::vector<float> vertices;

	double x_new;
	double y_new;

	float velocity[2];

	box box;

	GLuint vbo;
	GLuint vao;
	GLuint ebo;

	explicit player(double x, double y) : 
		box(x, y, 0.f, 0.06f, 0.06f), 
		x_new(x),
		y_new(y) {
		velocity[0] = 0.f;
		velocity[1] = 0.f;

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(box.vertices), box.vertices, GL_STATIC_DRAW);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(box.elements), box.elements, GL_STATIC_DRAW);
	}


	void render() {
		box.update_position(x_new, y_new);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(box.vertices), box.vertices);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(6), GL_UNSIGNED_INT, nullptr);
	}

	void update_position(float time_delta) {
		velocity[1] = std::fmax(velocity[1] + gravity * time_delta, -vertical_velocity_max);
		y_new = std::fmax(y_new + velocity[1] * time_delta + gravity * pow(time_delta, gravity_exponent) / 2.f, .0f);
	}
};




//	GLuint shots_buffer_object;
//	glGenBuffers(1, &shots_buffer_object);
//	GLuint vao2;
//	glGenVertexArrays(1, &vao2);
//	glBindVertexArray(vao2);
//	glBindBuffer(GL_ARRAY_BUFFER, shots_buffer_object);
//	glBufferData(GL_ARRAY_BUFFER, 1000 * sizeof(GLfloat), &shots_buffer_object, GL_STREAM_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
//	glEnableVertexAttribArray(0);

//		if (shots.size() > 0) {
//			std::vector<GLfloat> shots_vertices;
//			shots_vertices.reserve(shots.size() * 3 + 1);
//			for (shot_object& shot : shots) {
//				shots_vertices.push_back(shot.position().x);
//				shots_vertices.push_back(shot.position().y);
//				shots_vertices.push_back(shot.position().z);
//			}
//
//			glBindVertexArray(vao2);
////			glBindBuffer(GL_ARRAY_BUFFER, shots_buffer_object);
//			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * shots_vertices.size(), shots_vertices.data());
////			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
////			glBufferData(GL_ARRAY_BUFFER, 100 * sizeof(GLfloat), shots_vertices.data(), GL_STREAM_DRAW);
////			glBufferData(GL_ARRAY_BUFFER, 100 * sizeof(GLfloat), nullptr, GL_STREAM_DRAW);
//			glDrawArrays(GL_POINTS, 0, shots.size());
//
//		}