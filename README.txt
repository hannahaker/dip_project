                Hausdorff Face Detection
                    April 27, 2015

Institution: South Dakota School of Mines and Technology
Authors: Hanna Aker, Steven Huerta, & Zachary Pierson
Instructor: Dr. John Weiss
Class: CSC442 - Digital Image Processing


CONTENTS OF THIS FILE
---------------------
    I. Introduction
   II. Requirements
  III. Installation
   IV. User Manual
    V. Working Examples
   VI. References


I. INTRODUCTION
---------------
The main idea behind the hausdorff face detection is finding the distance
between two sets of points. This Method is explaied in detail by Rucklidge
along with efficient implimentation techniques such as the voronoi surface,
early rejection, and early acceptance. If you would like more information
about the hausdorff face detection, check out the papers under section VI
References.

For this project, We implement the following:
    1. Hausdorff distance calculations using the voronoi surface  
    2. 2D transformation space solely consisting of translations
    3. Cell Decomposition in the transformation space

Eventually we hope to implement the following:
    1. Early Rejection and Early Acceptance
    2. Include scaling and rotation for a 6D transformation space
    3. Parallelization of the project


II. REQUIREMENTS
----------------
    *Linix or Windows OS
    *qt4
    *Dr. Weiss's QtImageLib
    *g++


III. INSTALLATION
-----------------
Installation guide can be found at www.mcs.sdsmt.edu/csc442/Resources

Windows Installation:

    1. Download qt-sdk-win-opensource-2010.05.exe, qtimagelib-1.0.6.msi,
    and qtimagelib-1.0.7.zip from the course website.

    2. Install Qt:
    Double-click on qt-sdk-win-opensource-2010.05.exe and use the defaults.

    3. Install QtImageLib:
    Double-click on qtimagelib-1.0.6.msi and use the defaults

    4. Update QtImageLib:
    32-bit OS: move qtimagelib-1.0.7.zip to: 
        C:\Program Files\Common File\Qt ImageLib
    64-bit OS: move qtimagelib-1.0.7.zip to: 
        C:\Program Files (x86)\Common File\Qt ImageLib

    Unzip qtimagelib-1.0.7.zip, overwriting previous folder contents.

    5. Final tweaks:
    a. Add C:\Qt\2010.05\mingw\bin and C:\Qt\2010.05\qt\bin to your PATH
     Click the Windows Start Menu, Accessories, Right-click on Command
        Prompt, select Run as Administrator, and type:

        Setx path "%path%;c:\qt\2010.05\mingw\bin" /m

    b. Change to C:\Qt\2010.05\mingw\bin, and copy mingw32-make.exe to
        make.exe

Fedora Installation:

    # install QT
    sudo yum insrtall qt qt-devel

    #install QtImageLib
    sudo yum install qtimagelib qtimagelib-devel qtimagelib-docs

IV. USER MANUAL
---------------
Compile Instructions:
    1. run qmake to generate the Makefile
    2. run make to compile the program

The QtImageLib will automatically generate a window with basic functionality.
To run the huasdorff face detection, navigate to the menu bar and click
"Run" -> "hausdorff".


























