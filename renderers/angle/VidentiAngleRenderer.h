#pragma once

#include "VidentiRenderer.h"
#include "VidentiUI.h"
#include "angle_gl.h"
#include "stb_image.h"
#include <fstream>
#include <map>

namespace VUI
{
	namespace Renderer
	{
		class VidentiAngleRenderer : public VidentiRenderer
		{
			typedef unsigned int TextureIDType;
		public:
			virtual VUI::Renderer::TextureID LoadTexture(std::string filepath);
			virtual void GenElements(std::map<std::string, UIElement*> elements);
			virtual ElementRenderData GenVerts(UIElement* element);
			virtual void Render();
			virtual void CompileRender();
			virtual void CleanCompiledRender();
			virtual void Init();
			virtual void StartFrame();
			virtual void EndFrame();
			virtual TextureID LoadFontTexture(std::string filepath);
		private:
			std::unordered_map<std::string, TextureID> loadedTextures;
			GLuint colorProgramID = 0;
			GLuint textureProgramID = 0;
			GLuint vertArray = 0;
			GLsizei vertSize = 0;
			void CreateShader(const char* vertShader, const char* fragShader, GLuint* programID);
			struct RenderData
			{
				RenderData(GLuint textureID, GLuint vertBuffer, GLsizei vertCount)
				{
					RenderData::textureID = textureID;
					RenderData::vertBuffer = vertBuffer;
					RenderData::vertCount = vertCount;
				}
				GLuint textureID, vertBuffer;
				GLsizei vertCount;
			};
			std::multimap<int32_t, RenderData> compiledRenderData;
			void SetVertexLayout();
		};
	}
}