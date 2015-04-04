
// next two lines are typical
#pragma once
#include <qtimagelib.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <cstdio>
#include <algorithm>

using namespace std;

struct point
{
    int x;
    int y;
};

class Image_Model
{
    public:
        Image_Model(Image& img);

        unsigned int init_points();
        bool match(Image& img);
        bool thin();

        vector<point> points;
        Image image;
};

double euclidean_dist(point A, point B);
vector<double> directed_hausdorff(vector<point>& model, vector<point>& target);
bool equal(Image& image1, Image& image2);
