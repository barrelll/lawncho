#pragma once
#ifdef OS_WINDOWS
#include <ctgmath>
#elif OS_UNIX
#include <tgmath.h>
#endif

#include <string>
#include <stdexcept>
#include <memory>
#include <map>
#include <include/GL/glew.h>
#include <include/GLFW/glfw3.h>
#include <include/ft2build.h>
#include FT_FREETYPE_H
#include <include/texture.h>

namespace iom
{
	/* Each font will hold a list of iom::texture* types, 
	 * a font has multiple glyphs, 
	 * we'll hold one for each, 
	 * for now start with 512
	 * for Reference, see glyph.png in fonts/ directory
	 */

	class font
	{
	public:
		font();
		virtual ~font() noexcept(true);

		/* Initialize the font class, something that will be entirely handled inside any class that needs it
		 * careful not to initialize in a loop
		 */
		void LoadTexture(const std::string& fileName);
		void RenderText(const std::string& str, float x, float y, float sx, float sy);

		float GetStringAdvancedWidth(const std::string& str);
		float GetStringAdvancedHeight(const std::string& str);
		float GetGlyphWidth(const char* glyphn);
		float GetGlyphHeight(const char* glyphn);
	private:
		/* Each char is a texture type, has  GLuint ID for generating a
		 * texture ID 
		 */

		struct glyph
		{
			GLuint ID;
			unsigned int width, height;
			unsigned int top, left;
			float ndc_width = 0, ndc_height = 0;
		};

		std::map<char, glyph> _gMap;

		// Helper functon, grab the 
		// ^2 
		inline void 	calcDPI(int& hori_dpi, int& vert_dpi);
		inline void 	next_p2(int& a);
		void 		load_glyph(glyph& glyph, const unsigned char* data);
		/**************************/
		FT_Library 	_library;
		FT_Face    	_face;
	};
}
