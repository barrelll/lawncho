#include <include/objloader.h>

// Flags for each obj class, to check if it _has all 3, 
// and route to correct face number ##
// 
// *** Try reserving memory beforehand to increase loading speed
namespace iom
{
	objloader::objloader()
		:
			_flags(0),
			_vertID(0),
			_normID(0),
			_vertices(0),
			_UV(0),
			_normals(0)
	{
		_has.VERT = 0;
		_has.UV   = 0;
		_has.NORM = 0;
	};

	objloader::~objloader()
	{
		if (_vertID)
		    glDeleteBuffers(1, &_vertID);
		if(_uvID)
		    glDeleteBuffers(1, &_uvID);
		if(_normID)
		    glDeleteBuffers(1, &_normID);
		if(!_vertices.empty())
		    _vertices.clear();
		if(!_normals.empty())
		    _normals.clear();
		if(!_UV.empty())
		    _UV.clear();
		if(_flags)
			_flags = 0;
		if(_has.VERT || _has.UV || _has.NORM)
		{
			_has.VERT = 0; _has.UV = 0; _has.NORM = 0;
		}
	};

	// Something to mimic glm::vec3 for faces;
	// 'maybe' TODO: in spoofydoodle, make a class that can return alternate between
	// glm::vec3 or glm::vec2 or just int, based on flag variables
	// for now mimic a vec2 because cube.obj only _has
	// vertices and normals

	// loadFace helper function
	std::vector<int> objloader::spacePos(const std::string& line)
	{
		// TODO:
		// Might be better to use a linked-list in this case
		// SHOULD only be 3 spaces that we need
		// it is also possible that there will only be 3 vertice cases *** on an individual or all lines
		// resulting in only 2 spaces, need to test for that, and 
		// allocate memory appropriately
		int a = 0;
		int spaces = 0;
		std::vector<int> list;
		for (unsigned int i = 0; i < line.length() - 1; i++)
		{
			if(line.at(i) == ' ')
			{
				spaces += 1;
			}
		}

		for (unsigned int i = 0; i < line.length() - 1; i++)
		{
			if(line.at(i) == ' ' && a < spaces)
			{
				list.push_back(i);
				a++;
			}
			else if(line.at(i) == ' ' && a > spaces)
			{
				break;
			}
		}
		return list;
	}

	// Loads the vertices from each line
	void objloader::loadTempVert(std::vector<glm::vec3>& verts, const std::string& line)
	{
		// Remove the header from the file.line
		std::string removedHeader = line.substr(2);

		glm::vec3 val;

		std::sscanf(removedHeader.c_str(), "%f %f %f", &val.x, &val.y, &val.z);
		verts.push_back(val);
	
		if (_has.VERT == 0)
			_has.VERT = 0x01;
	}

	void objloader::loadTempUV(std::vector<glm::vec2>& uv, const std::string& line)
	{
		// Remove the header from the file.line
		std::string removedHeader = line.substr(3);

		glm::vec2 val;
		std::sscanf(removedHeader.c_str(), "%f %f", &val.x, &val.y);
		uv.push_back(val);

		if (_has.UV == 0)
			_has.UV = 0x02;
	}

	void objloader::loadTempNorm(std::vector<glm::vec3>& norm, const std::string& line)
	{
		// Remove the header from the file.line
		std::string removedHeader = line.substr(3);

		glm::vec3 val;
		std::sscanf(removedHeader.c_str(), "%f %f %f", &val.x, &val.y, &val.z);
		norm.push_back(val);

		if (_has.NORM == 0)
			_has.NORM = 0x04;
	}

