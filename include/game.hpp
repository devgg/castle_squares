#pragma once

#include <GLFW/glfw3.h>

#include <vector>

#include "world.hpp"
#include "player.hpp"

#define EPSILON 0.000001f

class game : renderable {

	

public:
	world world_collidable;
	world world_non_collidable;
	std::vector<player> players;
	float camera_position_x = 0.f;
	float camera_position_y = 0.5f;

	float update_time;
	bool right_pressed = false;
	bool left_pressed = false;

	bool right_pressed_p2 = false;
	bool left_pressed_p2 = false;



	explicit game(GLFWwindow* window, world world_collidable, world world_non_collidable) :
		world_collidable(world_collidable),
		world_non_collidable(world_non_collidable),
		update_time(static_cast<float>(glfwGetTime())) {
		glfwSetWindowUserPointer(window, this);
		glfwSetKeyCallback(window, key_callback_tmp);
	}

	void render() override {
		world_collidable.render();
		world_non_collidable.render();
		for (player &player : players) {
			player.render();
		}
	}



	void update() {
		float current_update_time = (float) glfwGetTime();
		float time_delta = current_update_time - update_time;

		for (player &p : players) {
			if (&p == &players[0] && right_pressed != left_pressed) {
				players[0].x_new += (right_pressed ? 1.0f : -1.0f) * time_delta * 0.5;
			}
			if (&p == &players[1] && right_pressed_p2 != left_pressed_p2) {
				players[1].x_new += (right_pressed_p2 ? 1.0f : -1.0f) * time_delta * 0.5;
			}



			for (box &b : world_collidable.boxes) {
				if (abs(p.x_new + p.box.half_width - b.x - b.half_width) * 2 < (p.box.width + b.width) && 
					abs(p.y_new + p.box.half_height - b.y - b.half_height) * 2 < (p.box.height + b.height)) {
					resolve_collision(p.box.x, p.box.width, b.x, b.width, p.x_new);
				}
			}

			for (player &p2 : players) {
				if (&p != &p2) {
					if (abs(p.x_new + p.box.half_width - p2.x_new - p2.box.half_width) * 2 < (p.box.width + p2.box.width) &&
						abs(p.y_new + p.box.half_height - p2.y_new - p2.box.half_height) * 2 < (p.box.height + p2.box.height)) {
						resolve_collision(p.box.x, p.box.width, p2.x_new, p2.box.width, p.x_new);
					}
				}
			}

			p.update_position(time_delta);

			for (box &b : world_collidable.boxes) {
				if (abs(p.x_new + p.box.half_width - b.x - b.half_width) * 2 < (p.box.width + b.width) &&
					abs(p.y_new + p.box.half_height - b.y - b.half_height) * 2 < (p.box.height + b.height)) {
					resolve_collision(p.box.y, p.box.height, b.y, b.height, p.y_new);
					p.velocity[1] = 0.f;
				}
			}

			for (player &p2 : players) {
				if (&p != &p2) {
					if (abs(p.x_new + p.box.half_width - p2.x_new - p2.box.half_width) * 2 < (p.box.width + p2.box.width) &&
						abs(p.y_new + p.box.half_height - p2.y_new - p2.box.half_height) * 2 < (p.box.height + p2.box.height)) {
						resolve_collision(p.box.y, p.box.height, p2.y_new, p2.box.height, p.y_new);
						p.velocity[1] = 0.f;
					}
				}
			}
		}
		camera_position_x = (float) players[0].x_new;
		update_time = current_update_time;
	}
	

	int resolve_collision(double b1_pos, double b1_size, double b2_pos, double b2_size, double& new_pos) {

  		double v = new_pos - b1_pos;
		double invEntry, invExit;
		if (v > 0.0f) {
			invEntry = b2_pos - (b1_pos + b1_size);
			invExit = (b2_pos + b2_size) - b1_pos;
		} else {
			invEntry = (b2_pos + b2_size) - b1_pos;
			invExit = b2_pos - (b1_pos + b1_size);
		}

		double entry = invEntry / v;
		double exit = invExit / v;

		if (invEntry < 0.0f) {
			new_pos = fmin(b1_pos, b2_pos + b2_size + EPSILON);
			return 1;
		} else {
			new_pos = fmax(b1_pos, b2_pos - b1_size - EPSILON);
			return -1;
		}


	}


