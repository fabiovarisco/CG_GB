#pragma once

#ifndef CURVE_DESIGNER_H
#define CURVE_DESIGNER_H

#include <vector>
#include <string>

#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cstdlib>
#include <limits>
#include <stdexcept>
#include <fstream>

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Curve.h"
#include "Shader.h"

using namespace std;

const static unsigned int C_WIDTH = 1024, C_HEIGHT = 768;
const static unsigned int MODE_INSERTING = 0, MODE_ZOOM = 1, MODE_CURVES = 2, MODE_MAX = 3;

class CurveDesigner
{

private: 
	string filename;
	Curve *curve;
	Shader *shaderPoints, *shaderCurve;
	GLFWwindow *window;
	GLuint vaoPoints, vboPoints, vboZoomLevels, vboSegments, vboSegmentColors, vboInternalCurve, vboExternalCurve;

	int mode = MODE_INSERTING, currentPoint;
	int initGLFW();
	void processInput();
	void static mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	void static mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void static scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void static key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void addPoint(float x, float y);
	void updatePoints();
	void updateColors();
	void updateCurve();
	

public:
	CurveDesigner(string filename);
	~CurveDesigner();

	//void open(std::string obj_filename);
	void init();
	void loop();
};

#endif