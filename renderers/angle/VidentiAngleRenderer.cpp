
#include "VidentiAngleRenderer.h"

const char* colorVertShader =
"#version 300 es\n"
"precision highp float;\n"
"layout (location = 0) in vec2 inPos;\n"
"layout (location = 1) in uint inColor;\n"
"layout (location = 2) in vec2 inTexCoord;\n"
"flat out vec4 finalColor;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(inPos, 0.0f, 1.0f);\n"
"uint r = (inColor) & uint(0xFF);\n"
"uint g = (inColor >> 8) & uint(0xFF);\n"
"uint b = (inColor >> 16) & uint(0xFF);\n"
"uint a = (inColor >> 24) & uint(0xFF);\n"
"finalColor = vec4(float(r),float(g),float(b),float(a)) / 255.0f;\n"
"}\n";

const char* colorFragShader =
"#version 300 es\n"
"precision highp float;\n"
"out vec4 color;\n"
"flat in vec4 finalColor;\n"
"void main()\n"
"{\n"
"if (finalColor.w == 0.0f)\n"
"discard;\n"
"color = finalColor;\n"
"}\n";

const char* textureVertShader =
"#version 300 es\n"
"precision highp float;\n"
"layout (location = 0) in vec2 inPos;\n"
"layout (location = 1) in uint inColor;\n"
"layout (location = 2) in vec2 inTexCoord;\n"
"flat out vec4 tint;\n"
"out vec2 texCoord;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(inPos, 0.0f, 1.0f);\n"
"uint r = (inColor) & uint(0xFF);\n"
"uint g = (inColor >> 8) & uint(0xFF);\n"
"uint b = (inColor >> 16) & uint(0xFF);\n"
"uint a = (inColor >> 24) & uint(0xFF);\n"
"tint = vec4(float(r),float(g),float(b),float(a)) / 255.0f;\n"
"texCoord = inTexCoord;\n"
"}\n";

const char* textureFragShader =
"#version 300 es\n"
"precision highp float;\n"
"out vec4 color;"
"flat in vec4 tint;\n"
"in vec2 texCoord;\n"
"uniform sampler2D mainTex;\n"
"void main()\n"
"{\n"
"if (texture(mainTex,texCoord).w == 0.0f || tint.w == 0.0f)\n"
"discard;\n"
"color = tint * texture(mainTex,texCoord);\n"
"}\n";

VUI::Renderer::TextureID VUI::Renderer::VidentiAngleRenderer::LoadTexture(std::string filepath)
{
	stbi_set_flip_vertically_on_load(true);

	if (filepath == "")
		return TextureID(GLuint(0));

	if (loadedTextures.contains(filepath))
		return loadedTextures.at(filepath);

	std::ifstream f(filepath.c_str());
	if (!f.is_open())
	{
		VUI::Log(VUI::ERROR_MINOR, std::string("VidentiAngleRenderer::LoadTexture: Could not find file \"" + filepath + "\", returning texture 0").c_str());
		return TextureID(GLuint(0));
	}
	f.close();

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	int width, height, channels;
	unsigned char* bytes = stbi_load(filepath.c_str(), &width, &height, &channels, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, GL_NONE, GL_RGBA, GL_UNSIGNED_BYTE, bytes);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	stbi_image_free(bytes);

	// Add texture to list of loaded ones
	loadedTextures[filepath] = TextureID(textureID);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Create and assign uniform
	glUseProgram(textureProgramID);
	GLint loc = glGetUniformLocation(textureProgramID, "mainTex");
	glUniform1i(loc, 0);

	return TextureID(textureID);
}

void VUI::Renderer::VidentiAngleRenderer::GenElements(std::map<std::string, UIElement* > elements)
{
	for (auto [id, element] : elements)
		elementVertices.insert(std::pair{ element->layer,GenVerts(element)});

	generated = true;
	compiled = false;
}

VUI::Renderer::ElementRenderData VUI::Renderer::VidentiAngleRenderer::GenVerts(UIElement* element)
{
	VUI::Renderer::TextureID texID = LoadTexture(element->texture);
	VUI::Renderer::TextureID fontTexID = LoadFontTexture(element->font);
	ElementVertices uiVertices = element->GenVerts();

	return { uiVertices.vertices, texID, uiVertices.textVertices, fontTexID };
}

void VUI::Renderer::VidentiAngleRenderer::Render()
{
	if (!compiled)
		CompileRender();
	glBindVertexArray(vertArray);
	for (auto iter = compiledRenderData.begin(); iter != compiledRenderData.end(); iter++)
	{
		if (iter->second.textureID != 0) // if the element has a texture
		{
			glUseProgram(textureProgramID);
			glBindTexture(GL_TEXTURE_2D, iter->second.textureID);
			glBindVertexBuffer(0, iter->second.vertBuffer, 0, vertSize);
			glDrawArrays(GL_TRIANGLES, 0, iter->second.vertCount);
		}
		else
		{
			glUseProgram(colorProgramID);
			glBindVertexBuffer(0, iter->second.vertBuffer, 0, vertSize);
			glDrawArrays(GL_TRIANGLES, 0, iter->second.vertCount);
		}
	}
}

