#ifndef HEADER_H
#define HEADER_H

#pragma once
#include <qtimagelib.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <queue>
#include <time.h>
#include <cfloat>   //DBL_MAX
#include <cstdio>
#include <cstdlib>
#include <algorithm>

using namespace std;


/*!
 * @brief Object to contain x,y coordinates
 */
struct point
{
    int x;
    int y;
};


/*********************** facedetection.cpp ***************************/
class faceDetection: public QObject
{
Q_OBJECT; // QOBJECT macro must appear at start of class

public slots:
// the following methods add actions to the menu bar
// note the prototype and naming convention: Menu_Menubar_Menuitem
// all menu items must be prototyped in the "public slots" section

    /*!
     * @brief Menu_Hausdorff_RunHausdorff to run hausdorff
     * comparison
     */
    bool Menu_Hausdorff_RunHausdorff( Image& image1 );

    /*!
     * @brief Menu_Preprocessing_SuccessiveThinning to apply
     * successive thinning to image
     */
    bool Menu_Preprocessing_SuccessiveThinning( Image &image );

    /*!
     * @brief Menu_Preprocessing_BinaryThreshold to apply binary
     * threshold to image
     */
    bool Menu_Preprocessing_BinaryThreshold( Image &image );

    /*!
     * @brief Menu_Preprocessing_Sobel to apply edge detector
     * to image
     */
    bool Menu_Preprocessing_Sobel( Image &image );

    /*!
     * @brief Menu_Preprocessing_GaussianSmoothing to
     * apply smoothing to image
     */
    bool Menu_Preprocessing_GaussianSmoothing( Image &image );

};


/*********************** imagemodel.cpp ***************************/

/*!
 * @brief Image_Model holds image and image properties
 */
class Image_Model
{
    public:
        /*!
         * @brief CONSTRUCTOR
         */
        Image_Model(Image& img);

        /*!
         * @brief DESTRUCTOR
         */
        ~Image_Model();

        /*!
         * @brief init_points sets points to vector and
         * returns size of vector
         */
        unsigned int init_points();

        /*!
         * @brief push_neighbors, NOT IMPLEMENTED
         */
        int push_neighbors( queue<point> &q, point p);

        /*!
         * @brief init_voronoi, NOT IMPLEMENTED
         */
        void init_voronoi();

        /*!
         * @brief init_voronoi_mask creates the 2D array of the
         * voronoi surface
         */
        void init_voronoi_mask();

        /*!
         * @brief display_voronoi outputs the voronoi surface
         */
        void display_voronoi();

        /*!
         * @brief match Rescale image close to the size of the
         * given image
         */
        bool match(Image& img);

        vector<point> points;   // Set of coordinates of image
        double ** voronoi;      // Array holding voronoi surface
        unsigned int cols;      // width of the image
        unsigned int rows;      // height of the image
        Image image;            // holds a copy of the image

};

/*********************** tsobject.cpp ***************************/
class tsObject
{
public:
    // METHODS

    // CONSTRUCTORS
    tsObject( int tXMin, int tXMax, int tYMin, int tYMax,
              int sXMin = 1, int sXMax = 1, int sYMin = 1, int sYMax = 1) ;

    // Set the bounds of translations in the x dimension
    int transXMin ;
    int transXMax ;
    int transXCenter ;

    // Set the bounds of translation in the y dimension
    int transYMin ;
    int transYMax ;
    int transYCenter ;

    // Set the bounds of scaling in the y dimension
    int scaleXMin ;
    int scaleXMax ;
    int scaleXCenter ;

    // Set the bounds of scaling in the y dimension
    int scaleYMin ;
    int scaleYMax ;
    int scaleYCenter ;

    // Set the bounds of rotations, NOT USED
    int rotThetaMin ;
    int rotThetaMax ;
    int rotThetaCenter ;

    // Overloaded function for equals to copy tsObject objects
    tsObject operator= ( const tsObject &rhs);
};

/*********************** hausdorff.cpp ***************************/
/*!
 * @brief thin applies thinning to the image
 */
bool thin( Image& image );

/*!
 * @brief equal checks if images are equal
 */
bool equal(Image& image1, Image& image2);

/*!
 * @brief euclidean_dist, calculate distance between two points
 */
double euclidean_dist(point A, point B);

/*!
 * @brief forward_hausdorff calculate the foward partial HD
 */
vector<double> forward_hausdorff(vector<point> modelPoints,
                                 Image_Model& target);

/*!
 * @brief reverse_hausdorff calculate the reverse partial HD
 */
vector<double> reverse_hausdorff(tsObject ts, Image_Model& target,
                                 Image_Model& model);

/*!
 * @brief directed_hausdorff, exact distance NOT USED
 */
vector<double> directed_hausdorff( vector<point>& B,
                                   vector<point>& A);

/*!
 * @brief decomp evaluate how to divide tranformation space
 */
vector<tsObject> decomp(Image_Model& target, Image_Model& model,
                        float pixelErrorThresh, float percentList,
                        int alpha);

/*!
 * @brief calcGamma calculates distance of transforms in transform
 * space
 */
double calcGamma( tsObject tsObj, int xMax, int yMax);

/*!
 * @brief isInteresting evaluate if area holds possible matches
 */
bool isInteresting( tsObject & transSpace, Image_Model & target,
                    Image_Model & model, float percentList,
                    int threshold );

/*!
 * @brief divide Divides transform space into subspaces
 */
vector<tsObject> divide( tsObject trfSpace );

/*!
 * @brief transform Applies transform to model points
 */
vector<point> transform(tsObject ts, Image_Model & transImage );

/*!
 * @brief draw_box Draws a box around confirmed matches
 */
void draw_box(Image& image, vector<tsObject> &ts, int rows, int cols);

/*!
 * @brief validMatches evaluates and confirms matches
 */
vector<tsObject> validMatches ( vector<tsObject> & matches,
                                Image_Model & target, Image_Model & model,
                                double thresh, int percentList );

#endif // HEADER_H
