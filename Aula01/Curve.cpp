#include "Curve.h"



Curve::Curve()
{
	this->points = new vector<float>();
	this->zoomLevels = new vector<float>();
	this->segments = new vector<float>();
	SEGMENT_LENGTH = int(1 / CURVE_INC);
	SEGMENT_SIZE = SEGMENT_LENGTH * 2;
}


Curve::~Curve()
{
}

void Curve::addPoint(float x, float y) {
	this->points->push_back(x);
	this->points->push_back(y);
	this->zoomLevels->push_back(0.5);
	this->DISPLAY_CURVE = false;
}

vector<float>* Curve::getPoints() {
	return this->points;
}

vector<float>* Curve::getZoomLevels() {
	return this->zoomLevels;
}

int Curve::getLength() {
	return this->points->size() / 2;
}

void Curve::setZoomLevel(int i, float zoomLevel) {
	this->zoomLevels->at(i) = zoomLevel;
}

void Curve::increaseZoom(int i) {
	this->zoomLevels->at(i) = this->zoomLevels->at(i) + 0.1;
}

void Curve::decreaseZoom(int i) {
	this->zoomLevels->at(i) = this->zoomLevels->at(i) - 0.1;
}

void Curve::getPoint(int i, float &x, float &y) {
	x = this->points->at(2 * i);
	y = this->points->at(2 * i + 1);
}

int Curve::findNearest(float x, float y) {
	int nearest = -1; 
	float nearestDist = std::numeric_limits<double>::infinity();
	float currentDist;
	for (int i = 0; i < this->getLength(); i++) {
		currentDist = sqrt(pow(x - this->points->at(2 * i), 2) + pow(y - this->points->at(2 * i + 1), 2));
		if (currentDist < nearestDist) {
			nearestDist = currentDist;
			nearest = i;
		}
	}
	return nearest;
}

void Curve::calculateSegment(int i0, int i1, int i2, int i3) {
	int x, y;
	float z;
	float x0 = this->points->at(2 * i0);
	float y0 = this->points->at(2 * i0 + 1);
	float z0 = this->zoomLevels->at(i0);
	int j = 0;
	for (float t = 0; t <= 1; t += CURVE_INC) {
		x = (((-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1) * this->points->at(2*i0) +
			(3 * pow(t, 3) - 6 * pow(t, 2) + 0 * t + 4) * this->points->at(2*i1) +
			(-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1)*this->points->at(2*i2) +
			(1 * pow(t, 3) + 0 * pow(t, 2) + 0 * t + 0)*this->points->at(2*i3)) / 6);
		y = (((-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1)*this->points->at(2*i0 + 1) +
			(3 * pow(t, 3) - 6 * pow(t, 2) + 0 * t + 4)*this->points->at(2*i1 + 1) +
			(-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1)*this->points->at(2*i2 + 1) +
			(1 * pow(t, 3) + 0 * pow(t, 2) + 0 * t + 0)*this->points->at(2*i3 + 1)) / 6);
		z = (((-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1)*this->zoomLevels->at(i0) +
			(3 * pow(t, 3) - 6 * pow(t, 2) + 0 * t + 4)*this->zoomLevels->at(i1) +
			(-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1)*this->zoomLevels->at(i2) +
			(1 * pow(t, 3) + 0 * pow(t, 2) + 0 * t + 0)*this->zoomLevels->at(i3)) / 6);
		this->segments->push_back(x);
		this->segments->push_back(y);
		this->segmentColors->push_back(z);
		//plot(x0, y0, x, y);
		x0 = x; y0 = y; z0 = z;
	}
	
}

void Curve::calculateCurve() {
	this->segments = new vector<float>();
	this->segmentColors = new vector<float>();
	int length = this->getLength();
	for (int i = 0; i < length - 3; i++) {
		this->calculateSegment(i, i + 1, i + 2, i + 3);
	}
	for (int i = length - 3; i < length; i++) {
		this->calculateSegment(i % length, (i + 1) % length, (i + 2) % length, (i + 3) % length);
	}
	
	this->computeCurve(true);
	this->computeCurve(false);

	DISPLAY_CURVE = true;
}

