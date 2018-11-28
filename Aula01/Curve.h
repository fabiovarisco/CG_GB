#pragma once

#ifndef CURVE
#define CURVE


#define _USE_MATH_DEFINES

#include <vector>
#include <math.h>
#include <iostream>
#include <fstream>

using namespace std;

static const float CURVE_INC = 0.05;
static const int CURVES_DISTANCE = 5;
static const int MAX_Z = 50;
/*
struct segment {
	float *points;
};
*/
class Curve
{

private: 
	vector<float>* points;
	vector<float>* zoomLevels;
	vector<float>* segments;
	vector<float> *internal, *external;
	vector<float>* segmentColors;
	void exportOBJ(string filename);
	void exportControlPoints(string filename);
public:
	Curve();
	~Curve();
	int SEGMENT_LENGTH, SEGMENT_SIZE;
	bool DISPLAY_CURVE = false;
	void addPoint(float x, float y);
	vector<float>* getPoints();
	vector<float>* getZoomLevels();
	int getLength();
	void setZoomLevel(int i, float zoomLevel);
	void increaseZoom(int i);
	void decreaseZoom(int i);
	void getPoint(int i, float &x, float &y);
	int findNearest(float x, float y);
	void calculateSegment(int i0, int i1, int i2, int i3);
	void calculateCurve();
	void computeCurve(bool INTERNAL);
	void calculateCurvePoint(int i0, int i1, int MODE_FACTOR, float &x, float &y);
	vector<float>* getSegments();
	vector<float>* getSegmentColors();
	vector<float>* getInternalCurve();
	vector<float>* getExternalCurve();
	int getSegmentLength();
	void exportCurve(string filename);
	
};


#endif // !CURVE