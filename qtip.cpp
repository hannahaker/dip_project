/*
               ***** qtip.cpp *****

Demo program to illustrate most of QtImageLib functionality.

Author: John M. Weiss, Ph.D.
Written Spring 2015 for SDSM&T CSC442/542 DIP course.

Modifications:
*/

// QtImageLib requires interface (qtip.h) as well as implementation (qtip.cpp)
#include "qtip.h"
#include "tsObject.h"
#include "hausdorff.h"
#include <iostream>
#include <cmath>

// main function will always follow this format
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
bool MyApp::Menu_Run_Hausdorff( Image& image1 )
{
    Image image2;
    if ( !getParams( image2 ) ) return false;

    Image_Model target(image1);
    Image_Model model(image2);
    vector<double> distances;
    double f = .80;

    //resize target image to match model image and
    //reinitialize the points
    // Taking out match so we are not resizing image
    //target.match(model.image);

    //smooth image to get better result of sobel edge detection
    smoothGaussian( target.image, 2 );
    displayImage(target.image, "Gaussian Smoothed Image");

    // find edges
    sobel( target.image );
    displayImage(target.image, "Sobel Image");

    // prepare image for thinning
    binaryThreshold( target.image, 50 );
    displayImage(target.image, "Thresholded Image");

    // Successive thinnings until there is no more cahnge.
    //This allows a smaller set of points to work with for the Hausdorff
    target.thin();
    displayImage(target.image, "Thinnned image");

    // get the target points from the image
    if( target.init_points() == 0 )
        return false;

    // calculate the voronoi surface of both target and model
    target.init_voronoi_mask();
    model.init_voronoi_mask();
//    target.init_voronoi();
//    target.display_voronoi();




    printf("---------- Start -------------\n");
    //hausdorff using voronoi surface
    distances = forward_hausdorff(model.points, target.voronoi);
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


//    image.DrawLine(5, 200, 5, 5, Pixel(0,255,0));
    return true;
}

// ----------------------------------- palette manipulations ------------------------------------

// convert a color image to grayscale
bool MyApp::Menu_Palette_Grayscale( Image &image )
{
    grayscale( image );
    return true;
}

// negate the image
bool MyApp::Menu_Palette_Negate( Image &image )
{
    negatePalette( image );
    return true;
}

// adjust image brightness
bool MyApp::Menu_Palette_Brightness( Image &image )
{
    int c = 0;
    if ( !Dialog( "Brightness" ).Add( c, "offset", -255, 255 ).Show() )
        return false;
    imageAddConstant( image, c );
    return true;
}

// adjust image contrast
bool MyApp::Menu_Palette_Contrast( Image &image )
{
    // int imin = 32, imax = 224;
    int imin = 0, imax = 255;
    if ( !Dialog( "endpoints" ).Add( imin, "left", 0, 255 ).Add( imax, "right", 0, 255 ).Show() )
        return false;

    // create lookup table
    byte pal[256];
    for ( int i = 0; i < imin; i++ )
        pal[i] = 0;
    for ( int i = imax + 1; i < 256; i++ )
        pal[i] = 255;
    float slope = 255.0 / (imax - imin );
    for ( int i = imin; i <= imax; i++ )
        pal[i] = static_cast<uchar>( slope * ( i - imin ) + 0.5 );

    // apply palette
    int nrows = image.Height();
    int ncols = image.Width();
    image.ToYIQ();
    for ( int r = 0; r < nrows; r++ )
        for ( int c = 0; c < ncols; c++ )
            image[r][c].yiq.y = pal[ image[r][c].yiq.y ];

    return true;
}

// adjust image gamma
bool MyApp::Menu_Palette_Gamma( Image &image )
{
    double g = 1.0;
    if ( !getParams( g ) ) return false;
    gammaPalette( image, g );
    return true;
}

// apply log transform
bool MyApp::Menu_Palette_Log( Image &image )
{
    logPalette( image );
    return true;
}

// posterize (requantize) the image
bool MyApp::Menu_Palette_Posterize( Image &image )
{
    int n = 8;
    if ( !getParams( n ) ) return false;
    posterizePalette( image, n );
    return true;
}

// apply a discrete pseudocoloring
bool MyApp::Menu_Palette_PseudoColor8( Image &image )
{
    pseudocolor8Palette( image );
    return true;
}

// apply a continuous pseudocoloring
bool MyApp::Menu_Palette_PseudoColorCube( Image &image )
{
    pseudocolorCubePalette( image );
    return true;
}

// apply a continuous pseudocoloring
bool MyApp::Menu_Palette_PseudoColorGW( Image &image )
{
    pseudocolorGWPalette( image );
    return true;
}

// solarization
bool MyApp::Menu_Palette_Solarize( Image &image )
{
    solarizePalette( image );
    return true;
}

// sawtooth scaling
bool MyApp::Menu_Palette_Sawtooth( Image &image )
{
    int n = 8;
    if ( !getParams( n ) ) return false;
    sawtoothPalette( image, n );
    return true;
}

// color sawtooth scaling
bool MyApp::Menu_Palette_SawtoothRGB( Image &image )
{
    sawtoothRGBPalette( image );
    return true;
}

