#pragma once

#include <GL/glew.h>
#include <vector>

#include "renderable.hpp"
#include "box.hpp"


class world : renderable {
public:
	std::vector<box> boxes;
	std::vector<GLfloat> vertices;
	std::vector<GLuint> elements;

	GLuint vbo;
	GLuint vao;
	GLuint ebo;
	
	


	~world() {
		//		glDeleteBuffers(1, &vbo);
		//		glDeleteBuffers(1, &ebo);
		//		glDeleteVertexArrays(1, &vao);
		//		std::cout << "~mesh" << std::endl;
	}

	world() {};

	//explicit world(std::vector<box> boxes, std::vector<GLfloat> vertices, std::vector<GLuint> elements) :
	explicit world(std::vector<box> boxes) :
		boxes(boxes) {
		for (box b : boxes) {
			std::copy(b.vertices, b.vertices + sizeof(b.vertices) / sizeof(GLfloat), back_inserter(vertices));
			std::copy(b.elements, b.elements + sizeof(b.elements) / sizeof(GLuint), back_inserter(elements));
		}
		for (unsigned int i = 0; i < boxes.size(); i++) {
			for (int j = 0; j < 6; j++) {
				elements.push_back(boxes[i].elements[j] + 4 * i);
			}
		}

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(GLuint), elements.data(), GL_STATIC_DRAW);
	};

	void render() override {
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(elements.size()), GL_UNSIGNED_INT, nullptr);
	}
};