void VUI::Renderer::VidentiAngleRenderer::CleanCompiledRender()
{
	std::vector<GLuint> vertBuffers;
	for (auto iter = compiledRenderData.begin(); iter != compiledRenderData.end(); iter++)
	{
		vertBuffers.push_back(iter->second.vertBuffer);
	}
	if (!vertBuffers.empty())
		glDeleteBuffers(vertBuffers.size(), vertBuffers.data());
	compiledRenderData.clear();
}

void VUI::Renderer::VidentiAngleRenderer::CompileRender()
{
	CleanCompiledRender();
	for (auto iter = elementVertices.begin(); iter != elementVertices.end(); iter++)
	{
		for (int i = 0; i < 2; i++)
		{
			if (i && iter->second.textVertices.empty())
				continue;
			std::vector<VUI::Renderer::UIVertex>* vertices = i ? &iter->second.textVertices : &iter->second.vertices;
			TextureID texID = i ? iter->second.fontTextureID : iter->second.textureID;
			int32_t layer = i ? iter->first + 1 : iter->first;
			GLuint vertBuffer;
			glGenBuffers(1, &vertBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
			glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(VUI::Renderer::UIVertex), vertices->data(), GL_DYNAMIC_DRAW);

			compiledRenderData.insert({ layer,RenderData(std::any_cast<GLuint>(texID),vertBuffer,vertices->size()) });
		}
	}
	compiled = true;
}

void VUI::Renderer::VidentiAngleRenderer::CreateShader(const char* vertShader, const char* fragShader, GLuint* programID)
{
	GLuint vertexID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexID, 1, &vertShader, NULL);
	glShaderSource(fragmentID, 1, &fragShader, NULL);

	glCompileShader(vertexID);
	GLint compiled;
	glGetShaderiv(vertexID, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLint infoLogLength = 0;
		glGetShaderiv(vertexID, GL_INFO_LOG_LENGTH, &infoLogLength);

		std::string infoLog;
		infoLog.reserve(infoLogLength);
		glGetShaderInfoLog(vertexID, infoLogLength, NULL, infoLog.data());

		VUI::Log(VUI::ERROR_MAJOR, infoLog.c_str());
	}
	
	glCompileShader(fragmentID);
	glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLint infoLogLength = 0;
		glGetShaderiv(fragmentID, GL_INFO_LOG_LENGTH, &infoLogLength);

		std::string infoLog;
		infoLog.reserve(infoLogLength);
		glGetShaderInfoLog(fragmentID, infoLogLength, NULL, infoLog.data());

		VUI::Log(VUI::ERROR_MAJOR, infoLog.c_str());
	}

	*programID = glCreateProgram();
	glAttachShader(*programID, vertexID);
	glAttachShader(*programID, fragmentID);
	glLinkProgram(*programID);

	GLint success;
	glGetProgramiv(*programID, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(*programID, 512, NULL, infoLog);
		Log(VUI::ERROR_MAJOR, infoLog);
	}

	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
}

void VUI::Renderer::VidentiAngleRenderer::Init()
{
	CreateShader(colorVertShader, colorFragShader, &colorProgramID);
	CreateShader(textureVertShader, textureFragShader, &textureProgramID);

	SetVertexLayout();
}

void VUI::Renderer::VidentiAngleRenderer::SetVertexLayout()
{
	if (vertArray)
		glDeleteVertexArrays(1, &vertArray);

	glGenVertexArrays(1, &vertArray);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(vertArray);
	glBindVertexBuffer(0, 0, 0, sizeof(VUI::Renderer::UIVertex));
	
	glEnableVertexAttribArray(0);
	glVertexAttribFormat(0, 2, GL_FLOAT, false, 0);
	glVertexAttribBinding(0, 0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribIFormat(1, 1, GL_UNSIGNED_INT, sizeof(float) * 2);
	glVertexAttribBinding(1, 0);
	
	glEnableVertexAttribArray(2);
	glVertexAttribFormat(2, 2, GL_FLOAT, false, sizeof(float) * 2 + sizeof(unsigned int));
	glVertexAttribBinding(2, 0);

	vertSize = sizeof(VUI::Renderer::UIVertex);
}

void VUI::Renderer::VidentiAngleRenderer::StartFrame()
{
	glDisable(GL_DEPTH_TEST);
}

void VUI::Renderer::VidentiAngleRenderer::EndFrame()
{
	glEnable(GL_DEPTH_TEST);
}

VUI::Renderer::TextureID VUI::Renderer::VidentiAngleRenderer::LoadFontTexture(std::string filepath)
{
	if (filepath == "")
		return TextureID(GLuint(0));

	if (loadedTextures.contains(filepath))
		return loadedTextures.at(filepath);

	TexInfo texInfo = LoadFont(filepath);
	GLuint texID;

	if (texInfo.index + texInfo.height * texInfo.width * 4 > typefaceTexBytes.size())
	{
		VUI::Log(VUI::ERROR_MAJOR, "Font texture info did not appear to be loaded");
		return TextureID(GLuint(0));
	}

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texInfo.width, texInfo.height, GL_NONE, GL_RGBA, GL_UNSIGNED_BYTE, typefaceTexBytes.data() + texInfo.index);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	loadedTextures[filepath] = TextureID(texID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
	glUseProgram(textureProgramID);
	GLint loc = glGetUniformLocation(textureProgramID, "mainTex");
	glUniform1i(loc, 0);

	return TextureID(texID);
}
