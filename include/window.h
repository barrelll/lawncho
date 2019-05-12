#pragma once
#include <include/GL/glew.h>
#include <include/GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>

namespace core
{
	// Declare window friend, to let window know 
	// window_k and window_c are extern classes and to find them outside 
	// this header
	class window_keyboard;
	class window_cursor;
	class window
	{
	public:
		friend class window_keyboard;
		friend class window_cursor; // get access to window type private 
		window();
		window(const window& rhs);
		~window();

		void        CreateWindowM();
		//void        CreateWindowC();
		void        SwapWindow();
		void        SwapWindow(const window& rhs);
		bool        CloseWindow();
	private:
		/***    Error Callback function for GLFW    ***/
		static void error_callback(int error, const char* description)
		{ fputs(description, stderr + '\n'); glfwTerminate(); }

		std::string _windowName;
		int         _windowWidth, _windowHeight;
		GLFWwindow* _window;
	};
}
