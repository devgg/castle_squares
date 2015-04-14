// Headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>
#include <chrono>
#include <thread>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "utility.hpp"
#include "game.hpp"
#include "box.hpp"
#include "program_manager.hpp"
#include "post_processor.hpp"



GLFWwindow* initWindow(GLFWwindow* &window2) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//int count = 0;
	//GLFWmonitor** monitors = glfwGetMonitors(&count);


	GLFWwindow* window = glfwCreateWindow(2000, 1200, "OpenGL", nullptr, nullptr);
	glfwSetWindowPos(window, 6000, 100);
	//window2 = glfwCreateWindow(2000, 1200, "OpenGL", nullptr, window);
	//	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", glfwGetPrimaryMonitor(), nullptr);
	glfwMakeContextCurrent(window);


	glewExperimental = GL_TRUE;
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewInit();
	//glfwMakeContextCurrent(window2);
	//glewInit();


	glGetError();
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_LINE_SMOOTH);
	
	



	return window;
}


void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods, game& game) {
	if (!game.key_callback(key, scancode, action, mods)) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		}
	}
}


int main() {
	GLFWwindow* window2 = nullptr;
	GLFWwindow* window = initWindow(window2);

	
	
	std::vector<box> boxes_collidable;
	//for (int i = 0; i < 10000; i++) {
	//	boxes_collidable.push_back(box(0.f, 0.021f, 0.f, 1.f, 0.02f));
	//}
	boxes_collidable.push_back(box(0.f, 0.021f, 0.f, 1.f, 0.02f));

	boxes_collidable.push_back(box(0.f, 0.091f, 0.f, .5f, 0.02f));
	boxes_collidable.push_back(box(0.f, 0.15f, 0.f, .25f, 0.02f));
	boxes_collidable.push_back(box(0.f, 0.99f, 0.f, 1.f, 0.02f));

	boxes_collidable.push_back(box(0.2f, 0.0f, 0.f, 0.02f, 0.3f));
	boxes_collidable.push_back(box(0.5f, 0.3f, 0.f, 0.1f, 0.1f));


	std::vector<box> boxes_non_collidable;
	boxes_non_collidable.push_back(box(0.f, 0.0f, 0.f, .001f, 1.0f));
	boxes_non_collidable.push_back(box(5.f, 0.6f, -5.f, 0.5f, 0.5f));


	game game(window, world(boxes_collidable), world(boxes_non_collidable));
	game.players.push_back(player(-0.7, 0.0));
	game.players.push_back(player(0.3, 0.8));


	program_manager program_manager;
	post_processor post_processor(2000, 1200);

	double last_time = glfwGetTime();
	double lastTime = glfwGetTime();
	int nbFrames = 0;


	while (!glfwWindowShouldClose(window)) {
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
			// printf and reset timer
			printf("%f ms/frame, %i fps\n", 1000.0 / double(nbFrames), nbFrames);
			nbFrames = 0;
			lastTime += 1.0;
		}

		//while (glfwGetTime() - last_time < 1.f / 210.f) {
		//	std::this_thread::sleep_for(std::chrono::milliseconds(1));
		//}
		last_time = glfwGetTime();

		

		glfwPollEvents();
		game.update();

		
		//glUniformMatrix4fv(program_manager.main_view, 1, GL_FALSE, glm::value_ptr(view));

		//glfwMakeContextCurrent(window2);
		//game.render();
		//glfwSwapBuffers(window2);

		//glfwMakeContextCurrent(window);
		//glUseProgram(program_manager.post_shader_program);
		//game.render();
		post_processor.render(game, program_manager);
		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		


		GLenum error = glGetError();
		if (error != 0) {
			std::cout << error << " " << gluErrorString(error) << std::endl;
		}
		
	}

	//glDeleteProgram(shader_program);
	//glDeleteShader(fragmentShader);
	//glDeleteShader(vertexShader);

	glfwTerminate();
}

/*
double lastTime = glfwGetTime();
int nbFrames = 0;


while (!glfwWindowShouldClose(window)) {
	double currentTime = glfwGetTime();
	nbFrames++;
	if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
		// printf and reset timer
		printf("%f ms/frame, %i fps\n", 1000.0 / double(nbFrames), nbFrames);
		nbFrames = 0;
		lastTime += 1.0;
	}
	*/


/*
template <typename T, precision P>
GLM_FUNC_QUALIFIER tmat4x4<T, P> lookAt
(
tvec3<T, P> const & eye,
tvec3<T, P> const & center,
tvec3<T, P> const & up
) {
	tvec3<T, P> const f(normalize(center - eye));
	tvec3<T, P> const s(normalize(cross(f, up)));
	tvec3<T, P> const u(cross(s, f));

	tmat4x4<T, P> Result(1);
	Result[0][0] = s.x;
	Result[1][0] = s.y;
	Result[2][0] = s.z;
	Result[0][1] = u.x;
	Result[1][1] = u.y;
	Result[2][1] = u.z;
	Result[0][2] = -f.x;
	Result[1][2] = -f.y;
	Result[2][2] = -f.z;
	Result[3][0] = -dot(s, eye);
	Result[3][1] = -dot(u, eye);
	Result[3][2] = dot(f, eye);
	return Result;
}
*/