#pragma once

#include <angle/angle_gl.h>
#include "EGL/egl.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <utility>
#include "Tests.h"

static std::chrono::seconds runTime;
static const int numTests = 1;

VUI::UIHandler uiHandler;

static int testWaitSeconds = 3;

// first = loadingPassed, second = unloadingPassed
static std::pair<bool, bool> testsPassed[numTests];

// first = started, second = ended
static std::pair<bool,bool> testsRun[numTests]; 

// first = Load, second = Unload
static std::pair<bool (*)(VUI::UIHandler&), bool (*)(VUI::UIHandler&)> testFunctions[numTests]
{
	{VUI::Tests::LoadParseTest, VUI::Tests::UnloadParseTest}
};
static bool shouldQuit = false;