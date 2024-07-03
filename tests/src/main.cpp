
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
	glClipControlEXT(GL_LOWER_LEFT_EXT, GL_ZERO_TO_ONE_EXT);

	return window;
}

static void Unload(GLFWwindow* window)
{
	// TODO: Report test passes and failures
	glfwDestroyWindow(window);
	glfwTerminate();
	system("pause");
}

static void Update(std::chrono::seconds runTime)
{
	shouldQuit = true;
	for (int i = 0; i < numTests; i++)
	{
		if (!testsRun[i].first && runTime >= (std::chrono::seconds)(testWaitSeconds * i))
		{
			std::cout << "Loading Test #" << std::to_string(i + 1) << "...\n";
			testsPassed[i].first = testFunctions[i].first(uiHandler);

			if (testsPassed[i].first)
				std::cout << "Loading Test #" << std::to_string(i + 1) << " \033[1;32mpassed\033[0m\n";
			else
				std::cout << "Loading Test #" << std::to_string(i + 1) << " \033[1;31mfailed\033[0m\n";

			testsRun[i].first = true;
		}
		else if (!testsRun[i].second && runTime >= (std::chrono::seconds)(testWaitSeconds * (i + 1)))
		{
			std::cout << "Unloading Test #" << std::to_string(i + 1) << "...\n";
			testsPassed[i].second = testFunctions[i].second(uiHandler);

			if (testsPassed[i].second)
				std::cout << "Unloading Test #" << std::to_string(i + 1) << " \033[1;32mpassed\033[0m\n";
			else															  
				std::cout << "Unloading Test #" << std::to_string(i + 1) << " \033[1;31mfailed\033[0m\n";

			testsRun[i].second = true;
		}
		shouldQuit &= testsRun[i].second;
	}
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
	std::chrono::seconds runTime = 0s;

	std::memset(testsRun, 0, sizeof(bool) * numTests);
	
	while (!((glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) || shouldQuit))
	{
		Update(runTime);
		Input();
		Render(window);
		runTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - startTime);
	}
	Unload(window);
}

