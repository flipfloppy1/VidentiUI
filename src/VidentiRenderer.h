#pragma once

#include <string>
#include <any>
#include <unordered_map>
#include <map>
#include <vector>
#include "VidentiMath.h"
#include <ft2build.h>
#include FT_FREETYPE_H

namespace VUI
{
	class UIElement;

	namespace Renderer
	{
		struct UIVertex 
		{
			VUI::Math::vec2 pos;
			VUI::Math::u8vec4 color;
			VUI::Math::vec2 texCoord;
			UIVertex(VUI::Math::vec2 pos, VUI::Math::u8vec4 color, VUI::Math::vec2 texCoord)
			{
				UIVertex::pos = pos;
				UIVertex::color = color;
				UIVertex::texCoord = texCoord;
			}
		};

		struct ElementVertices
		{
			std::vector<UIVertex> vertices;
			std::vector<UIVertex> textVertices;
		};

		typedef std::any TextureID;
		struct ElementRenderData
		{
			std::vector<UIVertex> vertices;
			TextureID textureID;
			std::vector<UIVertex> textVertices;
			TextureID fontTextureID;
		};

		class VidentiRenderer
		{
		public:
			virtual TextureID LoadTexture(std::string filepath) = 0;
			virtual void GenElements(std::map<std::string, UIElement*> elements) = 0;
			virtual ElementRenderData GenVerts(UIElement* element) = 0;
			virtual void Render() = 0;
			virtual void Init() = 0;
			virtual void CompileRender() = 0;
			virtual void CleanCompiledRender() = 0;
			virtual void StartFrame() = 0;
			virtual void EndFrame() = 0;
			virtual TextureID LoadFontTexture(std::string filepath) = 0;
			struct GlyphInfo
			{
				float advance;
				Math::vec2 pos, dim, offset;
			};
			GlyphInfo GetGlyphInfo(std::string fontFile, char character);
			void InitFreetype();
			void InstructCompilation()
			{
				compiled = false;
			}
			bool IsCompiled()
			{
				return compiled;
			}
			void InstructGeneration()
			{
				generated = false;
				elementVertices.clear();
			}
			bool IsGenerated()
			{
				return generated;
			}
			VUI::Math::vec2 windowDimensions;
			void SetWindowDimensions(VUI::Math::vec2 dimensions)
			{
				windowDimensions = dimensions;
			};
			struct TexInfo
			{
				unsigned int index;
				unsigned int height;
				unsigned int width;
			};
			struct TypefaceInfo
			{
				TypefaceInfo(TexInfo texInfo, std::map<char, GlyphInfo> glyphInfo)
				{
					TypefaceInfo::texInfo = texInfo;
					TypefaceInfo::glyphInfo = glyphInfo;
				}
				TexInfo texInfo;
				std::map<char, GlyphInfo> glyphInfo;
			};
			TexInfo GetTypeFaceInfo(std::string fontFile);
		protected:
			FT_Library freetypeLibrary;
			std::multimap<int32_t, ElementRenderData> elementVertices;
			bool compiled = false;
			bool generated = false;
			std::vector<unsigned char> typefaceTexBytes;
			TexInfo LoadFont(std::string filepath);
			std::map<std::string, TypefaceInfo> typefaceMap;
		};
	}
}