// color sawtooth scaling
bool MyApp::Menu_Palette_SawtoothBGR( Image &image )
{
    sawtoothBGRPalette( image );
    return true;
}

// triangle scaling
bool MyApp::Menu_Palette_Triangle( Image &image )
{
    trianglePalette( image );
    return true;
}

// inverse triangle scaling
bool MyApp::Menu_Palette_InverseTriangle( Image &image )
{
    inverseTrianglePalette( image );
    return true;
}

// zebra scaling
bool MyApp::Menu_Palette_Zebra( Image &image )
{
    zebraPalette( image );
    return true;
}

// random palette (except for 0 and 255)
bool MyApp::Menu_Palette_Random( Image &image )
{
    randomPalette( image );
    return true;
}

// totally random palette
bool MyApp::Menu_Palette_TotallyRandom( Image &image )
{
    totallyRandomPalette( image );
    return true;
}

// almost random palette (more gradual transitions)
bool MyApp::Menu_Palette_AlmostRandom( Image &image )
{
    almostRandomPalette( image );
    return true;
}

// ------------------------------ histogram routines -------------------------------

// display histogram
bool MyApp::Menu_Histogram_Display( Image &image )
{
    displayHistogram( image );
    return true;
}

// histogram contrast stretch
bool MyApp::Menu_Histogram_Stretch( Image &image )
{
    histogramStretch( image );
    return true;
}

// histogram contrast stretch with specified endpoints
bool MyApp::Menu_Histogram_StretchSpecify( Image &image )
{
    double left = 1.0, right = 1.0;
    if ( !Dialog( "endpoints" ).Add( left, "left", 0.0, 100.0 ).Add( right, "right", 0.0, 100.0 ).Show() )
        return false;
    histogramStretchSpecify( image, left, right);
    return true;
}

// histogram equalization
bool MyApp::Menu_Histogram_Equalize( Image &image )
{
    histogramEqualize( image );
    histogramStretch( image );
    return true;
}

// histogram equalization with clipping
bool MyApp::Menu_Histogram_EqualizeClip( Image &image )
{
    double clip = 1.0;
    if ( !getParams( clip ) ) return false;
    histogramEqualizeClip( image, clip );
    histogramStretch( image );
    return true;
}

// adaptive histogram equalization
bool MyApp::Menu_Histogram_AdaptiveEqualize( Image &image )
{
    int w = 15;
    if ( !getParams( w ) ) return false;
    adaptiveHistogramEqualize( image, w );
    return true;
}

// adaptive contrast stretch
bool MyApp::Menu_Histogram_AdaptiveContrastStretch( Image &image )
{
    int w = 15;
    if ( !getParams( w ) ) return false;
    adaptiveContrastStretch( image, w );
    return true;
}

// histogram matching
bool MyApp::Menu_Histogram_Match( Image &image1 )
{
    Image image2;
    if ( !getParams( image2 ) ) return false;
    histogramMatch( image1, image2 );
    return true;
}

// ----------------------------------- spatial filtering ------------------------------------

// smooth the image (3x3 center-weighted smoothing filter)
bool MyApp::Menu_Filter_Smooth( Image &image )
{
    smooth( image );
    return true;
}

// sharpen the image
bool MyApp::Menu_Filter_Sharpen( Image &image )
{
    sharpen( image );
    return true;
}

// neighborhood average
bool MyApp::Menu_Filter_Average( Image &image )
{
    int w = 3;
    if ( !getParams( w ) ) return false;
    average( image, w );
    return true;
}

// neighborhood weighted average
bool MyApp::Menu_Filter_WeightedAverage( Image &image )
{
    int w = 3;
    if ( !getParams( w ) ) return false;
    weightedAverage( image, w );
    return true;
}

// Gaussian smoothing filter
bool MyApp::Menu_Filter_GaussianSmoothing( Image &image )
{
    double sigma = 2.0;
    if ( !getParams( sigma ) ) return false;
    smoothGaussian( image, sigma );
    return true;
}

// neighborhood min
bool MyApp::Menu_Filter_Minimum( Image &image )
{
    int w = 3;
    if ( !getParams( w ) ) return false;
    minimum( image, w );
    return true;
}

// neighborhood max
bool MyApp::Menu_Filter_Maximum( Image &image )
{
    int w = 3;
    if ( !getParams( w ) )
        return false;
    maximum( image, w );
    return true;
}

// neighborhood range
bool MyApp::Menu_Filter_Range( Image &image )
{
    int w = 3;
    if ( !getParams( w ) ) return false;
    range( image, w );
    return true;
}

// 3x3 plus-shaped median filter
bool MyApp::Menu_Filter_MedianPlus( Image &image )
{
    medianPlus( image );
    return true;
}

// 3x3 median filter
bool MyApp::Menu_Filter_MedianBox( Image &image )
{
    medianBox( image );
    return true;
}

// neighborhood median filter
bool MyApp::Menu_Filter_Median( Image &image )
{
    int w = 3;
    if ( !getParams( w ) ) return false;
    medianFilter( image, w );
    return true;
}