	void resolve_collision(player& p, box& b) {
		box b1 = box(p.box.x, p.box.y, 0.f, p.box.width, p.box.height);
		box b2 = b;

		double vx = p.x_new - p.box.x;
		double vy = p.y_new - p.box.y;


		double xInvEntry, yInvEntry;
		double xInvExit, yInvExit;

		// find the distance between the objects on the near and far sides for both x and y
		if (vx > 0.0f) {
			xInvEntry = b2.x - (b1.x + b1.width);
			xInvExit = (b2.x + b2.width) - b1.x;
		} else {
			xInvEntry = (b2.x + b2.width) - b1.x;
			xInvExit = b2.x - (b1.x + b1.width);
		}

		if (vy > 0.0f) {
			yInvEntry = b2.y - (b1.y + b1.height);
			yInvExit = (b2.y + b2.height) - b1.y;
		} else {
			yInvEntry = (b2.y + b2.height) - b1.y;
			yInvExit = b2.y - (b1.y + b1.height);
		}

		// find time of collision and time of leaving for each axis (if statement is to prevent divide by zero)
		double xEntry, yEntry;
		double xExit, yExit;

		if (vx == 0.0f) {
			xEntry = -std::numeric_limits<float>::infinity();
			xExit = std::numeric_limits<float>::infinity();
		} else {
			xEntry = xInvEntry / vx;
			xExit = xInvExit / vx;
		}

		if (vy == 0.0f) {
			yEntry = -std::numeric_limits<float>::infinity();
			yExit = std::numeric_limits<float>::infinity();
		} else {
			yEntry = yInvEntry / vy;
			yExit = yInvExit / vy;
		}

		if (xEntry > 1.0f) xEntry = -std::numeric_limits<float>::infinity();
		if (yEntry > 1.0f) yEntry = -std::numeric_limits<float>::infinity();

		// find the earliest/latest times of collision
		double entryTime = std::max(xEntry, yEntry);
		double exitTime = std::min(xExit, yExit);

		//xEntry = -xEntry;
		//yEntry = -yEntry;

		// if there was no collision
		//if (entryTime > exitTime || xEntry < 0.0f && yEntry < 0.0f || xEntry > 1.0f || yEntry > 1.0f) {
		//	normal_x = 0.0f;
		//	normal_y = 0.0f;
		//	std::cout << "_";
		//return 1.0f;
		//} else // if there was a collision
		{
			// calculate normal of collided surface
			if (xEntry > yEntry) {
				if (xInvEntry < 0.0f) {
 					p.x_new = b.x + b.width + EPSILON;
					//std::cout << "right";

				} else {
					p.x_new = b.x - p.box.width - EPSILON;
					//old_position_x = p.position[0];
					//std::cout << "left";
				}
			} else {
				if (yInvEntry <= 0.0f) {
					p.y_new = b.y + b.height + EPSILON;
					if (p.velocity[1] < 0) {
						p.velocity[1] = 0.f;
					}
					//std::cout << "top";
				} else {
					p.y_new = b.y - p.box.height - EPSILON;
					p.velocity[1] = 0.f;
					//std::cout << "bottom";
				}
			}

			// return the time of collision
			//return entryTime;
		}
	}



	bool key_callback(int key, int scancode, int action, int mods) {
		update();
		switch (key) {
		case GLFW_KEY_A:
			left_pressed = (action != GLFW_RELEASE);
			return true;
		case GLFW_KEY_D:
			right_pressed = (action != GLFW_RELEASE);
			return true;
		case GLFW_KEY_SPACE:
			if (action == GLFW_PRESS) {
				players[0].velocity[1] = 3.0f;
			}
			return true;

		case GLFW_KEY_LEFT:
			left_pressed_p2 = (action != GLFW_RELEASE);
			return true;
		case GLFW_KEY_RIGHT:
			right_pressed_p2 = (action != GLFW_RELEASE);
			return true;
		case GLFW_KEY_L:
			if (action == GLFW_PRESS) {
				players[1].velocity[1] = 3.0f;
			}
			return true;
		}
		return false;
	}

	static void game::key_callback_tmp(GLFWwindow* window, int key, int scancode, int action, int mods) {
		static_cast<game*>(glfwGetWindowUserPointer(window))->key_callback(key, scancode, action, mods);
	}
};