	// TODO: Need to test for 3 or 4 sets of faces in spacePos, 
	void objloader::loadFaces(std::vector<glm::ivec3>& faces, const std::string& line)
	{
		// Remove the header from the file.line
		std::string removedHeader = line.substr(2);
		// Finds pos of 3 spaces in face values
		std::vector<int> a = spacePos(removedHeader);
		// Create vector indices for each case

		// A bit more complicated, use faces to correctly indicate
		// which indices should be used first
		std::string* newLine = new std::string[a.size() + 1];
		for (unsigned int i = 0; i <= a.size(); i++)
		{
			if(i == 0)
				newLine[i] = removedHeader.substr(0, a[0]);
			else if (i == a.size())
				newLine[i] = removedHeader.substr(removedHeader.find_last_of(' ') + 1, 
						removedHeader.length());
			else
				newLine[i] = removedHeader.substr(a[i - 1] + 1, a[i] - a[i - 1] );
		}
		
		if (_has.VERT || _has.UV || _has.NORM)
		{
			_flags = _has.VERT | _has.UV | _has.NORM;
			switch (_flags)
			{
			case 1: // Just vertices
				for (unsigned int i = 0; i <= a.size(); i++)
				{
					faces.push_back(glm::ivec3{});
					std::sscanf(newLine[i].c_str(), "%d", &faces.back().x);
				}
				break;
			case 3: // Vertices and UV's
				for (unsigned int i = 0; i <= a.size(); i++)
				{
					faces.push_back(glm::ivec3{});
					std::sscanf(newLine[i].c_str(), "%d/%d", &faces.back().x,
							&faces.back().y);
				}
				break;
			case 5: // Vertices and normals, another likely case
				for (unsigned int i = 0; i <= a.size(); i++)
				{
					faces.push_back(glm::ivec3{});
					std::sscanf(newLine[i].c_str(), "%d//%d", &faces.back().x,
							&faces.back().z);
				}
				break;
			case 7: // All 3, most likely case
				for (unsigned int i = 0; i <= a.size(); i++)
				{
					faces.push_back(glm::ivec3{});
					std::sscanf(newLine[i].c_str(), "%d/%d/%d", &faces.back().x,
							&faces.back().y, &faces.back().z);
				}
				break;
			default:
				break;
			}
		}
		delete [] newLine;
		newLine = nullptr;
	}

	// Consider using uvec3 for unsigned int
	void objloader::hashFaces(const std::vector<glm::ivec3>& faces,
				const std::vector<glm::vec3>& tempvert,
				const std::vector<glm::vec2>& tempuv,
				const std::vector<glm::vec3>& tempnorm)
	{
		// Finally _hashing faces, 
		// Push verts, uvs, and norms, from the index incidicated by face value - 1
		// to the back of the container we store them in
		// in this case, it's easy to do with vectors
		switch (_flags)
		{
		case 1: // Just vertices
			for (unsigned int i = 0; i < faces.size(); i++)
			{
				// vertices are x
				// uv is y
				// normals are z 
				int Index;

				Index = faces.at(i).x - 1;
				if (Index == -1)
				{
					i++;
					continue;
				}
				_vertices.push_back(tempvert.at(Index));
			}

			break;
		case 3: // Vertices and UV's
			for (unsigned int i = 0; i < faces.size(); i++)
			{
				// vertices are x
				// uv is y
				// normals are z 
				int Index;

				Index = faces.at(i).x - 1;
				if (Index == -1)
				{
					i++;
					continue;
				}
				_vertices.push_back(tempvert.at(Index));

				Index = faces.at(i).y - 1;
				if (Index == -1)
				{
					i++;
					continue;
				}
				_UV.push_back(tempuv.at(Index));
			}
			break;
		case 5: // Vertices and normals, another likely case
			for (unsigned int i = 0; i < faces.size(); i++)
			{
				// vertices are x
				// uv is y
				// normals are z 
				int Index;

				Index = faces.at(i).x - 1;
				if (Index == -1)
				{
					i++;
					continue;
				}
				_vertices.push_back(tempvert.at(Index));

				Index = faces.at(i).z - 1;
				if (Index == -1)
				{
					i++;
					continue;
				}
				_normals.push_back(tempnorm.at(Index));
			}
			break;
		case 7: // All 3, most likely case
			for (unsigned int i = 0; i < faces.size(); i++)
			{
				// vertices are x
				// uv is y
				// normals are z 
				int Index;

				Index = faces.at(i).x - 1;
				if (Index == -1)
				{
					i++;
					continue;
				}
				_vertices.push_back(tempvert.at(Index));

				Index = faces.at(i).y - 1;
				if (Index == -1)
				{
					i++;
					continue;
				}
				_UV.push_back(tempuv.at(Index));

				Index = faces.at(i).z - 1;
				if (Index == -1)
				{
					i++;
					continue;
				}
				_normals.push_back(tempnorm.at(Index));
			}
			break;
		default:
			std::cout << "This format can't be read just yet\n";
			break;
		}
	}

	void objloader::mtlLoader()
	{}