// separable neighborhood median filter
bool MyApp::Menu_Filter_SeparableMedian( Image &image )
{
    int w = 3;
    if ( !getParams( w ) ) return false;
    separableMedianFilter( image, w );
    return true;
}

// unsharp masking
bool MyApp::Menu_Filter_UnsharpMasking( Image &image )
{
    unsharp( image );
    return true;
}

// parametric unsharp masking
bool MyApp::Menu_Filter_UnsharpParametric( Image &image )
{
    double a = 2.0;
    if ( !getParams( a ) ) return false;
    unsharpParametric( image, a );
    return true;
}

// emboss the image
bool MyApp::Menu_Filter_Emboss( Image &image )
{
    emboss( image );
    return true;
}

// neighborhood variance
bool MyApp::Menu_Filter_Variance( Image &image )
{
    int w = 3;
    if ( !getParams( w ) ) return false;
    variance( image, w );
    return true;
}

// neighborhood standard deviation
bool MyApp::Menu_Filter_StandardDeviation( Image &image )
{
    int w = 3;
    if ( !getParams( w ) ) return false;
    standardDeviation( image, w );
    return true;
}

// neighborhood standard deviation
bool MyApp::Menu_Filter_StatisticalDifference( Image &image )
{
    int w = 15;
    double k = 1.0;
    if ( !getParams( w, k ) ) return false;
    statisticalDifference( image, w, k );
    return true;
}


// ----------------------------------- frequency filtering ------------------------------------

// display DFT magnitude
bool MyApp::Menu_Frequency_Magnitude( Image &image )
{
    dftMagnitude( image );
    return true;
}

// display DFT phase
bool MyApp::Menu_Frequency_Phase( Image &image )
{
    dftPhase( image );
    return true;
}

// ideal low pass filter
bool MyApp::Menu_Frequency_IdealLowPassFilter( Image &image )
{
    double cutoff = 10.0;
    if ( !getParams( cutoff ) ) return false;
    idealLowPassFilter( image, cutoff );
    return true;
}

// ideal high pass filter
bool MyApp::Menu_Frequency_IdealHighPassFilter( Image &image )
{
    double cutoff = 10.0;
    if ( !getParams( cutoff ) ) return false;
    idealHighPassFilter( image, cutoff );
    return true;
}

// ideal high frequency emphasis filter
bool MyApp::Menu_Frequency_IdealEmphasisFilter( Image &image )
{
    double cutoff = 10.0, lowscale = 0.5, highscale = 2.0;
    if ( !getParams( cutoff, lowscale, highscale ) ) return false;
    idealEmphasisFilter( image, cutoff, lowscale, highscale );
    return true;
}

// Gaussian low pass filter
bool MyApp::Menu_Frequency_GaussianLowPassFilter( Image &image )
{
    double cutoff = 10.0;
    if ( !getParams( cutoff ) ) return false;
    gaussianLowPassFilter( image, cutoff );
    return true;
}

// Gaussian high pass filter
bool MyApp::Menu_Frequency_GaussianHighPassFilter( Image &image )
{
    double cutoff = 10.0;
    if ( !getParams( cutoff ) ) return false;
    gaussianHighPassFilter( image, cutoff );
    return true;
}

// Gaussian high frequency emphasis filter
bool MyApp::Menu_Frequency_GaussianEmphasisFilter( Image &image )
{
    double cutoff = 10.0, boost = 2.0;
    if ( !getParams( cutoff, boost ) ) return false;
    gaussianEmphasisFilter( image, cutoff, boost );
    return true;
}

// Butterworth low pass filter
bool MyApp::Menu_Frequency_ButterworthLowPassFilter( Image &image )
{
    double cutoff = 10.0;
    if ( !getParams( cutoff ) ) return false;
    butterworthLowPassFilter( image, cutoff );
    return true;
}

// Butterworth high pass filter
bool MyApp::Menu_Frequency_ButterworthHighPassFilter( Image &image )
{
    double cutoff = 10.0;
    if ( !getParams( cutoff ) ) return false;
    butterworthHighPassFilter( image, cutoff );
    return true;
}

// Butterworth high frequency emphasis filter
bool MyApp::Menu_Frequency_ButterworthEmphasisFilter( Image &image )
{
    double cutoff = 10.0, boost = 2.0;
    if ( !getParams( cutoff, boost ) ) return false;
    butterworthEmphasisFilter( image, cutoff, boost );
    return true;
}

// homomorphic high frequency emphasis filter
bool MyApp::Menu_Frequency_HomomorphicFilter( Image &image )
{
    double cutoff = 10.0, boost = 2.0;
    if ( !getParams( cutoff, boost ) ) return false;
    homomorphicFilter( image, cutoff, boost );
    return true;
}

