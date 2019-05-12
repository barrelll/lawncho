#include <include/font.h>

namespace iom
{	
	font::font()
		:
			_library(nullptr),
			_face(nullptr)
	{};

	font::~font() noexcept(true)
	{
		// Release our Freetype data
		if (_face)
			FT_Done_Face(_face);
		if (_library)
			FT_Done_FreeType(_library);
		for (unsigned int i = 0; i < _gMap.size(); i++)
		{
			glDeleteTextures(1, &_gMap.at(i).ID);
		}
		_gMap.clear();
	};

	// Calculate dots per inch 
	// dp is diagonal resolution in pixels
	// wp is width resolution in pixels
	// hp is height resolution in pixels
	// di is diagonal size in inches (this is the number advertised as the size of the display).
	// dp sqareroot{ wp^2 + hp^2 };
	// di squareroot{ wi^2 + hi^2 }
	// dpi(ppi) = dp / di;
	inline void font::calcDPI(int& hori_dpi, int& vert_dpi)
	{
		GLFWmonitor* primary = glfwGetPrimaryMonitor();
		int mwidth = 0, mheight = 0;
		glfwGetMonitorPhysicalSize(primary, &mwidth, &mheight);
		mwidth /= 25.4; mheight /= 25.4;
		hori_dpi = 800 / mwidth; vert_dpi = 600 / mheight;
	}

	// OpenGL expects textures to be in a width and
	// height at the ^2
	// example 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024		
	inline void font::next_p2 (int& a )
	{
		int rval = 2;
		if (a == 0)
			a = 0;
		// rval<<=1 Is A Prettier Way Of Writing rval*=2;
		else
		{
			while(rval < a) rval <<= 1;
			a = rval;
		}
	}

	void font::LoadTexture(const std::string& fileName)
	{
		int hori_dpi = 0, vert_dpi = 0;
		calcDPI(hori_dpi, vert_dpi);

		/* Initialize the freetype library
		 */
		if(FT_Init_FreeType(&_library))
			throw std::runtime_error("Failed to initialize freetype");


		/* Create a face object from freetype, 
		 */
		if(FT_New_Face(_library, fileName.c_str(), 0, &_face))
			throw std::runtime_error("Failed to create FT_Face");

		// Set the char size 
		// # << 6 is just a way of saying # * 2^6;
		if(FT_Set_Char_Size(	  _face,    	 /* handle to face object           */
					  36 << 6,    	 /* char_width in 1/64th of points  */
					  36 << 6,    	 /* char_height in 1/64th of points */
					  hori_dpi,      /* horizontal device resolution    */
					  vert_dpi ))    /* vertical device resolution      */
			throw std::runtime_error("Failed to set character sizes");

		for (int c = 0; c < 256; c++)
		{
			// Load each char from our new face
			if(FT_Load_Char(_face, c, FT_LOAD_RENDER))
				throw std::runtime_error("Failed to load character sheet");
			std::unique_ptr<FT_Bitmap> bitmap(new FT_Bitmap);
			*bitmap = _face->glyph->bitmap;
			glyph gl;
			gl.width = bitmap->width;
			gl.height = bitmap->rows;
			gl.top = _face->glyph->bitmap_top;
			gl.left = _face->glyph->bitmap_left;
			load_glyph(gl, bitmap->buffer);
			_gMap.insert(std::pair<char, glyph>(c, gl));
		}
	}

	// Loads the glyph into OpenGL memory
	void font::load_glyph(glyph& glyph, const unsigned char* data)
	{
		glGenTextures(1, &glyph.ID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, glyph.ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexImage2D(	GL_TEXTURE_2D, 0, GL_R8,
				glyph.width, glyph.height,
				0, GL_RED, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, 0);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}       

	void font::RenderText(const std::string& str, float x, float y, float sx, float sy)
	{
		GLuint is = 0;
		glGenBuffers(1, &is);
		for(auto c : str) 
		{
			glyph& glyph = _gMap.at(c);
			glBindTexture(GL_TEXTURE_2D, _gMap.at(c).ID);

			const float vx = x + glyph.left * sx;
			const float vy = y + glyph.top * sy;
			const float w = glyph.width * sx;
			const float h = glyph.height * sy;
			glyph.ndc_width = w; glyph.ndc_height = h;

			struct {
				float x, y, s, t;
			} data[6] = {
				{vx    , vy    , 0, 0},
				{vx    , vy - h, 0, 1},
				{vx + w, vy    , 1, 0},
				{vx + w, vy    , 1, 0},
				{vx    , vy - h, 0, 1},
				{vx + w, vy - h, 1, 1}
			};

			glBindBuffer(GL_ARRAY_BUFFER, is);
			glBufferData(GL_ARRAY_BUFFER, 24*sizeof(float), &data[0], GL_DYNAMIC_DRAW);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			x += (_face->glyph->advance.x >> 6) * sx;
			y += (_face->glyph->advance.y >> 6) * sy;
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// Get specific glyph dimensions
	float font::GetGlyphWidth(const char* glyphn)
	{
		glyph& glyph = _gMap.at(*glyphn);
		return glyph.ndc_width;
	}

	float font::GetGlyphHeight(const char* glyphn)
	{
		glyph& glyph = _gMap.at(*glyphn);
		return glyph.ndc_width;
	}

	// Return the advance of the entire string
	// not just glyph dimensions, but the size of the spaceing
	// between them too
	float font::GetStringAdvancedWidth(const std::string& str)
	{
		float ndc_width = 0; 
		for(unsigned int i = 0; i < str.length(); i++)
		{
			ndc_width += (_face->glyph->advance.x >> 6) * 1.0f / 800;
		}
		return ndc_width;
	}

	float font::GetStringAdvancedHeight(const std::string& str)
	{
		float ndc_height = 0; 
		for(unsigned int i = 0; i < str.length(); i++)
		{
			ndc_height += (_face->glyph->advance.x >> 6) * 1.0f / 800;
		}
		return ndc_height;
	}
}
