#pragma once

#include <angle_gl.h>
#include "EGL/egl.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <utility>
#include "Tests.h"
#include "VidentiUI.h"
#include "VidentiParse.h"
#include "VidentiAngleRenderer.h"
#include "VidentiGLFWPoller.h"

static std::chrono::seconds runTime;

// first = Load, second = Unload
static std::pair<bool (*)(VUI::VidentiHandler&), bool (*)(VUI::VidentiHandler&)> testFunctions[]
{
	{VUI::Tests::LoadRendererTest, VUI::Tests::UnloadRendererTest},
	{VUI::Tests::LoadParseTest, VUI::Tests::UnloadParseTest}
};

static constexpr const int numTests = std::size(testFunctions);

VUI::VidentiHandler uiHandler;

static int testWaitSeconds = 2;

// first = loadingPassed, second = unloadingPassed
static std::pair<bool, bool> testsPassed[numTests];

// first = started, second = ended
static std::pair<bool,bool> testsRun[numTests]; 


static bool shouldQuit = false;