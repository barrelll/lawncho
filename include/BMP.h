#pragma once
#include <include/texture.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace iom
{

	// is a texture type, 
	// should load texture based on bitmap load functions when texture == &BMP
	class BMP : public texture
	{
	public:
		BMP();
		BMP(const BMP& rhs);
		~BMP();

		void LoadTexture(const std::string& imageFilePath);
	private:
		int width, height;
	};
}
