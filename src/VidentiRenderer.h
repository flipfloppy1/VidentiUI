#pragma once

#include <string>
#include <any>
#include <unordered_map>
#include <map>
#include <vector>
#include "VidentiMath.h"

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

		class VidentiHandler;

		typedef std::any TextureID;
		struct ElementVertices
		{
			std::vector<UIVertex> vertices;
			TextureID textureID;
		};

		class VidentiRenderer
		{
		public:
			virtual TextureID LoadTexture(std::string filepath) = 0;
			virtual void GenElements(std::map<std::string, UIElement*> elements) = 0;
			virtual ElementVertices GenVerts(UIElement* element) = 0;
			virtual void Render() = 0;
			virtual void Init() = 0;
			virtual void CompileRender() = 0;
			virtual void CleanCompiledRender() = 0;
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
		protected:
			std::multimap<int32_t, ElementVertices> elementVertices;
			bool compiled = false;
			bool generated = false;
		};
	}
}