// ideal band pass filter
bool MyApp::Menu_Frequency_IdealBandPassFilter( Image &image )
{
    double u1 = 0.0, u2 = 100.0, v1 = 0.0, v2 = 100.0;
    if ( !Dialog( "cutoffs" ).Add( u1, "u1", 0, 100 ).Add( u2, "u2", 0, 100 ).
        Add( v1, "v1", 0, 100 ).Add( v2, "v2", 0, 100 ).Show() )
        return false;
    idealBandPassFilter( image, u1, u2, v1, v2 );
    return true;
}
// ideal band reject (notch) filter
bool MyApp::Menu_Frequency_NotchFilter( Image &image )
{
    double u1 = 0.0, u2 = 100.0, v1 = 0.0, v2 = 100.0;
    if ( !Dialog( "cutoffs" ).Add( u1, "u1", 0, 100 ).Add( u2, "u2", 0, 100 ).
        Add( v1, "v1", 0, 100 ).Add( v2, "v2", 0, 100 ).Show() )
        return false;
    idealBandRejectFilter( image, u1, u2, v1, v2 );
    return true;
}

// -------------------------- edge detection -----------------------------

// Sobel edge detection
bool MyApp::Menu_Edge_Sobel( Image &image )
{
    sobel( image );
    return true;
}

// Sobel edge detection
bool MyApp::Menu_Edge_SobelMagnitude( Image &image )
{
    sobelMagnitude( image );
    return true;
}

// Sobel edge detection
bool MyApp::Menu_Edge_SobelDirection( Image &image )
{
    sobelDirection( image );
    return true;
}

// Prewitt edge detection
bool MyApp::Menu_Edge_Prewitt( Image &image )
{
    prewitt( image );
    return true;
}

// Prewitt edge detection
bool MyApp::Menu_Edge_PrewittMagnitude( Image &image )
{
    prewittMagnitude( image );
    return true;
}

// Prewitt edge detection
bool MyApp::Menu_Edge_PrewittDirection( Image &image )
{
    prewittDirection( image );
    return true;
}

// FreiChen edge detection
bool MyApp::Menu_Edge_FreiChen( Image &image )
{
    freichen( image );
    return true;
}

// FreiChen edge detection
bool MyApp::Menu_Edge_FreiChenMagnitude( Image &image )
{
    freichenMagnitude( image );
    return true;
}

// FreiChen edge detection
bool MyApp::Menu_Edge_FreiChenDirection( Image &image )
{
    freichenDirection( image );
    return true;
}

// Canny edge detection
bool MyApp::Menu_Edge_Canny( Image &image )
{
    double sigma = 2.0;
    if ( !getParams( sigma ) )
        return false;
    canny( image, sigma );
    return true;
}

// Laplacian edge detection
bool MyApp::Menu_Edge_Laplacian( Image &image )
{
    laplacian( image );
    return true;
}

// Laplacian-Of-Gaussian edge detection
bool MyApp::Menu_Edge_LaplacianOfGaussian( Image &image )
{
    double sigma = 2.0;
    if ( !getParams( sigma ) )
        return false;
    laplacianOfGaussian( image, sigma );
    return true;
}

// detect zero crossings
bool MyApp::Menu_Edge_ZeroCrossings( Image &image )
{
    zeroCrossings( image );
    return true;
}

// Kirsch edge detection
bool MyApp::Menu_Edge_KirschMagnitude( Image &image )
{
    kirschMagnitude( image );
    return true;
}

// Kirsch edge detection
bool MyApp::Menu_Edge_KirschDirection( Image &image )
{
    kirschDirection( image );
    return true;
}

// horizontal edges
bool MyApp::Menu_Edge_Horizontal( Image &image )
{
    prewittHorizontal( image );
    return true;
}

// vertical edges
bool MyApp::Menu_Edge_Vertical( Image &image )
{
    prewittVertical( image );
    return true;
}

// Roberts edge operator
bool MyApp::Menu_Edge_Roberts( Image &image )
{
    roberts( image );
    return true;
}

// Robinson edge detection
bool MyApp::Menu_Edge_RobinsonMagnitude( Image &image )
{
    robinsonMagnitude( image );
    return true;
}

// Robinson edge detection
bool MyApp::Menu_Edge_RobinsonDirection( Image &image )
{
    robinsonDirection( image );
    return true;
}

// Nevatia-Babu edge detection
bool MyApp::Menu_Edge_NevatiaBabu( Image &image )
{
    nevatiaBabu( image );
    return true;
}

// -------------------------- binary morphological filtering ------------------------------

// binary 3x3 diamond-shaped dilation
bool MyApp::Menu_BinaryMorphology_Dilate3x3( Image &image )
{
    dilate( image );
    return true;
}

// binary 3x3 diamond-shaped erosion
bool MyApp::Menu_BinaryMorphology_Erode3x3( Image &image )
{
    erode( image );
    return true;
}

// binary neighborhood dilation
bool MyApp::Menu_BinaryMorphology_Dilate( Image &image )
{
    int w = 3;
    if ( !getParams( w ) ) return false;
    int *mask = new int [ w * w ];
    for ( int i = 0; i < w * w; i++ ) mask[i] = 1;
    dilate2D( image, mask, w, w );
    delete [] mask;
    return true;
}

// binary neighborhood erosion
bool MyApp::Menu_BinaryMorphology_Erode( Image &image )
{
    int w = 3;
    if ( !getParams( w ) ) return false;
    int *mask = new int [ w * w ];
    for ( int i = 0; i < w * w; i++ ) mask[i] = 1;
    erode2D( image, mask, w, w );
    delete [] mask;
    return true;
}

