#pragma once
#include <string>
#include <SDL2/SDL.h>
	
class Display {

public:
	Display(int width, int height, const std::string& title);

	void ClearWindow(float red, float green, float blue, float alpha);
	void UpdateWindow();
	void CloseWindow();
	bool IsClosed();
	SDL_Window* Display::GetWindow();

	~Display();

private:
	SDL_Window* window;
	SDL_GLContext glcontext;
	bool isClosed;

};