vector<float>* Curve::getSegments() {
	return this->segments;
}

int Curve::getSegmentLength() {
	return this->segments->size() / 2;
}

vector<float>* Curve::getSegmentColors() {
	return this->segmentColors;
}

void Curve::computeCurve(bool INTERNAL) {
	vector<float>* curve;
	int MODE_FACTOR;
	if (INTERNAL) {
		MODE_FACTOR = 1;
	}
	else {
		MODE_FACTOR = -1;
	}
	curve = new vector<float>();
	
	float x, y;
	for (int i = 0; i < this->getSegmentLength() - 1; i++) {
		this->calculateCurvePoint(i, i + 1, MODE_FACTOR, x, y);
		curve->push_back(x);
		curve->push_back(y);
	}

	this->calculateCurvePoint(this->getSegmentLength() - 1, 0, MODE_FACTOR, x, y);
	curve->push_back(x);
	curve->push_back(y);

	if (INTERNAL) {
		this->internal = curve;
	} else {
		this->external = curve;
	}
}

void Curve::calculateCurvePoint(int i0, int i1, int MODE_FACTOR, float &x, float &y) {
	float x0 = this->segments->at(2 * i0);
	float y0 = this->segments->at(2 * i0 + 1);
	float x1 = this->segments->at(2 * i1);
	float y1 = this->segments->at(2 * i1 + 1);
	float w = x1 - x0;
	float h = y1 - y0;
	float theta = atan(h / w);
	float angle;
	if (w < 0) angle = theta - MODE_FACTOR * (M_PI / 2);
	else angle = theta + MODE_FACTOR * (M_PI / 2);
	x = cos(angle) * CURVES_DISTANCE + x0;
	y = sin(angle) * CURVES_DISTANCE + y0;
}


vector<float>* Curve::getInternalCurve() {
	return this->internal;
}

vector<float>* Curve::getExternalCurve() {
	return this->external;
}



void Curve::exportOBJ(string filename) {
	ofstream objFile;
	objFile.open(filename);
	objFile << "mtllib pista.mtl\n";
	int curveLength = this->getSegmentLength();
	for (int i = 0; i < curveLength; i++) {
		objFile << "v " << this->internal->at(2 * i) << " " << (this->segmentColors->at(i) * MAX_Z) << " " << this->internal->at(2 * i + 1) << "\n";
	}
	for (int i = 0; i < curveLength; i++) {
		objFile << "v " << this->external->at(2 * i) << " " << (this->segmentColors->at(i) * MAX_Z) << " " << this->external->at(2 * i + 1) << "\n";
	}
	objFile << "vt 0 0\nvt 1 0\nvt 1 1\nvt 0 1\nvn 0 1 0\n";
	objFile << "g pista\n";
	objFile << "usemtl pista\n";
	
	for (int i = 1; i < curveLength; i++) {
		objFile << "f " << i << "/3/1 " << (i + curveLength) << "/1/1 " << (i + curveLength + 1) << "/2/1\n";
		objFile << "f " << (i + curveLength + 1) << "/2/1 " << (i + 1) << "/4/1 " << i << "/3/1\n";
	}
	int i = 0;
	objFile << "f " << curveLength << "/3/1 " << (2*curveLength) << "/1/1 " << (curveLength + 1) << "/2/1\n";
	objFile << "f " << (curveLength + 1) << "/2/1 " << (1) << "/4/1 " << curveLength << "/3/1\n";
	objFile.close();
}

void Curve::exportControlPoints(string filename) {
	ofstream objFile;
	objFile.open(filename);
	for (int i = 0; i < this->getSegmentLength(); i++) {
		objFile << this->segments->at(2 * i) << " " << (this->segmentColors->at(i) * MAX_Z) << " " << this->segments->at(2 * i + 1) << " ";
	}
	objFile.close();
}

void Curve::exportCurve(string filename) {
	this->exportOBJ(filename + ".obj");
	this->exportControlPoints(filename + ".curve");
}