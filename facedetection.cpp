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



#include "facedetection.h"
#include "qtip.h"
#include "tsObject.h"
#include "hausdorff.h"
#include <iostream>
#include <cmath>

/**************************************************************************//**
 * @author Dr. John Weiss
 *
 * @par Description:
 * This function calls the imageLib GUI
 *
 * @param[in] argc - argument count from the command line
 * @param[in] argv - c string from command line
 *
 * @returns 0   program executed successfully
 *****************************************************************************/
int main( int argc, char *argv[] )
{
    MyApp obj;
    ImageApp app( argc, argv );
    app.AddActions( &obj );
    return app.Start();
}

// the following methods add actions to the menu bar
// note the prototype and naming convention: Menu_Menubar_Menuitem

// ----------------------------------- Hausdorff ------------------------------------


/**************************************************************************//**
 * @author Zachary Pierson
 *
 * @par Description:
 * GUI Function that allows the user to define two images. The first image
 * is received as a parameter passed to this function, where we will be
 * searching matches to our model. The second image defined by the user is the
 * model that will be used to search the target space. This is defined through
 * a pop up window requesting the user to select from a list of images or
 * browse to the desired model
 *
 * @param[in,out] image1 - the target image to search
 *
 * @returns true   successfully completed function
 * @returns false  unable to complete function
 *****************************************************************************/
bool MyApp::Menu_Run_Hausdorff( Image& image1 )
{
    Image image2;
    if ( !getParams( image2 ) ) return false;

    // setup image_model objects to store image, voronoi surface of image,
    // and other properties of the image
    Image_Model target(image1);
    Image_Model model(image2);

    // intantiate list containers to hold HD distance values
    vector<double> distances;

    // intantiate list containers to hold objects containing matching
    // transformations
    vector<tsObject> matches;

    // intantiate list containers to hold validated transformations
    // validation occurs through the partial box-reverse HD
    vector<tsObject> goodMatches;

    // setup the forward partial distance index to ascending list of
    // distances. example: f * sizeOfList = nth index to compare
    double f = .80;

    //smooth image to get better result of sobel edge detection
    smoothGaussian( target.image, 2 );
    //displayImage(target.image, "Gaussian Smoothed Image");

    // find edges
    sobel( target.image );
    //displayImage(target.image, "Sobel Image");

    // prepare image for thinning
    binaryThreshold( target.image, 50 );
    //displayImage(target.image, "Thresholded Image");

    // Successive thinnings until there is no more cahnge.
    //This allows a smaller set of points to work with for the Hausdorff
    target.thin();
    //displayImage(target.image, "Thinnned image");

    // get the target points from the image
    if( target.init_points() == 0 )
        return false;

    // calculate the voronoi surface of both target and model
    target.init_voronoi_mask();
    model.init_voronoi_mask();
//    target.init_voronoi();
//    target.display_voronoi();

    int thresh = 10;
        if ( !Dialog( "pixel error threshold" ).Add( thresh, "Threshold", 1, 30 ).Show() )
            return false;

    // store matches found using partial forward HD
    matches = decomp(target, model, thresh, 1.0, 1);

    // store matches validated through the partial box-reverse HD
    goodMatches = validMatches(matches, target, model, thresh, 0.95);


    /*printf("---------- Start -------------\n");
    //hausdorff using voronoi surface
    distances = forward_hausdorff(model.points, target);
    printf("model to target .20: %f\n", distances[f*distances.size()]);
    printf("-----------------------------\n");

    //hausdorff using voronoi surface
    distances = reverse_hausdorff(model.points, target);
    printf("model to target .20: %f\n", distances[f*distances.size()]);
    printf("-----------------------------\n");

    //hausdorff withouth voronoi surface
//    distances = directed_hausdorff(model.points, target.points);
//    printf("model to target .20: %f\n", distances[f*distances.size()]);
//    printf("-----------------------------\n");

//    distances = directed_hausdorff(target.points, model.points);
//    printf("target to model .20: %f\n", distances[f*distances.size()]);
//    printf("-----------------------------\n");
    printf("--------- End ----------------\n");
*/


    // Draw boxes around all the matches
    draw_box(image1, goodMatches, model.rows, model.cols);

    return true;
}
