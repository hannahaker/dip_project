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

#ifndef FACEDETECTION_H
#define FACEDETECTION_H
#pragma once
#include <qtimagelib.h>
#include "tsObject.h"
#include "hausdorff.h"
#include <iostream>
#include <cmath>
//#include "qtip.h"

class faceDetection: public QObject
{
Q_OBJECT; // QOBJECT macro must appear at start of class

public slots:
// the following methods add actions to the menu bar
// note the prototype and naming convention: Menu_Menubar_Menuitem
// all menu items must be prototyped in the "public slots" section
    bool Menu_Hausdorff_RunHausdorff( Image& image1 );
    bool Menu_Preproccessing_SuccessiveThinning( Image &image );
    bool Menu_Preproccessing_BinaryThreshold( Image &image );
    bool Menu_Preprocessing_Sobel( Image &image );
    bool Menu_Preproccessing_GaussianSmoothing( Image &image );

};

bool thin( image );

#endif // FACEDETECTION_H

