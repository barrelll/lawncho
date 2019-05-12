#include <include/window_keyboard.h>

// This keyboard class wouldn't work too well if we wanted game controls,
// it works better as a typing class
// maybe remake this into a typing class
// or have a typing class inherit from this
// maybe just use glfw's getKey(*window, key)

namespace core
{
	/* Literally just here to declare static type kserv in keyboard.h
	 */
	keyserver* window_keyboard::kserv;
	/****************************************************************/

	/***	keyevent	***/
	// This class will log a keyevent, to pressed or released
	// keeps the information in an array and sets it to its respective state enum
	// It prevents key pressed from overflowing characters per frame
	// and allows for a smooth keyboard character input
	keyevent::keyevent()
		:
			_nKeys(348) // 348 is the last of GLFW's key macros
	{
		_keystate = new STATE[_nKeys];
		// Set each state to released, until pressed
		for (uint16_t i = 0; i < _nKeys; i++)
		{
			_keystate[i] = STATE::RELEASED;
		}
	};

	keyevent::~keyevent()
	{
		delete [] _keystate;
		_keystate = nullptr;
		_nKeys = 0;
	};

	// Returns the state of the key at rhs
	keyevent::STATE keyevent::GetKeyState(const int rhs) const
	{ return _keystate[rhs]; }

	// Sets the state of the key at rhs
	void keyevent::SetKeyState(const STATE keystate, const int rhs) 
	{ _keystate[rhs] = keystate; }
	/***			***/

	/***	keyserv		***/
	// Used for static type kserv
	// for static glfw callback functions
	// uses a keyevent type to set the key to correct event type
	// and sets _key to current char pressed, or repeated, if not pressed
	// sets _key back to 0
	keyserver::keyserver()
		:
			_key(0),
			_action(0)
	{};

	keyserver::~keyserver()
	{
		_key = 0;
		_action = 0;
	}

	// Sets the action, used in glfw_key_callback
	// glfw_key_callback gets called first, so this should never
	// be a problem
	void keyserver::SetAction(int action)
	{ _action = action; }

	// Sets the key from the static glfw_char_callback
	void keyserver::SetKey(uint16_t keycode)
	{
		_key = keycode;

		// Probably use switch case here, but I'm too lazy to change it
		if (_action == GLFW_RELEASE)
		{
			_event.SetKeyState(_event.RELEASED, _key);
		}
		else if (_action == GLFW_PRESS)
		{
			_event.SetKeyState(_event.PRESSED, _key);
		}
		else if (_action  == GLFW_REPEAT)
		{
			_event.SetKeyState(_event.PRESSED, _key);
		}
	}

	// Sets the keycode by reference
	void keyserver::GetKey(uint16_t& keycode)
	{
		keycode = (	_event.GetKeyState(_key) == _event.PRESSED 
				&& (uint16_t)_key <= 348) ? _key : 0;
		_event.SetKeyState(_event.RELEASED, keycode);
	}
	/***			***/

	/***	window_keyboard	***/
	window_keyboard::window_keyboard(window& w)
		:
			_window(&w)
	{
		kserv = new keyserver();
		glfwSetCharModsCallback(_window->_window, glfw_char_callback);
		glfwSetKeyCallback(_window->_window, glfw_key_callback);
	};

	window_keyboard::~window_keyboard()
	{
		_window = nullptr;
		delete kserv;
		kserv = nullptr;
	};

	// Polls the keyboard, returns the last pressed key,
	// can't just set and return, need to grab the last key before hand, or it will always be
	// set to 0;
	void window_keyboard::PollKeyboard()
	{
		uint16_t k;
		kserv->GetKey(k);
		if(!_buffer.empty())
			_buffer.pop();
		_buffer.push(k);
	}

	// Returns the key from our queue
	uint16_t window_keyboard::GetKey()
	{
		uint16_t k = (_buffer.empty()) ? 0 : _buffer.back();
		return k;
	}
	/***			***/
}