	bool objloader::Rasterize(const std::string& fileName)
	{
		// Temporary vectors to store information,
		// will be rearranged later ***if there are faces

		// Might not even need to create temps, if we just rearrange 
		// them and pass by reference
		std::vector<glm::vec3>  tempverts, tempnorms;
		std::vector<glm::vec2>  tempuvs;
		std::vector< glm::ivec3 > faces;

		// Could definitely do more checking to see if it's
		// a object file, this will do for now
		int a = fileName.rfind(".obj");
		if(a == -1)
			return false;

		// Load buffer from file
		
		iom::file_buffer _file(fileName);
		_file.LoadFileBuffer();
		auto file = _file.GetFileBuffer();

		// iterate over file, test for v, vn, vt, and f
		// will always be 2 uv's
		// different kinds of face types include:
		// f v/vt v/vt v/vt		vertices and uv's
		// f v//vn v//vn v//vn		vertices and normals
		// f v/vt/vn v/vt/vn v/vt/vn	vertices, uv's, and normals
		// f v v v			just vertices
		
		unsigned int i = 0;
		while ( i != file.size() )
		{
			std::string line = file.at(i);
			// Check for comments
			// Need to worry about order of operations here, 
			/***vertices come before uv's and normals and so on*/
			if (line.at(0) == '#') {
				i++;
				continue;
			}

			else if (line.at(0) == 'v' && line.at(1) == ' ')
				loadTempVert(tempverts, line);
			else if (line.at(0) == 'v' && line.at(1) == 't')
				loadTempUV(tempuvs, line);
			else if (line.at(0) == 'v' && line.at(1) == 'n')
				loadTempNorm(tempnorms, line);
			else if (line.at(0) == 'f' && line.at(1) == ' ')
				loadFaces(faces, line);
			else
			{}
			i++;
		}
		// After faces are succefully loaded time to _hash them
		// maybe test if obj _has faces first ?
		hashFaces(faces, tempverts, tempuvs, tempnorms);

		if (!_vertices.empty()) {
			glGenBuffers(1, &_vertID);
			glBindBuffer(GL_ARRAY_BUFFER, _vertID);
			glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(glm::vec3), 
					&_vertices.front(), GL_STATIC_DRAW);
		}

		if (!_UV.empty()) {
			glGenBuffers(1, &_uvID);
			glBindBuffer(GL_ARRAY_BUFFER, _uvID);
			glBufferData(GL_ARRAY_BUFFER, _UV.size() * sizeof(glm::vec2), 
					&_UV.front(), GL_STATIC_DRAW);
		}

		if (!_normals.empty()) {
			glGenBuffers(1, &_normID);
			glBindBuffer(GL_ARRAY_BUFFER, _normID);
			glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(glm::vec3), 
					&_normals.front(), GL_STATIC_DRAW);
		}
		/***    Unbind everything!  ***/
		tempverts.clear();
		tempnorms.clear();
		tempuvs.clear();
		faces.clear();
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return true;
	}

	void objloader::Draw()
	{
		// 1rst attribute buffer : vertices
		glBindBuffer(GL_ARRAY_BUFFER, _vertID);
		glVertexAttribPointer(
		   0,                  // attribute 0. Must match the layout in the shader.
		   3,                  // size
		   GL_FLOAT,           // type
		   GL_FALSE,           // normalized?
		   0,                  // stride
		   (void*)0            // array buffer offset
		);

		glBindBuffer(GL_ARRAY_BUFFER, _uvID);
		glVertexAttribPointer(
		   1,                  // attribute 1. Must match the layout in the shader.
		   2,                  // size
		   GL_FLOAT,           // type
		   GL_FALSE,           // normalized?
		   0,                  // stride
		   (void*)0            // array buffer offset
		);

		/*glBindBuffer(GL_ARRAY_BUFFER, _normID);
		glVertexAttribPointer(
		   2,                  // attribute 2. Must match the layout in the shader.
		   3,                  // size
		   GL_FLOAT,           // type
		   GL_FALSE,           // normalized?
		   0,                  // stride
		   (void*)0            // array buffer offset
		);*/
	
		glDrawArrays( GL_TRIANGLES, 0, _vertices.size());
		// Starting from vertex 0; 3 vertices total -> 1 triangle
		// Don't forget to unbind everything!
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
