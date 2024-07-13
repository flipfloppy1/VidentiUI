
#include "VidentiRenderer.h"
#include "VidentiUI.h"
#include <cmath>
#include <ft2build.h>
#include FT_FREETYPE_H

VUI::Renderer::VidentiRenderer::TexInfo VUI::Renderer::VidentiRenderer::LoadFont(std::string filepath)
{
	if (freetypeLibrary == NULL)
	{
		VUI::Log(VUI::ERROR_MAJOR, "Could not load typeface, freetype library was NULL; returning null texture");
	}

	FT_Face face;
	int error = FT_New_Face(freetypeLibrary,filepath.c_str(),0,&face);

	if (error == FT_Err_Unknown_File_Format)
	{
		VUI::Log(VUI::ERROR_MINOR, "Font file was of unknown format");
		return TexInfo();
	}
	else if (error)
	{
		VUI::Log(VUI::ERROR_MINOR, std::string("Font file \"" + filepath + "\" could not be loaded").c_str());
	}

	FT_Set_Char_Size(face, 0, 48 * 64, 96, 96);

	int maxDim = (1 + (face->size->metrics.height >> 6)) * ceilf(sqrtf(128));
	int texWidth = 1;
	while(texWidth < maxDim) texWidth <<= 1;
	int texHeight = texWidth;

	std::vector<unsigned char> pixels(texWidth * texHeight);
	std::map<char, GlyphInfo> glyphInfo;
	int penX = 0, penY = 0;
	

	for(int i = 0; i < 128; ++i){
		FT_Load_Char(face, i, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT);
		FT_Bitmap* bmp = &face->glyph->bitmap;

		if(penX + bmp->width >= texWidth){
			penX = 0;
			penY += ((face->size->metrics.height >> 6) + 1);
		}

		for(int row = 0; row < bmp->rows; ++row){
			for(int col = 0; col < bmp->width; ++col){
				int x = penX + col;
				int y = penY + row;
				pixels[y * texWidth + x] = bmp->buffer[row * bmp->pitch + col];
			}
		}

		GlyphInfo info;
		info.pos = { (float)penX, (float)penY + (float)bmp->rows };
		info.dim = { (float)penX + (float)bmp->width, (float)penY };

		info.offset = { (float)face->glyph->bitmap_left, (float)face->glyph->bitmap_top };
		info.advance = (float)(face->glyph->advance.x >> 6);

		glyphInfo.insert({ (char)i,info });

		penX += bmp->width + 1;
	}

	FT_Done_Face(face);

	std::vector<unsigned char> texture(texWidth * texHeight * 4);

	for (int i = 0; i < texWidth * texHeight; i++)
	{
		texture[i * 4] |= pixels[i];
		texture[i * 4 + 1] |= pixels[i];
		texture[i * 4 + 2] |= pixels[i];
		texture[i * 4 + 3] |= pixels[i];
	}

	TexInfo texInfo;
	texInfo.index = typefaceTexBytes.size();
	texInfo.width = texWidth;
	texInfo.height = texHeight;

	typefaceTexBytes.insert(typefaceTexBytes.end(), texture.begin(), texture.end());

	typefaceMap.insert({ filepath,TypefaceInfo(texInfo,glyphInfo) });

	return texInfo;
}

VUI::Renderer::VidentiRenderer::TexInfo VUI::Renderer::VidentiRenderer::GetTypeFaceInfo(std::string fontFile)
{
	if (!typefaceMap.contains(fontFile))
	{
		VUI::Log(VUI::ERROR_MINOR, std::string("Failed to find type face info for font file \"" + fontFile + '\"').c_str());
		return TexInfo();
	}

	return typefaceMap.at(fontFile).texInfo;
}

void VUI::Renderer::VidentiRenderer::InitFreetype()
{
	FT_Library library;

	int error = FT_Init_FreeType(&library);
	if (error)
	{
		VUI::Log(VUI::ERROR_FATAL, "Failed to load freetype library");
		return;
	}

	freetypeLibrary = library;
}

VUI::Renderer::VidentiRenderer::GlyphInfo VUI::Renderer::VidentiRenderer::GetGlyphInfo(std::string fontFile, char character)
{
	if (!typefaceMap.contains(fontFile))
	{
		VUI::Log(VUI::ERROR_MAJOR, std::string("VidentiRenderer::GetGlyphInfo: Could not find info for font file \"" + fontFile + "\"").c_str());
		return GlyphInfo();
	}

	if (!typefaceMap.at(fontFile).glyphInfo.contains(character))
	{
		std::string message;
		message += "VidentiRenderer::GetGlyphInfo: Could not find info for glyph \'";
		message.push_back(character);
		message += "\' in font file \"" + fontFile + '\"';
		VUI::Log(VUI::ERROR_MINOR, message.c_str());
		return GlyphInfo();
	}

	return typefaceMap.at(fontFile).glyphInfo.at(character);
}