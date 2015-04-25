
// next two lines are typical
#pragma once
#include <qtimagelib.h>
#include <vector>
#include <queue>
#include <iostream>
#include <cmath>
#include <cfloat>   //DBL_MAX
#include <cstdio>
#include <cstdlib>
#include <algorithm>

using namespace std;

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

class Image_Model
{
    public:
        Image_Model(Image& img);
        ~Image_Model();

        unsigned int init_points();
        int push_neighbors(queue<point> &q, point p);
        void init_voronoi();
        void init_voronoi_mask();
        void display_voronoi();
        bool match(Image& img);
        bool thin();

        vector<point> points;
        double ** voronoi;
        unsigned int cols;
        unsigned int rows;
        Image image;
        Transformation trans;
};

double euclidean_dist(point A, point B);
vector<double> forward_hausdorff(vector<point>& pts, double** surface);
vector<double> reverse_hausdorff(vector<point>& pts, double** surface, int Xmax, int Ymax );
vector<double> directed_hausdorff( vector<point>& B, vector<point>& A);
bool equal(Image& image1, Image& image2);

//queue<point> decomp(Image_Model& image, Image_Model& model, int scale);
//int calcGamma( tsObject tsObj);
