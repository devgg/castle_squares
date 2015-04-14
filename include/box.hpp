#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <array>

class box {
	

public:
	double x;
	double y;
	double width;
	double height;
	double half_width;
	double half_height;

	GLfloat vertices[12];
	GLuint elements[6];
	

	box(double x, double y, double z, double width, double height) :
		x(x),
		y(y),
		width(width),
		height(height),
		half_width(width / 2.f),
		half_height(height / 2.f) {

		vertices[0] = (GLfloat) x;
		vertices[1] = (GLfloat) (y + height);
		vertices[2] = (GLfloat) z;
		vertices[3] = (GLfloat) (x + width);
		vertices[4] = (GLfloat) (y + height);
		vertices[5] = (GLfloat) z;
		vertices[6] = (GLfloat) x;
		vertices[7] = (GLfloat) y;
		vertices[8] = (GLfloat) z;
		vertices[9] = (GLfloat) (x + width);
		vertices[10] = (GLfloat) y;
		vertices[11] = (GLfloat) z;

		elements[0] = 0;
		elements[1] = 1;
		elements[2] = 2;
		elements[3] = 1;
		elements[4] = 3;
		elements[5] = 2;
	}

	void update_position(double x_new, double y_new) {
		x = x_new;
		y = y_new;
		vertices[0] = (GLfloat) x;
		vertices[1] = (GLfloat) (y + height);
		vertices[3] = (GLfloat) (x + width);
		vertices[4] = (GLfloat) (y + height);
		vertices[6] = (GLfloat) x;
		vertices[7] = (GLfloat) y;
		vertices[9] = (GLfloat) (x + width);
		vertices[10] = (GLfloat) y;
	}
};
