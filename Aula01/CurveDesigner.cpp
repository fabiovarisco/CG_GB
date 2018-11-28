#include "CurveDesigner.h"



CurveDesigner::CurveDesigner(string filename)
{
	this->curve = new Curve();
	this->filename = filename;
}


CurveDesigner::~CurveDesigner()
{
}


void CurveDesigner::processInput()
{
	/*
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE) {
		this->mode = (this->mode + 1) % MODE_MAX;
		cout << "Mode: " << this->mode << "\n";
	}
		
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		if (this->mode == MODE_ZOOM) {
			this->curve->decreaseZoom(this->currentPoint);
			this->updateColors();
		}
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		if (this->mode == MODE_ZOOM) {
			this->curve->increaseZoom(this->currentPoint);
			this->updateColors();
		}
	}*/
}

void CurveDesigner::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	CurveDesigner* curveDesigner = reinterpret_cast<CurveDesigner*>(glfwGetWindowUserPointer(window));
	if (key == GLFW_KEY_M && action == GLFW_PRESS) {
		curveDesigner->mode = (curveDesigner->mode + 1) % MODE_MAX;
		cout << "Mode: " << curveDesigner->mode << "\n";
	}

	if (key == GLFW_KEY_U && action == GLFW_PRESS) {
		if (curveDesigner->mode == MODE_ZOOM) {
			curveDesigner->curve->decreaseZoom(curveDesigner->currentPoint);
			curveDesigner->updateColors();
		}
	}

	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		if (curveDesigner->mode == MODE_ZOOM) {
			curveDesigner->curve->increaseZoom(curveDesigner->currentPoint);
			curveDesigner->updateColors();
		}
	}
	
	if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		curveDesigner->curve->calculateCurve();
		curveDesigner->updateCurve();
	}

	if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		curveDesigner->curve->exportCurve(curveDesigner->filename);
	}
}

void CurveDesigner::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		CurveDesigner* curveDesigner = reinterpret_cast<CurveDesigner*>(glfwGetWindowUserPointer(window));
		if (curveDesigner->mode == MODE_INSERTING) {
			curveDesigner->addPoint(xpos, ypos);
		}
		else {
			curveDesigner->currentPoint = curveDesigner->curve->findNearest(xpos, ypos);
		}
		
		
	}
}

void CurveDesigner::mouse_callback(GLFWwindow* window, double xpos, double ypos) {

}

void CurveDesigner::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// zoom in and out
}

int CurveDesigner::initGLFW() {

	// Initialise GLFW
	glewExperimental = true; // Needed for core profile
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 


	// Open a window and create its OpenGL context
	window = glfwCreateWindow(C_WIDTH, C_HEIGHT, "Trabalho GB - Editor da Curva", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSetWindowUserPointer(window, this);
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);


	glm::mat4 projection = glm::ortho(0.0f, (float)C_WIDTH, (float)C_HEIGHT, 0.0f, -1.0f, 1.0f);

	shaderPoints = new Shader("vs_points.txt", "fs_points.txt");
	shaderPoints->use();
	shaderPoints->setMat4("projection", projection);

	
	glGenVertexArrays(1, &vaoPoints);
	glBindVertexArray(vaoPoints);
	glGenBuffers(1, &vboPoints);
	glGenBuffers(1, &vboZoomLevels);

	glGenBuffers(1, &vboSegments);
	glGenBuffers(1, &vboSegmentColors);
	glGenBuffers(1, &vboInternalCurve);
	glGenBuffers(1, &vboExternalCurve);

	glPointSize(6.0f);
	glLineWidth(4.0f);

}

void CurveDesigner::addPoint(float x, float y) {
	this->curve->addPoint(x, y);
	this->updatePoints();
	this->updateColors();
}

void CurveDesigner::updatePoints() {

	shaderPoints->use();
	glBindVertexArray(vaoPoints);
	
	glBindBuffer(GL_ARRAY_BUFFER, vboPoints);
	glBufferData(GL_ARRAY_BUFFER, this->curve->getPoints()->size() * sizeof(GLfloat), this->curve->getPoints()->data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

}

void CurveDesigner::updateColors() {
	shaderPoints->use();
	glBindVertexArray(vaoPoints);

	glBindBuffer(GL_ARRAY_BUFFER, vboZoomLevels);
	glBufferData(GL_ARRAY_BUFFER, this->curve->getLength() * sizeof(GLfloat), this->curve->getZoomLevels()->data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void CurveDesigner::updateCurve() {
	shaderPoints->use();
	glBindVertexArray(vaoPoints);

	glBindBuffer(GL_ARRAY_BUFFER, vboSegments);
	glBufferData(GL_ARRAY_BUFFER, this->curve->getSegments()->size() * sizeof(GLfloat), this->curve->getSegments()->data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vboSegmentColors);
	glBufferData(GL_ARRAY_BUFFER, this->curve->getSegmentLength() * sizeof(GLfloat), this->curve->getSegmentColors()->data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vboInternalCurve);
	glBufferData(GL_ARRAY_BUFFER, this->curve->getInternalCurve()->size() * sizeof(GLfloat), this->curve->getInternalCurve()->data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vboExternalCurve);
	glBufferData(GL_ARRAY_BUFFER, this->curve->getExternalCurve()->size() * sizeof(GLfloat), this->curve->getExternalCurve()->data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

}

void CurveDesigner::init() {
	this->initGLFW();
}

void CurveDesigner::loop() {
	

	do {
		
	//	processInput();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderPoints->use();
		
		glBindVertexArray(this->vaoPoints);

		glBindBuffer(GL_ARRAY_BUFFER, this->vboPoints);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (void*)0);
		
		glBindBuffer(GL_ARRAY_BUFFER, this->vboZoomLevels);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_POINTS, 0, this->curve->getLength());

		if (this->curve->getLength() > 1) {

			if (this->curve->DISPLAY_CURVE) {
				//shaderCurve->use();
				//glBindVertexArray(this->vaoCurve);
				//glDrawArrays(GL_LINE_LOOP, 0, this->curve->getLength());
				glBindBuffer(GL_ARRAY_BUFFER, this->vboSegments);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
				
				glBindBuffer(GL_ARRAY_BUFFER, this->vboSegmentColors);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);

				glDrawArrays(GL_LINE_STRIP, 0, this->curve->getSegmentLength());

				glBindBuffer(GL_ARRAY_BUFFER, this->vboInternalCurve);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
				glDrawArrays(GL_LINE_STRIP, 0, this->curve->getSegmentLength());

				glBindBuffer(GL_ARRAY_BUFFER, this->vboExternalCurve);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
				glDrawArrays(GL_LINE_STRIP, 0, this->curve->getSegmentLength());
			}
			else {
				glDrawArrays(GL_LINE_STRIP, 0, this->curve->getLength());
			}
		}
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();


	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

}


