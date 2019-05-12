#include <include/window_cursor.h>

namespace core
{
	window_cursor::window_cursor()
		:
			_window(nullptr)
	{};

	window_cursor::window_cursor(window& mwindow)
		:
			_window(&mwindow)
	{};

	window_cursor::~window_cursor()
	{
		_window = nullptr;
	};

	void window_cursor::GetScreenCoords(double& x, double& y)
	{
		glfwGetCursorPos(_window->_window, &x, &y);
	}

	void window_cursor::GetNDCoords(double& x, double& y) // Base OpenGL coords **Normalized Device Coordinates
	{
		glfwGetCursorPos(_window->_window, &x, &y);
		glm::dmat4 cursMat = glm::ortho(0.0f, 
						(float)_window->_windowWidth, 
						(float)_window->_windowHeight, 
						0.0f, 
						-1.0f, 
						1.0f);
		glm::dvec4 tempPos = glm::dvec4(x, y, 0.0f, 0.0f);
		glm::dvec4 cursPos = cursMat * tempPos;
		x = cursPos.x - 1.0f; y = cursPos.y + 1.0f;
	}
}
