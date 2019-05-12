#pragma once
#include <include/window.h>
#include <queue>

namespace core
{
	class keyevent
	{
	public:
		// State of each key
		enum STATE
		{
			PRESSED,
			RELEASED
		};

		keyevent();
		~keyevent();

		STATE GetKeyState(const int rhs) const;
		void SetKeyState(const STATE keystate, const int rhs) ;
	private:
		uint16_t _nKeys;
		STATE* _keystate;
	};

	class keyserver
	{
	public:
		keyserver();
		~keyserver();
		
		void SetAction(int action);
		void SetKey(uint16_t keycode);
		void GetKey(uint16_t& keycode);
		
	private:
		uint16_t 	_key;
		int		_action;
		keyevent	_event;
	};

	class window_keyboard
	{
	public:
		window_keyboard(window& w);
		~window_keyboard();

		void 	 PollKeyboard();
		uint16_t GetKey();

	private:
		window_keyboard()
		{};

		window_keyboard(const window_keyboard& rhs)
		{};

		static keyserver* kserv;

		// This gets called after key callback during a press event,
		// and waits for a release / key repeat event
		static void glfw_char_callback(GLFWwindow* window, unsigned int key, int mods)
		{
			kserv->SetKey(key);
		}

		// This gets called as soon as a key with a glfw macro is pressed, or released.
		// lower cases will get called with their respective capital letter glfw macro
		static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			kserv->SetAction(action);
			if (key > 256)
				kserv->SetKey(key);
		}

		window* _window;
		std::queue<uint16_t> _buffer;
	};
}
