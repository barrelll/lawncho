#define GLEW_STATIC
#include <include/GL/glew.h>
#define GLM_FORCE_RADIANS
#include <include/glm/glm.hpp>
#include <include/glsl_program.h>
#include <include/window.h>
#include <include/window_cursor.h>
#include <include/window_keyboard.h>
#include <include/camera.h>
#include <include/texture_cache.h>
#include <include/objloader.h>
#include <include/console.h>
#include <include/font.h>
#include <vector>

#ifdef _WIN32 
std::string shader_location = "C:\\Users\\jharr\\Desktop\\cxx\\spoofydoodle\\core\\shaders\\";
#elif __linux__
std::string shader_location = "";
#endif

/***    Test implementation of library   ***/
// In the future, will have a main class with all necessary types
struct 
{
    core::window window;
    core::glsl_program shader;
    core::camera* camera;
    sdc::console console;
} game;

void InitShaders()
{
	game.shader.CompileShaders(shader_location + "test.vert", shader_location + "test.frag");
	// Binding takes place along side linking, 
	// so if we add anymore attribute locations 
	// consider relinking
	game.shader.AddAttribute("pos");
	game.shader.AddAttribute("vertexUVcoords");
	game.shader.AddAttribute("vertexNormalcoords");
	game.shader.LinkShaders();
}

void GameLoop()
{
	InitShaders();
	/***    Test stuff  ***/
	iom::texture_cache cache;
	cache.LoadTextures();
	std::string a = "../objects/boat2.obj";
	std::string text = "../console/fonts/WHITRABT.TTF";

	iom::objloader obj;
	obj.Rasterize(a);

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = game.shader.GetUniformLocation("MVP");
	core::window_cursor cursor(game.window);
	core::window_keyboard keyb(game.window);
	game.console.LoadScreen(text);

	glm::vec3 camerapos = glm::vec3(4,3,3);
	glm::vec3 cameraori = glm::vec3(0,0,0);

	game.camera = new core::camera(camerapos, cameraori);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	do {
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double i = 0.0;
		i -= 0.00034906585;
		game.camera->Rotate(glm::vec3(0.0, 0.0, 1.0), 0.001);
		glm::mat4 MVP 	= game.camera->getCameraMat();

		if ( i < -6.28318531 )
		{
		    i = 0;
		}

		game.shader.Start();
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		keyb.PollKeyboard();
		cache.GetCachedTexture("../textures/colorMap.bmp")->UseTexture();
		obj.Draw();
		cache.GetCachedTexture("../textures/colorMap.bmp")->UnuseTexture();
		game.shader.End();

		game.console.RenderScreen(keyb, 0.0f, 0.8f);

		game.window.SwapWindow();

	} while (game.window.CloseWindow() == false);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int main(int argc, char* argv[])
{
	game.window.CreateWindowM();

	/***    Draw here   ***/

	GameLoop();

	/***      End       ***/

return 0;
}