// binary neighborhood opening
bool MyApp::Menu_BinaryMorphology_Open( Image &image )
{
    int w = 3;
    if ( !getParams( w ) ) return false;
    int *mask = new int [ w * w ];
    for ( int i = 0; i < w * w; i++ ) mask[i] = 1;
    open2D( image, mask, w, w );
    delete [] mask;
    return true;
}

// binary neighborhood closing
bool MyApp::Menu_BinaryMorphology_Close( Image &image )
{
    int w = 3;
    if ( !getParams( w ) ) return false;
    int *mask = new int [ w * w ];
    for ( int i = 0; i < w * w; i++ ) mask[i] = 1;
    close2D( image, mask, w, w );
    delete [] mask;
    return true;
}

// binary neighborhood smoothing (opening then closing)
bool MyApp::Menu_BinaryMorphology_SmoothOC( Image &image )
{
    int w = 3;
    if ( !getParams( w ) ) return false;
    int *mask = new int [ w * w ];
    for ( int i = 0; i < w * w; i++ ) mask[i] = 1;
    openClose2D( image, mask, w, w );
    delete [] mask;
    return true;
}

// binary neighborhood smoothing (closing then opening)
bool MyApp::Menu_BinaryMorphology_SmoothCO( Image &image )
{
    int w = 3;
    if ( !getParams( w ) ) return false;
    int *mask = new int [ w * w ];
    for ( int i = 0; i < w * w; i++ ) mask[i] = 1;
    closeOpen2D( image, mask, w, w );
    delete [] mask;
    return true;
}

// binary morphological gradient
bool MyApp::Menu_BinaryMorphology_Boundary( Image &image )
{
    // morphBoundary2D( image );	// oops, color shift
    Image eroimage( image );
    erode2D( eroimage );
    eroimage.ToRGB();
    imageSubtract( image, eroimage );
    return true;
}

// hit-miss transform
bool MyApp::Menu_BinaryMorphology_HitMiss( Image &image )
{
    hitMiss( image );
    return true;
}

// -------------------------- grayscale morphological filtering ------------------------------

// grayscale 3x3 diamond-shaped dilation
bool MyApp::Menu_GrayMorphology_Dilate3x3( Image &image )
{
    dilate( image );
    return true;
}

// grayscale 3x3 diamond-shaped erosion
bool MyApp::Menu_GrayMorphology_Erode3x3( Image &image )
{
    erode( image );
    return true;
}

// grayscale neighborhood dilation
bool MyApp::Menu_GrayMorphology_Dilate( Image &image )
{
    int w = 3;
    if ( !getParams( w ) ) return false;
    int *mask = new int [ w * w ];
    for ( int i = 0; i < w * w; i++ ) mask[i] = 1;
    dilate3D( image, mask, w, w );
    delete [] mask;
    return true;
}

// grayscale neighborhood erosion
bool MyApp::Menu_GrayMorphology_Erode( Image &image )
{
    int w = 3;
    if ( !getParams( w ) ) return false;
    int *mask = new int [ w * w ];
    for ( int i = 0; i < w * w; i++ ) mask[i] = 1;
    erode3D( image, mask, w, w );
    delete [] mask;
    return true;
}

// grayscale neighborhood opening
bool MyApp::Menu_GrayMorphology_Open( Image &image )
{
    int w = 3;
    if ( !getParams( w ) ) return false;
    int *mask = new int [ w * w ];
    for ( int i = 0; i < w * w; i++ ) mask[i] = 1;
    open3D( image, mask, w, w );
    delete [] mask;
    return true;
}

// grayscale neighborhood closing
bool MyApp::Menu_GrayMorphology_Close( Image &image )
{
    int w = 3;
    if ( !getParams( w ) ) return false;
    int *mask = new int [ w * w ];
    for ( int i = 0; i < w * w; i++ ) mask[i] = 1;
    close3D( image, mask, w, w );
    delete [] mask;
    return true;
}

// grayscale neighborhood smoothing (opening then closing)
bool MyApp::Menu_GrayMorphology_SmoothOC( Image &image )
{
    int w = 3;
    if ( !getParams( w ) ) return false;
    int *mask = new int [ w * w ];
    for ( int i = 0; i < w * w; i++ ) mask[i] = 1;
    openClose3D( image, mask, w, w );
    delete [] mask;
    return true;
}

// grayscale neighborhood smoothing (closing then opening)
bool MyApp::Menu_GrayMorphology_SmoothCO( Image &image )
{
    int w = 3;
    if ( !getParams( w ) ) return false;
    int *mask = new int [ w * w ];
    for ( int i = 0; i < w * w; i++ ) mask[i] = 1;
    closeOpen3D( image, mask, w, w );
    delete [] mask;
    return true;
}

// grayscale neighborhood sharpening
bool MyApp::Menu_GrayMorphology_Sharpen( Image &image )
{
    morphologicalSharpen( image );
    return true;
}

// grayscale morphological gradient
bool MyApp::Menu_GrayMorphology_Boundary( Image &image )
{
    // morphBoundary3D( image );	// oops, color shift
    Image eroimage( image );
    erode3D( eroimage );
    eroimage.ToRGB();
    imageSubtract( image, eroimage );
    return true;
}

