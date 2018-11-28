#pragma once

#ifndef scene_H
#define scene_H

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

#include "stb_image.h"

#include "Material.h"
#include "Mesh.h"
#include "OBJReader.h"
#include "MtlReader.h"
#include "Shader.h"
#include "Camera.h"

const static unsigned int width = 1024, height = 768;

static float lastX = (float)width / 2.0f;
static float lastY = (float)height / 2.0f;
static bool firstMouse = true;

static Camera* globalCamera;

using namespace std;


class Scene
{
private:

	vector<float>* curvePoints;
	
	Camera* camera = new Camera(glm::vec3(0.0f, 10.0f, 0.0f));

	// timing
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;

	glm::vec3 lightPos = glm::vec3(1.0, 2.5, 10.0);

	void processInput(GLFWwindow *window);
	void static mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void static scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	int setupGLFW();
	GLFWwindow* window;
	void setupGroups(Mesh* mesh);
	void setupMaterials(unordered_map<string, Material*> materials, string base_path);
	void drawMesh(Mesh *mesh, unordered_map<string, Material*> materials, Shader* shader);
	void readPoints(string filename);

public:
	Mesh* mesh;
	Mesh* carMesh; 
	unordered_map<string, Material*> materials, carMaterials;
	Shader* shader;
	GLuint VAO, vVBO, nVBO, tVBO;
	int init(string base_path, string obj_filename, string curve_filename);
	Scene();
	~Scene();
	void draw();
};

#endif
