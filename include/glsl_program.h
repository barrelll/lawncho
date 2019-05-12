#pragma once

#include <include/GL/glew.h>
#include <include/GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

namespace core
{
	class glsl_program
	{
	public:
		glsl_program();
		glsl_program(bool fromString);
		glsl_program(const glsl_program& rhs);
		~glsl_program();

		/* Only 2 shader types are absolutely necessary
		 * just overload this function for more shader types?
		 * or set enums for use outside glsl_program?
		 */
		void    CompileShaders( const std::string& vertFilePath, 
					const std::string& fragFilePath, 
					bool  fromString = 0); // This last bit might be a problem D:
		void    AddAttribute(const std::string& attributeName);
		void    LinkShaders();
		// These two could be shader logs for console 
		// make them return std::vector<char>?
		void 	GetActiveAttrib();
		void 	GetActiveUniforms();
		// Gets a uniform location, 
		// TODO: overload this for sending, makes it easier to read
		GLuint  GetUniformLocation(const std::string& uniformName);
		// Send 4 floats in an array
		void	SendUniform4fv(const std::string& name, float* val);

		void    Start();
		void    End();
	private:
		/***    Private functions!  ***/

		inline void  CompileShader(const std::string& filePath, GLuint shaderID);
		inline void  CompileShaderFromString(const std::string& fileContents, GLuint shaderID);

		/***    Private variables   ***/
		bool 	_fromString;
		GLuint 	_VAO;
		GLuint  _programID;
		GLuint  _vertexShaderID;
		GLuint  _fragmentShaderID;
		int     _numAttributes;
		/***                        ***/
	};
}
