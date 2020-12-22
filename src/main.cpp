#include "app.hpp"

int main(void)
{
	const int screenWidth = 1280;
    const int screenHeight = 720;
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	Application::App app(screenWidth, screenHeight);
	app.Start();

	return 0;
}