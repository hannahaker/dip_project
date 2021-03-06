/************************************************************************
   Program: Image Processing Final Project
   Author: Steven Heurta, Zach Pierson, Hannah Aker
   Class: Digital Image Processing
   Instructor: Dr. John Weiss
   Date: 4/27/2015
   Description:
        This project will consist of face detection using the Hausdorff
    distance to calculate a match between a simple template and a constrained image (an edge
    operator will first be applied to both the template and image). The simple template will
    consist of a single generic face instead of a template formed by many examples of possible
    faces. The input image will consist of a set size (n x m) of pixels and will or will not contain
    a face from the top of the head to the shoulders. The formula for the Hausdorff distance
    explained above will be the main focus of the project and will not include transformations
    of any kind.
        The best case project outcome will consist of face detection using an effcient Hausdorff
    algorithm to find all faces in a relatively unstructured environment. The template used to
    find the faces will be constructed in a way that will incorporate most of the features any
    human face would have. The input image will not be constrained to a set size and could
    consist of multiple human faces. An effcient algorithm for template transformations will be
    used to minimize the Hausdorff distance between the image and the template.
   Input:
   Output:
   Compilation instructions:
   Usage:
   Known bugs/missing features:
   Todo:

 ************************************************************************/


// next two lines are typical
#pragma once
#include <qtimagelib.h>
#include <vector>
#include <queue>
#include <time.h>
#include <iostream>
#include <cmath>
#include <cfloat>   //DBL_MAX
#include <cstdio>
#include <cstdlib>
#include <algorithm>

//#include <stdio.h>

#include "tsObject.h"
#include "imagemodel.h"
#include "facedetection.h"

using namespace std;

/*********************** structures ***************************/

struct point
{
    int x;
    int y;
};

struct Transformation
{
    int x;
    int y;
    double scale;
};


 /*********************** class interfaces ***************************/



 /*********************** function prototypes ***************************/

double euclidean_dist(point A, point B);
vector<double> forward_hausdorff(vector<point> modelPoints, Image_Model& target);
vector<double> reverse_hausdorff(tsObject ts, Image_Model& target, Image_Model& model);
vector<double> directed_hausdorff( vector<point>& B, vector<point>& A);

vector<tsObject> decomp(Image_Model& target, Image_Model& model, float pixelErrorThresh, float percentList, int alpha);
double calcGamma( tsObject tsObj, int xMax, int yMax);
bool isInteresting( tsObject & transSpace, Image_Model & target, Image_Model & model, float percentList, int threshold );
vector<tsObject> divide( tsObject trfSpace );
vector<point> transform(tsObject ts, Image_Model & transImage );
void draw_box(Image& image, vector<tsObject> &ts, int rows, int cols);
vector<tsObject> validMatches ( vector<tsObject> & matches, Image_Model & target, Image_Model & model, double thresh, int percentList );