// top hat transform
bool MyApp::Menu_GrayMorphology_TopHat( Image &image )
{
    topHat3D( image );
    return true;
}

// -------------------------- thinning ------------------------------

// binary thinning
bool MyApp::Menu_Thin_BinaryMorph4( Image &image )
{
    thinBinaryMorph4( image );
    return true;
}

// binary thinning
bool MyApp::Menu_Thin_BinaryMorph8( Image &image )
{
    thinBinaryMorph8( image );
    return true;
}

// grayscale thinning
bool MyApp::Menu_Thin_Gray4Min( Image &image )
{
    thinGray4Min( image );
    return true;
}

// grayscale thinning
bool MyApp::Menu_Thin_Gray4Max( Image &image )
{
    thinGray4Max( image );
    return true;
}

// grayscale thinning
bool MyApp::Menu_Thin_Gray8Min( Image &image )
{
    thinGray8Min( image );
    return true;
}

// grayscale thinning
bool MyApp::Menu_Thin_Gray8Max( Image &image )
{
    thinGray8Max( image );
    return true;
}

// binary thinning
bool MyApp::Menu_Thin_BinaryZS( Image &image )
{
    thinBinaryZS( image );
    return true;
}

// grayscale thinning
bool MyApp::Menu_Thin_GrayZSMin( Image &image )
{
    thinGrayZSMin( image );
    return true;
}

// grayscale thinning
bool MyApp::Menu_Thin_GrayZSMax( Image &image )
{
    thinGrayZSMax( image );
    return true;
}

// grayscale thinning
bool MyApp::Menu_Thin_GrayUmbra( Image &image )
{
    thinGrayUmbra( image );
    return true;
}

// grayscale thinning
bool MyApp::Menu_Thin_GrayUmbraIter( Image &image )
{
    int n = 4;
    if ( !getParams( n ) ) return false;
    thinGrayUmbraIter( image, n );
    return true;
}

// ridge detection
bool MyApp::Menu_Thin_Ridge( Image &image )
{
    ridge( image );
    return true;
}

// -------------------------- geometric transforms ------------------------------

// scale image intensities
bool MyApp::Menu_Geometry_ScaleIntensityLevels( Image &image )
{
    int minLevel = 0, maxLevel = 255;
    if ( !Dialog( "endpoints" ).Add( minLevel, "left", 0, 255 ).Add( maxLevel, "right", 0, 255 ).Show() )
        return false;
    scaleIntensityLevels( image, minLevel, maxLevel );
    return true;
}

// requantize image intensities
bool MyApp::Menu_Geometry_ReduceIntensityLevels( Image &image )
{
    int n = 16;
    if ( !getParams( n ) ) return false;
    reduceIntensityLevels( image, n );
    return true;
}

// resample image intensities
bool MyApp::Menu_Geometry_ReduceSpatialResolution( Image &image )
{
    int r = 2, c = 2;
    if ( !Dialog( "sampling factors" ).Add( r, "rows", 0, 256 ).Add( c, "cols", 0, 256 ).Show() )
        return false;
    reduceSpatialResolution( image, r, c );
    return true;
}

// reduce size (zoom out)
bool MyApp::Menu_Geometry_ReduceByAveraging( Image &image )
{
    int r = 2, c = 2;
    if ( !Dialog( "sampling factors" ).Add( r, "rows", 0, 256 ).Add( c, "cols", 0, 256 ).Show() )
        return false;
    reduceByAveraging( image, r, c );
    return true;
}

// reduce size (zoom out)
bool MyApp::Menu_Geometry_ReduceBySubsampling( Image &image )
{
    int r = 2, c = 2;
    if ( !Dialog( "sampling factors" ).Add( r, "rows", 0, 256 ).Add( c, "cols", 0, 256 ).Show() )
        return false;
    reduceBySubsampling( image, r, c );
    return true;
}

// enlarge (zoom in)
bool MyApp::Menu_Geometry_EnlargeByDuplication( Image &image )
{
    int r = 2, c = 2;
    if ( !Dialog( "sampling factors" ).Add( r, "rows", 0, 256 ).Add( c, "cols", 0, 256 ).Show() )
        return false;
    enlargeByDuplication( image, r, c );
    return true;
}

// resize image
bool MyApp::Menu_Geometry_Rescale( Image &image )
{
    int r = image.Height(), c = image.Width();
    if ( !Dialog( "image dimensions" ).Add( r, "rows" ).Add( c, "cols" ).Show() )
        return false;
    if ( r < 1 || r > 16384 || c < 1 || c > 16384 ) return false;
    rescale( image, r, c );
    return true;
}

// resize image
bool MyApp::Menu_Geometry_RescaleNN( Image &image )
{
    int r = image.Height(), c = image.Width();
    if ( !Dialog( "image dimensions" ).Add( r, "rows" ).Add( c, "cols" ).Show() )
        return false;
    if ( r < 1 || r > 16384 || c < 1 || c > 16384 ) return false;
    rescale( image, r, c, NEAREST_NEIGHBOR );
    return true;
}

