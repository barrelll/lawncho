#pragma once
#include <include/GL/glew.h>
#include <include/font.h>
#include <include/window_cursor.h>
#include <include/window_keyboard.h>
#include <include/glsl_program.h>
#include <include/file_buffer.h>
#include <sstream>

namespace sdc
{
	class console
	{
	public:
		console();
		console(const console& rhs);
		~console();

		void LoadScreen(const std::string& fontFileLocation);
		void RenderScreen(core::window_keyboard& keyboard, float pos_x, float pos_y);
		
		// Overload output operator
		void operator<<(std::string rhs);
		void operator<<(char* rhs);

	private:
		float _console_width, _console_height;
		/*** private functions that update in RenderScreen ***/
		void console_controls(unsigned int keypressed);
		void render_inputline();
		void cursor_render();
		// std::vector<char> Log; /* keep a log of everything and print at eof log file
		// maybe just use file_buffer;
		//iom::file_buffer _file_log;
		std::string _input_line;
		int _n_lines;
		GLuint _screenID;
		// Shaders for the console
		core::glsl_program* _font_shader;
		core::glsl_program* _cursor_shader;
		core::glsl_program* _console_shader;
		// Console font type
		iom::font* _font;
	};
}
