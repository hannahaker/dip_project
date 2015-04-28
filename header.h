#ifndef HEADER_H
#define HEADER_H
/*************************************************************************//**
 * @file
 *
 * @brief This file contains the main function and other functions to
 *        read puzzle criteria from file, solve knights tour
 *        puzzle, and output solution to file.
 *
 * @mainpage Program 3 - Knights Tour
 *
 * @section course_section Course Information
 *
 * @author Steven Huerta
 *
 * @date May 8, 2013
 *
 * @par Professor:
 *         Roger Schrader
 *
 * @par Course:
 *         CSC 250 - M001 - 10:00am
 *
 * @par Location:
 *         Classroom Building - 116
 *
 * @section program_section Program Information
 *
 * @details This program will emulate a knight piece on a chess board at a
 * starting position, and find a solution to land on every square of the
 * board only once. Therefore, a knight's tour. The program will read in a
 * file containing conditions for each puzzle. The conditions are: 1) The
 * dimension of the board 2) The start row and column from which to move.
 *
 * The program will then permute the moves on the chess board for a knight in
 * a clockwise manner, starting from 11 o'clock and continuing through to the
 * 10 o'clock position. The program will return the first solution to the
 * puzzle. Otherwise, it will continue until all possible moves have been
 * exhausted. The result will be output to a file with the same name as the
 * input file, but with "out.txt" appended to the end.
 *
 * The program will then look for the next puzzle in the input file, and
 * continue through each puzzle until all puzzles have been processed.
 *
 *
 * @section compile_section Compiling and Usage
 *
 * @par Compiling Instructions:
 *      None
 *
 * @par Usage:
   @verbatim
   c:\> prog3.exe inputfile.txt
   d:\> c:\bin\<prog>.exe inputfile.txt

   [inputfile.txt] file that contains puzzles


   @endverbatim
 *
 * @section todo_bugs_modification_section Todo, Bugs, and Modifications
 *
 *
 * @par Modifications and Development Timeline:
   @verbatim
   Date          Modification
   ------------  --------------------------------------------------------------
   Mar 30, 2013  Wrote main , open and read from input file
                 Wrote functions to create and initialize board
   Mar 31, 2013  Wrote functions to move knight
                 Modified functions to permute solution to knight tour
                 Wrote functions create and output to file
   Apr  8, 2013  Finish documentation
   @endverbatim
 *
 *****************************************************************************/

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

/*********************** facedetection.cpp ***************************/
class faceDetection: public QObject
{
Q_OBJECT; // QOBJECT macro must appear at start of class

public slots:
// the following methods add actions to the menu bar
// note the prototype and naming convention: Menu_Menubar_Menuitem
// all menu items must be prototyped in the "public slots" section
    bool Menu_Hausdorff_RunHausdorff( Image& image1 );
    bool Menu_Preprocessing_SuccessiveThinning( Image &image );
    bool Menu_Preprocessing_BinaryThreshold( Image &image );
    bool Menu_Preprocessing_Sobel( Image &image );
    bool Menu_Preprocessing_GaussianSmoothing( Image &image );

};


/*********************** imagemodel.cpp ***************************/
class Image_Model
{
    public:
        Image_Model(Image& img);
        ~Image_Model();

        unsigned int init_points();
        int push_neighbors( queue<point> &q, point p);
        void init_voronoi();
        void init_voronoi_mask();
        void display_voronoi();
        bool match(Image& img);

        vector<point> points;
        double ** voronoi;
        unsigned int cols;
        unsigned int rows;
        Image image;
        Transformation trans;
};

/*********************** tsobject.cpp ***************************/
class tsObject
{
public:
    // METHODS

    // CONSTRUCTORS
    tsObject( int tXMin, int tXMax, int tYMin, int tYMax,
              int sXMin = 1, int sXMax = 1, int sYMin = 1, int sYMax = 1) ;

    int transXMin ;
    int transXMax ;
    int transXCenter ;

    int transYMin ;
    int transYMax ;
    int transYCenter ;

    int scaleXMin ;
    int scaleXMax ;
    int scaleXCenter ;

    int scaleYMin ;
    int scaleYMax ;
    int scaleYCenter ;

    int rotThetaMin ;
    int rotThetaMax ;
    int rotThetaCenter ;

    tsObject operator= ( const tsObject &rhs);
};

/*********************** hausdorff.cpp ***************************/
bool thin( Image& image );
bool equal(Image& image1, Image& image2);

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

#endif // HEADER_H