// rotate image
bool MyApp::Menu_Geometry_Rotate( Image &image )
{
    double theta = 90;
    if ( !getParams( theta ) ) return false;
    rotate( image, theta );
    return true;
}

// rotate image
bool MyApp::Menu_Geometry_RotateNN( Image &image )
{
    double theta = 90;
    if ( !getParams( theta ) ) return false;
    rotate( image, theta, NEAREST_NEIGHBOR );
    return true;
}

// fisheye mapping to sphere (?)
bool MyApp::Menu_Geometry_Fisheye1( Image &image )
{
    fisheye1( image );
    return true;
}

// fisheye mapping to circle
bool MyApp::Menu_Geometry_Fisheye2( Image &image )
{
    fisheye2( image );
    return true;
}

// -------------------------- image arithmetic ------------------------------

// add constant
bool MyApp::Menu_Arithmetic_AddConstant( Image &image )
{
    int c = 0;
    if ( !getParams( c ) ) return false;
    imageAddConstant( image, c );
    return true;
}

// subtract constant
bool MyApp::Menu_Arithmetic_SubtractConstant( Image &image )
{
    int c = 0;
    if ( !getParams( c ) ) return false;
    imageSubtractConstant( image, c );
    return true;
}

// multiply by constant
bool MyApp::Menu_Arithmetic_MultiplyConstant( Image &image )
{
    double c = 1.0;
    if ( !getParams( c ) ) return false;
    imageMultiplyConstant( image, c );
    return true;
}

// divide by constant
bool MyApp::Menu_Arithmetic_DivideConstant( Image &image )
{
    double c = 1.0;
    if ( !getParams( c ) ) return false;
    imageDivideConstant( image, c );
    return true;
}

// image addition
bool MyApp::Menu_Arithmetic_AddImage( Image &image1 )
{
    Image image2;
    if ( !getParams( image2 ) ) return false;
    imageAdd( image1, image2 );
    return true;
}

// image subtraction
bool MyApp::Menu_Arithmetic_SubtractImage( Image &image1 )
{
    Image image2;
    if ( !getParams( image2 ) ) return false;
    imageSubtract( image1, image2 );
    return true;
}

// image multiplication
bool MyApp::Menu_Arithmetic_MultiplyImage( Image &image1 )
{
    Image image2;
    if ( !getParams( image2 ) ) return false;
    imageMultiply( image1, image2 );
    return true;
}

// image division
bool MyApp::Menu_Arithmetic_DivideImage( Image &image1 )
{
    Image image2;
    if ( !getParams( image2 ) ) return false;
    imageDivide( image1, image2 );
    return true;
}

// image AND
bool MyApp::Menu_Arithmetic_ImageAND( Image &image1 )
{
    Image image2;
    if ( !getParams( image2 ) ) return false;
    imageAND( image1, image2 );
    return true;
}

// image OR
bool MyApp::Menu_Arithmetic_ImageOR( Image &image1 )
{
    Image image2;
    if ( !getParams( image2 ) ) return false;
    imageOR( image1, image2 );
    return true;
}

// image XOR
bool MyApp::Menu_Arithmetic_ImageXOR( Image &image1 )
{
    Image image2;
    if ( !getParams( image2 ) ) return false;
    imageXOR( image1, image2 );
    return true;
}

// image NOT
bool MyApp::Menu_Arithmetic_ImageNOT( Image &image )
{
    imageNOT( image );
    return true;
}

// -------------------------- color models ------------------------------

// display color histogram
bool MyApp::Menu_Color_Histogram( Image &image )
{
    displayHistogram( image.ColorHistogram() );
    return true;
}

// display red bank
bool MyApp::Menu_Color_Red( Image &image )
{
    colorBankRed( image );
    return true;
}

// display green bank
bool MyApp::Menu_Color_Green( Image &image )
{
    colorBankGreen( image );
    return true;
}

// display blue bank
bool MyApp::Menu_Color_Blue( Image &image )
{
    colorBankBlue( image );
    return true;
}

// display luminance bank (Y of YIQ)
bool MyApp::Menu_Color_Luminance( Image &image )
{
    colorBankLuminance( image );
    return true;
}

// display inphase bank (I of YIQ)
bool MyApp::Menu_Color_Inphase( Image &image )
{
    colorBankInphase( image );
    return true;
}

// display quadrature bank (Q of YIQ)
bool MyApp::Menu_Color_Quadrature( Image &image )
{
    colorBankQuadrature( image );
    return true;
}

// display hue bank (H of HSI)
bool MyApp::Menu_Color_Hue( Image &image )
{
    colorBankHue( image );
    return true;
}

// display saturation bank (S of HSI)
bool MyApp::Menu_Color_Saturation( Image &image )
{
    colorBankSaturation( image );
    return true;
}

// display intensity bank (I of HSI)
bool MyApp::Menu_Color_Intensity( Image &image )
{
    colorBankIntensity( image );
    return true;
}

// display false color image
bool MyApp::Menu_Color_FalseColor( Image &image )
{
    Image image1, image2, image3, image4;
    if ( !getParams( image1, image2, image3 ) ) return false;
    createImageFromRGB( image1, image2, image3, image4 );
    image = image4;
    return true;
}

