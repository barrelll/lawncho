#include <include/glsl_program.h>

namespace core
{
	glsl_program::glsl_program()
		:
			_fromString(0),
			_VAO(0),
			_programID(0),
			_vertexShaderID(0),
			_fragmentShaderID(0),
			_numAttributes(0)
	{};

	glsl_program::glsl_program(bool fromString)
		:
			_fromString(fromString),
			_VAO(0),
			_programID(0),
			_vertexShaderID(0),
			_fragmentShaderID(0),
			_numAttributes(0)
	{};

	glsl_program::glsl_program(const glsl_program& rhs)
		:
			_fromString(rhs._fromString),
			_VAO(rhs._VAO),
			_programID(rhs._programID),
			_vertexShaderID(rhs._vertexShaderID),
			_fragmentShaderID(rhs._fragmentShaderID),
			_numAttributes(rhs._numAttributes)
	{};

	glsl_program::~glsl_program()
	{
		glDeleteVertexArrays(1, &_VAO);
		_fromString 		= 0;
		_VAO 			= 0;
		_programID 		= 0;
		_vertexShaderID 	= 0;
		_fragmentShaderID 	= 0;
		_numAttributes 		= 0;
		glDeleteProgram(_programID);
	};

	void glsl_program::CompileShaders(	const std::string& v_filePath,
										const std::string& f_filePath,
										bool fromString)
	{
		_programID = glCreateProgram();
		if (!_programID)
		{
			std::cout << "Program failed to initialize! Exiting." << std::endl;
			glfwTerminate();
		}
		_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (!_vertexShaderID)
		{
			std::cout << "Vertex shader failed to compile! Exiting." << std::endl;
			glfwTerminate();
		}
		_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (!_fragmentShaderID)
		{
			std::cout << "Fragment shader failed to compile! Exiting." << std::endl;
			glfwTerminate();
		}
		if (!_fromString)
		{
			CompileShader(v_filePath, _vertexShaderID);
			CompileShader(f_filePath, _fragmentShaderID);
		}
		else 
		{
			CompileShaderFromString(v_filePath.c_str(), _vertexShaderID);
			CompileShaderFromString(f_filePath.c_str(), _fragmentShaderID);
		}
	}

