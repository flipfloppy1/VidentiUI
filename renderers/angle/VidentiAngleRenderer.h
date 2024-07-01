#pragma once

#include "VidentiRenderer.h"
#include "VidentiUI.h"
#include "angle_gl.h"
#include "stb_image.h"
#include <fstream>

namespace VUI
{
	namespace Renderer
	{
		class VidentiAngleRenderer : public VidentiRenderer
		{
			typedef unsigned int TextureIDType;
		public:
			virtual VUI::Renderer::TextureID LoadTexture(std::string filepath);
			virtual void GenElements(std::vector<UIElement*> elements);
			virtual ElementVertices GenVerts(UIElement* element);
			virtual void Render();
			virtual void CompileRender();
			virtual void CleanCompiledRender();
			virtual void Init();
		private:
			std::unordered_map<std::string, TextureID> loadedTextures;
			GLuint colorProgramID;
			GLuint textureProgramID;
			void CreateShader(const char* vertShader, const char* fragShader, GLuint* programID);
			struct RenderData
			{
				RenderData(GLuint textureID, GLuint vertArray, GLuint vertBuffer, GLsizei vertCount)
				{
					RenderData::textureID = textureID;
					RenderData::vertArray = vertArray;
					RenderData::vertBuffer = vertBuffer;
					RenderData::vertCount = vertCount;
				}
				GLuint textureID, vertArray, vertBuffer;
				GLsizei vertCount;
			};
			std::multimap<int32_t, RenderData> compiledRenderData;
		};
	}
}