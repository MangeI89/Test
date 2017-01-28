#include "Display.h"
#include <GL/glew.h>
#include <iostream>

//The majority of the code in this class belongs to BennyQBD
//https://github.com/BennyQBD/ModernOpenGLTutorial/blob/master/display.cpp

Display::Display(int width, int height, const std::string& title) {

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	isClosed = false;
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	glcontext = SDL_GL_CreateContext(window);

	GLenum status = glewInit();

	if (status != GLEW_OK) {
		std::cout << "Glew init failed" << std::endl;
	}

	glEnable(GL_CLIP_DISTANCE0);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	glEnable(GL_BLEND); //Enable alpha blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set the blend function

}


Display::~Display() {

	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Display::ClearWindow(float red, float green, float blue, float alpha) {

	glClearColor(red, green, blue, alpha);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Display::IsClosed() {
	return isClosed;
}

void Display::CloseWindow() {
	isClosed = true;
}

void Display::UpdateWindow() {
	SDL_GL_SwapWindow(window);
}

SDL_Window* Display::GetWindow() {
	return window;
}