	void glsl_program::CompileShader(const std::string& filePath, GLuint shaderID)
	{
		std::ifstream file(filePath);

		if(file.fail())
		{
			std::cout << "Failed to open shader file, exiting." << std::endl;
			glfwTerminate();
		}

		std::string fileContents = "";
		std::string line;

		while (std::getline(file, line))
		{
			fileContents += line + "\n";
		}

		const char* contentsPtr = fileContents.c_str();
		glShaderSource(shaderID, 1, &contentsPtr, nullptr);
		glCompileShader(shaderID);
		std::cout << shaderID << std::endl;

		/*Do some error checking!*/

		GLint success = 0;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			std::cout << "Problem compiling shaders, exiting." << std::endl;
			GLint maxLength = 0;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the null character!
			std::vector<char> errorLog(maxLength);
			GLsizei written;
			glGetShaderInfoLog(shaderID, maxLength, &written, &errorLog[0]);

			std::printf("%s", &errorLog[0]);

			errorLog.clear();
			glDeleteShader(shaderID);

			return;
		}
		file.close();
	}

	void glsl_program::CompileShaderFromString(const std::string& fileContents, GLuint shaderID)
	{
		const char* filecont = fileContents.c_str();
		glShaderSource(shaderID, 1, &filecont, nullptr);
		glCompileShader(shaderID);

		/*Do some error checking!*/

		GLint success = 0;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			std::cout << "Problem compiling shaders, exiting." << std::endl;
			GLint maxLength = 0;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the null character!
			std::vector<char> errorLog(maxLength);
			GLsizei written;
			glGetShaderInfoLog(shaderID, maxLength, &written, &errorLog[0]);

			std::printf("%s", &errorLog[0]);

			errorLog.clear();
			glDeleteShader(shaderID);

			return;
		}
	}

	void glsl_program::LinkShaders()
	{
		// Now comes time to link shaders together :D
		glAttachShader(_programID, _vertexShaderID);
		glAttachShader(_programID, _fragmentShaderID);


		glLinkProgram(_programID);
		// Done, super simple stuff! 
		// now for error checking!
		GLint success = 0;
		glGetProgramiv(_programID, GL_LINK_STATUS, ( int * )&success);
		if (success == GL_FALSE)
		{
			std::cout << "Problem linking shaders to program, exiting." << std::endl;
			GLint maxLength = 0;
			glGetShaderiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the null character!
			std::vector<char> errorLog(maxLength);

			glGetShaderInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

			std::printf("%s", &errorLog[0]);

			// We don't need this crap no more
			glDeleteProgram(_programID);
			glDeleteShader(_vertexShaderID);
			glDeleteShader(_fragmentShaderID);
			return;
		}
		// always detach after successful linking
		glDetachShader(_programID, _vertexShaderID);
		glDetachShader(_programID, _fragmentShaderID);
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);
	}

	void glsl_program::AddAttribute(const std::string& attributeName)
	{
		glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());
	}

	void glsl_program::GetActiveAttrib()
	{
		int nAttribs = 0; int maxLength = 0;
		glGetProgramiv(_programID, GL_ACTIVE_ATTRIBUTES, &nAttribs);
		glGetProgramiv(_programID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);
		std::vector<char> Log(maxLength);

		GLint written, size, location;
		GLenum type;
		printf(" Index | Name\n");
		printf("------------------------------------------------\n");
		for( int i = 0; i < nAttribs; i++ ) 
		{
			glGetActiveAttrib( _programID, i, maxLength, &written, &size, &type, &Log[0] );
			location = glGetAttribLocation(_programID, &Log[0]);
			printf(" %-5d | %s\n",location, &Log[0]);
		}
		Log.clear();
	}

	void glsl_program::GetActiveUniforms()
	{
		int nUniforms = 0; int maxLength = 0;
		glGetProgramiv(_programID, GL_ACTIVE_UNIFORMS, &nUniforms);
		glGetProgramiv(_programID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);
		std::vector<char> Log(maxLength);

		GLint written, location;
		printf(" Index | Name\n");
		printf("------------------------------------------------\n");
		for( int i = 0; i < nUniforms; i++ ) 
		{
			glGetActiveUniformName( _programID, i, maxLength, &written, &Log[0] );
			location = glGetUniformLocation(_programID, &Log[0]);
			printf(" %-5d | %s\n",location, &Log[0]);
		}
		Log.clear();
	}

	GLuint glsl_program::GetUniformLocation(const std::string& uniformName)
	{
		GLuint location = glGetUniformLocation(_programID, uniformName.c_str());
		if (location == GL_INVALID_INDEX)
		{
			std::cout << "Uniform location not found!" << std::endl;
		}
		return location;
	}

	void glsl_program::SendUniform4fv(const std::string& name, float* val)
	{
		/* Need to find a way to preload these;
		 * so we don't get stuck loading these everytime
		 */
		GLuint location = glGetUniformLocation(_programID, name.c_str());
		if (location == GL_INVALID_INDEX)
		{
			std::cout << "Uniform location not found!" << std::endl;
		}
		glUniform4fv(location, 1, val);
	}

	// TODO: Add a flag to use shader layout instead of binding before hand, 
	// use glGetActiveUniform <- (provides name, size, and type) or glGetActiveUniformName <- (provides only a name)
	void glsl_program::Start()
	{
		if(!_VAO)
			glGenVertexArrays(1, &_VAO);
		glBindVertexArray(_VAO);
		glUseProgram(_programID);
		for (int i = 0; i < _numAttributes; i++)
		{
			glEnableVertexAttribArray(i);
		}
	}

	void glsl_program::End()
	{
		glUseProgram(0);
		for (int i = 0; i < _numAttributes; i++)
		{
			glDisableVertexAttribArray(i);
		}
		glBindVertexArray(0);
	}
}
