#pragma once

#include <string>
#include <any>
#include <unordered_map>
#include <map>
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

		typedef std::any TextureID;
		struct ElementVertices
		{
			std::vector<UIVertex> vertices;
			TextureID textureID;
		};
		class VidentiRenderer
		{
		public:
			virtual TextureID LoadTexture(std::string filepath) = NULL;
			virtual void GenElements(std::vector<UIElement*> elements) = NULL;
			virtual ElementVertices GenVerts(UIElement* element) = NULL;
			virtual void Render() = NULL;
			virtual void Init() = NULL;
			virtual void CompileRender() = NULL;
			virtual void CleanCompiledRender() = NULL;
			void SetWindowDimensions(VUI::Math::vec2 dimensions)
			{
				windowDimensions = dimensions;
			};
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
			}
			bool IsGenerated()
			{
				return generated;
			}
			VUI::Math::vec2 windowDimensions;
		protected:
			std::multimap<int32_t, ElementVertices> elementVertices;
			bool compiled = false;
			bool generated = false;
		};
	}
}