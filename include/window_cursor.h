#pragma once
#include <include/window.h>
#include <include/glm/glm.hpp>
#include <include/glm/gtc/matrix_transform.hpp>

namespace core
{
	class window_cursor
	{
	public:
		window_cursor();
		window_cursor(window& mwindow);
		~window_cursor();

		void GetScreenCoords(double& x, double& y);
		void GetNDCoords(double& x, double& y);
	private:
		window_cursor(const window_cursor& cursor);

		window* _window;
	};
}
