#include <include/window.h>

namespace core
{
    window::window()
        :
        	_windowWidth(800),
            	_windowHeight(600),
            	_window(nullptr)
    {};

    // Right now the copy constructor will copy nothing,
    // just creates a new empty window object
    window::window(const window& rhs)
	    :
		    _windowName(rhs._windowName),
		    _windowWidth(rhs._windowWidth),
		    _windowHeight(rhs._windowHeight),
		    _window(rhs._window)
    {};

    window::~window()
    {
        glfwDestroyWindow(_window);
	_window = nullptr;
    };

    void window::CreateWindowM()
    {
//      Setting the error callback function
        glfwSetErrorCallback(error_callback);

//      Initializing GLFW
        if (!glfwInit())
            glfwTerminate();
//      Making OpenGL context version = 4.5
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
//      specifies that we want a context that only supports the new core functionality
        glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
//      Make it forward compatible
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


        _window = glfwCreateWindow(_windowWidth, _windowHeight, "test impl", nullptr, nullptr);
        if (!_window)
            glfwTerminate();

        glfwMakeContextCurrent(_window);

        printf("\33[0;36m***     OpenGL %s     ***\33[0m\n", glGetString(GL_VERSION));

//      Initializing GLEW
        glewExperimental = true;
        GLenum err = glewInit();
        if (GLEW_OK != err)
        {
          /* Problem: glewInit failed, something is seriously wrong. */
          fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        }
    }

/*     void window::CreateWindowC()
    {
//      Initializing GLFW
        if (!glfwInit())
            glfwTerminate();
//      Making OpenGL context version = 4.5
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
//      specifies that we want a context that only supports the new core functionality
        glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
//      Make it forward compatible
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        _windowErrorChild = glfwCreateWindow(250, 100, "Error!", nullptr, nullptr);
	glfwSetWindowPos(_windowErrorChild, 400, 400); 	// Create an algo for handling error position, 
							// should be center of main window ***_window
        if (!_window)
            glfwTerminate();
    } */


    void window::SwapWindow()
    {
            glfwSwapBuffers(_window);
    }

    void window::SwapWindow(const window& rhs)
    {
            glfwSwapBuffers(rhs._window);
    }
 

    bool window::CloseWindow()
    {
        if (glfwWindowShouldClose(_window))
        {
            return true;
        }

        return false;
    }
}