// -------------------------- segmentation ------------------------------

// binary thresholding
bool MyApp::Menu_Segment_BinaryThreshold( Image &image )
{
    int thresh = 128;
    if ( !Dialog( "grayscale threshold" ).Add( thresh, "Threshold", 1, 255 ).Show() )
        return false;
    binaryThreshold( image, thresh );
    return true;
}

// iterative binary thresholding
bool MyApp::Menu_Segment_IterativeBinaryThreshold( Image &image )
{
    int thresh = iterativeBinaryThreshold( image );
    std::cerr << "iterative grayscale threshold = " << thresh << "\n";
    return true;
}

// Laplacian-based thresholding
bool MyApp::Menu_Segment_LaplacianThreshold( Image &image )
{
    int thresh = laplacianThreshold( image );
    std::cerr << "iterative grayscale threshold = " << thresh << "\n";
    return true;
}

// iterative Laplacian-based thresholding
bool MyApp::Menu_Segment_IterativeLaplacianThreshold( Image &image )
{
    int thresh = iterativeLaplacianThreshold( image );
    std::cerr << "iterative grayscale threshold = " << thresh << "\n";
    return true;
}

// adaptive iterative binary thresholding
bool MyApp::Menu_Segment_AdaptiveBinaryThreshold( Image &image )
{
    int w = 15;
    if ( !getParams( w ) ) return false;
    // Image &thresholds = adaptiveBinaryThreshold( image, w );
    Image thresholds;
    adaptiveBinaryThreshold( image, w, thresholds );
    displayImage( thresholds, "Thresholds" );
    return true;
}

// contour plot
bool MyApp::Menu_Segment_Contours( Image &image )
{
    int w = 32;
    if ( !getParams( w ) )
        return false;
    contours( image, w );
    return true;
}

// contour plot
bool MyApp::Menu_Segment_AddContours( Image &image )
{
    int w = 32;
    if ( !getParams( w ) ) return false;
    addContours( image, w );
    return true;
}

// seed fill
bool MyApp::Menu_Segment_SeedFill( Image &image )
{
    int w = 32;
    if ( !getParams( w ) ) return false;
    seedFill( image, w );
    return true;
}

// seed fill using max intensities as seeds
bool MyApp::Menu_Segment_MaximumIntensitySeedFill( Image &image )
{
    int w = 32;
    if ( !getParams( w ) ) return false;
    maximumIntensitySeedFill( image, w );
    return true;
}

// seed fill using max intensities as seeds
bool MyApp::Menu_Segment_MaxIntHistogramSeedFill( Image &image )
{
    int w = 32;
    if ( !getParams( w ) ) return false;
    maximumIntensityHistogramSeedFill( image, w );
    return true;
}

// scan fill
bool MyApp::Menu_Segment_ScanFill( Image &image )
{
    int w = 32;
    if ( !getParams( w ) ) return false;
    scanFill( image, w );
    return true;
}

// Wilton scan fill
bool MyApp::Menu_Segment_ScanFillWilton( Image &image )
{
    int w = 32;
    if ( !getParams( w ) ) return false;
    scanFillWilton( image, w );
    return true;
}

// scan fill using max intensities as seeds
bool MyApp::Menu_Segment_MaximumIntensityScanFill( Image &image )
{
    int w = 32;
    if ( !getParams( w ) ) return false;
    maximumIntensityScanFill( image, w );
    return true;
}

// chamfer 3-4 distance transform
bool MyApp::Menu_Segment_Chamfer34( Image &image )
{
    chamfer34( image );
    return true;
}

// -------------------------- noise ------------------------------

// impulse noise
bool MyApp::Menu_Noise_AddImpulseNoise( Image &image )
{
    int p = 64;
    if ( !Dialog( "Impulse Noise" ).Add( p, "probability", 0, 1000 ).Show() )
        return false;
    impulseNoise( image, p );
    return true;
}

// impulse noise
bool MyApp::Menu_Noise_AddWhiteNoise( Image &image )
{
    int p = 64;
    if ( !Dialog( "White Noise" ).Add( p, "probability", 0, 1000 ).Show() )
        return false;
    whiteNoise( image, p );
    return true;
}

// impulse noise
bool MyApp::Menu_Noise_AddBlackNoise( Image &image )
{
    int p = 64;
    if ( !Dialog( "Black Noise" ).Add( p, "probability", 0, 1000 ).Show() )
        return false;
    blackNoise( image, p );
    return true;
}

// Gaussian noise
bool MyApp::Menu_Noise_AddGaussianNoise( Image &image )
{
    double sd = 8.0;
    if ( !Dialog( "Gaussian Noise" ).Add( sd, "sigma", 0.0, 100.0 ).Show() )
        return false;
    gaussianNoise( image, sd );
    return true;
}

// noise cleaning
bool MyApp::Menu_Noise_NoiseClean( Image &image )
{
    int thresh = 32;
    if ( !Dialog( "Noise Threshold" ).Add( thresh, "threshold", 0, 255 ).Show() )
        return false;
    noiseClean( image, thresh );
    return true;
}

