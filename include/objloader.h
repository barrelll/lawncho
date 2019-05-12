#pragma once
#include <include/file_buffer.h>
#include <include/glm/glm.hpp>
#include <include/GL/glew.h>
#include <include/GLFW/glfw3.h>
#include <iostream>

namespace iom
{
    class objloader
    {
    public:
        objloader();
        ~objloader();

	/*** Loads obj file into usable data ***/
	bool	Rasterize(const std::string& fileName);
    void    Draw();
    private:
	/***	no reason for a copy constructor? ***/
        objloader(const objloader& rhs);

	// Flag types, test whether or not 'face has' verts uvs and norms
	struct FACE { 
		 unsigned int VERT, UV, NORM : 3;
	} _has;

	unsigned int _flags;

	/***	Private functions, for objloader use only	***/
	void			mtlLoader();
	std::vector<int>	spacePos(const std::string& line);
	void			loadTempVert(std::vector<glm::vec3>& tempverts, const std::string& line);
	void			loadTempUV(std::vector<glm::vec2>& tempuv, const std::string& line);
	void 			loadTempNorm(std::vector<glm::vec3>& tempnorm, const std::string& line);
	// Need to make a faces class for this, maybe a nested class?
	void			loadFaces(std::vector<glm::ivec3>& faces, const std::string& line);
	void			hashFaces( const std::vector<glm::ivec3>& faces,
					   const std::vector<glm::vec3>& tempvert,
					   const std::vector<glm::vec2>& tempuv,
					   const std::vector<glm::vec3>& tempnorm );
	
	/*** OpenGL ID's ***/
        GLuint _vertID;
        GLuint _uvID;
        GLuint _normID;

	// The containers OpenGL will use
        std::vector<glm::vec3> _vertices;
        std::vector<glm::vec2> _UV;
        std::vector<glm::vec3> _normals;
    };
}
