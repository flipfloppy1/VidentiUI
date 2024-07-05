
#include "main.h"

static void printDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
	const GLchar* message, const void* userParam)
{
	std::string messageStr = std::string(message, message + length);
	std::string logSeverity;
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_LOW:
		logSeverity = "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		logSeverity = "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		logSeverity = "HIGH";
		break;
	default:
		logSeverity = "INFO";
	}
	std::cout << "ANGLE log | Severity: " << logSeverity.c_str() << "; Message: " << messageStr.c_str() << '\n';
}

static GLFWwindow* Load()
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialise GLFW.\n" << std::endl;
	}
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);

#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* dimensions;
	dimensions = glfwGetVideoMode(monitor);

	// Using OpenGL ES 3.1
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	int windowWidth = dimensions->width;
	int windowHeight = dimensions->height;
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Videnti Tests", NULL, NULL);
	if (window == nullptr)
	{
		std::cout << "Failed to create window:\n";
		std::string errString;
		errString.resize(512);
		const char* msgPtr = errString.data();
		glfwGetError(&msgPtr);
		std::cout << msgPtr << '\n';
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	glfwSetWindowPos(window, dimensions->width / 2 - windowWidth / 2, dimensions->height / 2 - windowHeight / 2);
	glfwSetWindowSizeLimits(window, dimensions->width / 4, dimensions->height / 4, dimensions->width, dimensions->height);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	glfwSetWindowAspectRatio(window, 16, 9);
	
	glViewport(0, 0, windowWidth, windowHeight);

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(printDebug, NULL);
#endif

	//glFrontFace(GL_CCW);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glClipControlEXT(GL_LOWER_LEFT_EXT, GL_ZERO_TO_ONE_EXT);

	return window;
}

static void Unload(GLFWwindow* window)
{
	// TODO: Report test passes and failures
	glfwDestroyWindow(window);
	glfwTerminate();
}

static void Update(std::chrono::seconds runTime, float deltaTime)
{
	uiHandler.SetLuaGlobals(deltaTime);	
	uiHandler.ParseUI("resources/update.lua");
	uiHandler.GenUI();
}

static void Render(GLFWwindow* window)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	uiHandler.StartFrame();
	uiHandler.Render();
	uiHandler.EndFrame();

	glfwSwapBuffers(window);
}

static void Input()
{
	glfwPollEvents();
}

int main()
{
	GLFWwindow* window = Load();

	using namespace std::chrono_literals;
	auto startTime = std::chrono::high_resolution_clock::now();
	auto delta = std::chrono::high_resolution_clock::now();
	std::chrono::seconds runTime = 0s;
	std::chrono::milliseconds deltaTime = 0ms;


	uiHandler.AttachRenderer(new VUI::Renderer::VidentiAngleRenderer(), { WINDOW_WIDTH,WINDOW_HEIGHT });
	uiHandler.Init();

	uiHandler.ParseUI("resources/start.lua");
	uiHandler.GenUI();

	while (!((glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) || shouldQuit))
	{
		Update(runTime, deltaTime.count() / 1000.0f);
		Input();
		Render(window);
		runTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - startTime);
		deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - delta);
		delta = std::chrono::high_resolution_clock::now();
	}
	Unload(window);
}

