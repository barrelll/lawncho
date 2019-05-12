#include <include/console.h>

namespace sdc
{
	console::console()
		:
			_console_width(0.8f),
			_console_height(1.4f),
			_input_line(""),
			_n_lines(0),
			_screenID(0),
			_font_shader(nullptr),
			_cursor_shader(nullptr),
			_console_shader(nullptr),
			_font(nullptr)
	{};

	console::console(const console& rhs)
		:
			_input_line(rhs._input_line),
			_n_lines(rhs._n_lines),
			_screenID(rhs._screenID),
			_font_shader(rhs._font_shader),
			_cursor_shader(rhs._cursor_shader),
			_console_shader(rhs._console_shader),
			_font(rhs._font)
	{};
	
	console::~console()
	{
		if(_screenID)
			glDeleteBuffers(1, &_screenID);
		_input_line.clear();
		_screenID   = 0;
		_n_lines    = 0;
		delete _font_shader;
		delete _cursor_shader;
		delete _console_shader;
		delete _font;
		_font_shader 	= nullptr;
		_cursor_shader  = nullptr;
		_console_shader = nullptr;
		_font 		= nullptr;
	};

	void console::LoadScreen(const std::string& fontFileLocation)
	{
		/* Create shaders, 
		 * each for a different set of objects in console
		 */
		std::string v_cursor_shader   = "#version 450 core\n"
						"layout (location = 0) in vec4 vertex;\n"
						"out vec2 texcoords;\n"
						"void main()\n"
						"{\n"
						"    gl_Position = vec4(vertex.xy, 0.0, 1.0);\n"
						"    texcoords = vertex.zw;\n"
						"}\n";

		std::string v_constext_shader = "#version 450 core\n"
						"layout (location = 0) in vec4 vertex;\n"
						"out vec2 texcoords;\n"
						"void main()\n"
						"{\n"
						"    gl_Position = vec4(vertex.xy, 0.0, 1.0);\n"
						"    texcoords = vertex.zw;\n"
						"}\n";

		std::string f_cursor_shader   = "#version 450 core\n"
						"out vec4 outcolor;\n"
						"void main()\n"
						"{\n"
						"	outcolor = vec4(1, 1, 1, 1.0);\n"
						"}\n";

		std::string f_text_shader     = "#version 450 core\n"
						"out vec4 outcolor;\n"
						"in vec2 texcoords;\n"
						"uniform sampler2D tex;\n"
						"uniform vec4 color;\n"
						"void main()\n"
						"{\n"
						"	outcolor = vec4(1, 1, 1, texture(tex, texcoords).r) * color;\n"
						"}\n";

		std::string f_console_shader  = "#version 450 core\n"
						"out vec4 outcolor;\n"
						"void main()\n"
						"{\n"
						"	outcolor = vec4(0, 0, 0, 0.6);\n"
						"}\n";

		// Set shaders;
		// with compile from string = true
		_font_shader = new core::glsl_program(true);
		_cursor_shader = new core::glsl_program(true);
		_console_shader = new core::glsl_program(true);

		// Compile each shader from string
		_cursor_shader->CompileShaders(v_cursor_shader, f_cursor_shader);
		_cursor_shader->AddAttribute("vertex");
		_cursor_shader->LinkShaders();
		_console_shader->CompileShaders(v_constext_shader, f_console_shader);
		_console_shader->AddAttribute("vertex");
		_console_shader->LinkShaders();
		_font_shader->CompileShaders(v_constext_shader, f_text_shader);
		_font_shader->AddAttribute("vertex");
		_font_shader->LinkShaders();

		// Load our font type
		_font = new iom::font();
		_font->LoadTexture(fontFileLocation);
		
		glGenBuffers(1, &_screenID);	// Generate our screen buffer ID before rendering
	}

	void console::RenderScreen(core::window_keyboard& keyboard, float pos_x, float pos_y)
	{
		// Something similar to this for anchoring to cursor position
		
		/* TODO: create a box/rect type 
		 * this way this ugly bit of code isn't seen and could be reused
		 */
		struct {
			double x, y, s, t;
		} data[6] = {
			{pos_x    ,			pos_y    , 0, 0},
			{pos_x    , 			pos_y - _console_height, 0, 1},
			{pos_x + _console_width, 	pos_y    , 1, 0},
			{pos_x + _console_width, 	pos_y    , 1, 0},
			{pos_x    , 			pos_y - _console_height, 0, 1},
			{pos_x + _console_width, 	pos_y - _console_height, 1, 1}
		};
		glBindBuffer(GL_ARRAY_BUFFER, _screenID);
		glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(double), &data[0], GL_DYNAMIC_DRAW);
		_console_shader->Start();
		glVertexAttribPointer(
		   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		   4,                  // size
		   GL_DOUBLE,          // type
		   GL_FALSE,           // normalized?
		   0,                  // stride
		   (void*)0            // array buffer offset
		);

		// Finally draw our array types
		// and unbind our buffers
		glDrawArrays(GL_TRIANGLES, 0, 6);
		_console_shader->End();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		float sx = 1.0 / 800;
		float sy = 1.0 / 600;
		float white[4] = {1.0, 1.0, 1.0, 1.0};
		unsigned int keypressed = keyboard.GetKey();
		console_controls(keypressed);

		_font_shader->Start();
		_font_shader->SendUniform4fv("color", white);
		_font->RenderText(_input_line, pos_x, pos_y - 1.38f, sx, sy);
		_font_shader->End();

		_cursor_shader->Start();
		cursor_render();
		_cursor_shader->End();
	}

	void console::console_controls(unsigned int keypressed)
	{
		if (keypressed != 0 && keypressed < 256)
		{
			_input_line.push_back(keypressed);
		}
		else if (keypressed == GLFW_KEY_BACKSPACE && _input_line != "")
		{
			_input_line.pop_back();
		}
	}

	void console::cursor_render()
	{}
}
