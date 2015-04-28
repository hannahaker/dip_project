#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H
#include "facedetection.h"


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

#endif // IMAGEMODEL_H
