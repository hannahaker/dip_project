/*
               ***** qtip.h *****

Demo program to illustrate most of QtImageLib functionality.

Author: John M. Weiss, Ph.D.
Written Spring 2015 for SDSM&T CSC442/542 DIP course.

Modifications:
*/

// next two lines are typical
#pragma once
#include <qtimagelib.h>

// class declaration
class MyApp: public QObject
{
    Q_OBJECT;	// QOBJECT macro must appear at start of class

    // all menu items must be prototyped in the "public slots" section
  public slots:

    bool Menu_Palette_Grayscale( Image & );
    bool Menu_Palette_Negate( Image & );
    bool Menu_Palette_Brightness( Image & );
    bool Menu_Palette_Contrast( Image & );
    bool Menu_Palette_Gamma( Image & );
    bool Menu_Palette_Log( Image & );
    bool Menu_Palette_Posterize( Image & );
    bool Menu_Palette_PseudoColor8( Image & );
    bool Menu_Palette_PseudoColorCube( Image & );
    bool Menu_Palette_PseudoColorGW( Image & );
    bool Menu_Palette_Solarize( Image & );
    bool Menu_Palette_Sawtooth( Image & );
    bool Menu_Palette_SawtoothRGB( Image & );
    bool Menu_Palette_SawtoothBGR( Image & );
    bool Menu_Palette_Triangle( Image & );
    bool Menu_Palette_InverseTriangle( Image & );
    bool Menu_Palette_Zebra( Image & );
    bool Menu_Palette_Random( Image & );
    bool Menu_Palette_TotallyRandom( Image & );
    bool Menu_Palette_AlmostRandom( Image & );

    bool Menu_Run_Hausdorff( Image & );

    bool Menu_Histogram_Display( Image & );
    bool Menu_Histogram_Stretch( Image & );
    bool Menu_Histogram_StretchSpecify( Image & );
    bool Menu_Histogram_Equalize( Image & );
    bool Menu_Histogram_EqualizeClip( Image & );
    bool Menu_Histogram_AdaptiveEqualize( Image & );
    bool Menu_Histogram_AdaptiveContrastStretch( Image & );
    bool Menu_Histogram_Match( Image & );

    bool Menu_Filter_Smooth( Image & );
    bool Menu_Filter_Sharpen( Image & );
    bool Menu_Filter_Average( Image & );
    bool Menu_Filter_WeightedAverage( Image & );
    bool Menu_Filter_GaussianSmoothing( Image & );
    bool Menu_Filter_Minimum( Image & );
    bool Menu_Filter_Maximum( Image & );
    bool Menu_Filter_Range( Image & );
    bool Menu_Filter_MedianBox( Image & );
    bool Menu_Filter_MedianPlus( Image & );
    bool Menu_Filter_Median( Image & );
    bool Menu_Filter_SeparableMedian( Image & );
    bool Menu_Filter_UnsharpMasking( Image & );
    bool Menu_Filter_UnsharpParametric( Image & );
    bool Menu_Filter_Emboss( Image & );
    bool Menu_Filter_Variance( Image & );
    bool Menu_Filter_StandardDeviation( Image & );
    bool Menu_Filter_StatisticalDifference( Image & );

    bool Menu_Frequency_Magnitude( Image & );
    bool Menu_Frequency_Phase( Image & );
    bool Menu_Frequency_IdealLowPassFilter( Image & );
    bool Menu_Frequency_IdealHighPassFilter( Image & );
    bool Menu_Frequency_IdealEmphasisFilter( Image & );
    bool Menu_Frequency_GaussianLowPassFilter( Image & );
    bool Menu_Frequency_GaussianHighPassFilter( Image & );
    bool Menu_Frequency_GaussianEmphasisFilter( Image & );
    bool Menu_Frequency_ButterworthLowPassFilter( Image & );
    bool Menu_Frequency_ButterworthHighPassFilter( Image & );
    bool Menu_Frequency_ButterworthEmphasisFilter( Image & );
    bool Menu_Frequency_HomomorphicFilter( Image & );
    bool Menu_Frequency_IdealBandPassFilter( Image & );
    bool Menu_Frequency_NotchFilter( Image & );

    bool Menu_Edge_Sobel( Image & );
    bool Menu_Edge_SobelMagnitude( Image & );
    bool Menu_Edge_SobelDirection( Image & );
    bool Menu_Edge_Prewitt( Image & );
    bool Menu_Edge_PrewittMagnitude( Image & );
    bool Menu_Edge_PrewittDirection( Image & );
    bool Menu_Edge_FreiChen( Image & );
    bool Menu_Edge_FreiChenMagnitude( Image & );
    bool Menu_Edge_FreiChenDirection( Image & );
    bool Menu_Edge_Roberts( Image & );
    bool Menu_Edge_Horizontal( Image & );
    bool Menu_Edge_Vertical( Image & );
    bool Menu_Edge_Canny( Image & );
    bool Menu_Edge_Laplacian( Image & );
    bool Menu_Edge_LaplacianOfGaussian( Image & );
    bool Menu_Edge_ZeroCrossings( Image & );
    bool Menu_Edge_KirschMagnitude( Image & );
    bool Menu_Edge_KirschDirection( Image & );
    bool Menu_Edge_NevatiaBabu( Image & );
    bool Menu_Edge_RobinsonMagnitude( Image & );
    bool Menu_Edge_RobinsonDirection( Image & );

    bool Menu_BinaryMorphology_Dilate3x3( Image & );
    bool Menu_BinaryMorphology_Erode3x3( Image & );
    bool Menu_BinaryMorphology_Dilate( Image & );
    bool Menu_BinaryMorphology_Erode( Image & );
    bool Menu_BinaryMorphology_Open( Image & );
    bool Menu_BinaryMorphology_Close( Image & );
    bool Menu_BinaryMorphology_SmoothOC( Image & );
    bool Menu_BinaryMorphology_SmoothCO( Image & );
    bool Menu_BinaryMorphology_Boundary( Image & );
    bool Menu_BinaryMorphology_HitMiss( Image & );

    bool Menu_GrayMorphology_Dilate3x3( Image & );
    bool Menu_GrayMorphology_Erode3x3( Image & );
    bool Menu_GrayMorphology_Dilate( Image & );
    bool Menu_GrayMorphology_Erode( Image & );
    bool Menu_GrayMorphology_Open( Image & );
    bool Menu_GrayMorphology_Close( Image & );
    bool Menu_GrayMorphology_SmoothOC( Image & );
    bool Menu_GrayMorphology_SmoothCO( Image & );
    bool Menu_GrayMorphology_Sharpen( Image & );
    bool Menu_GrayMorphology_Boundary( Image & );
    bool Menu_GrayMorphology_TopHat( Image & );

    bool Menu_Thin_BinaryMorph4( Image & );
    bool Menu_Thin_BinaryMorph8( Image & );
    bool Menu_Thin_Gray4Min( Image & );
    bool Menu_Thin_Gray4Max( Image & );
    bool Menu_Thin_Gray8Min( Image & );
    bool Menu_Thin_Gray8Max( Image & );
    bool Menu_Thin_BinaryZS( Image & );
    bool Menu_Thin_GrayZSMin( Image & );
    bool Menu_Thin_GrayZSMax( Image & );
    bool Menu_Thin_GrayUmbra( Image & );
    bool Menu_Thin_GrayUmbraIter( Image & );
    bool Menu_Thin_Ridge( Image & );

    bool Menu_Segment_BinaryThreshold( Image & );
    bool Menu_Segment_IterativeBinaryThreshold( Image & );
    bool Menu_Segment_LaplacianThreshold( Image & );
    bool Menu_Segment_IterativeLaplacianThreshold( Image & );
    bool Menu_Segment_AdaptiveBinaryThreshold( Image & );
    bool Menu_Segment_Contours( Image & );
    bool Menu_Segment_AddContours( Image & );
    bool Menu_Segment_SeedFill( Image & );
    bool Menu_Segment_MaximumIntensitySeedFill( Image & );
    bool Menu_Segment_MaxIntHistogramSeedFill( Image & );
    bool Menu_Segment_ScanFill( Image & );
    bool Menu_Segment_ScanFillWilton( Image & );
    bool Menu_Segment_MaximumIntensityScanFill( Image & );
    bool Menu_Segment_Chamfer34( Image &image );

    bool Menu_Geometry_ScaleIntensityLevels( Image & );
    bool Menu_Geometry_ReduceIntensityLevels( Image & );
    bool Menu_Geometry_ReduceSpatialResolution( Image & );
    bool Menu_Geometry_ReduceByAveraging( Image & );
    bool Menu_Geometry_ReduceBySubsampling( Image & );
    bool Menu_Geometry_EnlargeByDuplication( Image & );
    bool Menu_Geometry_Rescale( Image & );
    bool Menu_Geometry_RescaleNN( Image & );
    bool Menu_Geometry_Rotate( Image & );
    bool Menu_Geometry_RotateNN( Image & );
    bool Menu_Geometry_Fisheye1( Image & );
    bool Menu_Geometry_Fisheye2( Image & );

    bool Menu_Arithmetic_AddConstant( Image & );
    bool Menu_Arithmetic_SubtractConstant( Image & );
    bool Menu_Arithmetic_MultiplyConstant( Image & );
    bool Menu_Arithmetic_DivideConstant( Image & );
    bool Menu_Arithmetic_AddImage( Image & );
    bool Menu_Arithmetic_SubtractImage( Image & );
    bool Menu_Arithmetic_MultiplyImage( Image & );
    bool Menu_Arithmetic_DivideImage( Image & );
    bool Menu_Arithmetic_ImageAND( Image & );
    bool Menu_Arithmetic_ImageOR( Image & );
    bool Menu_Arithmetic_ImageXOR( Image & );
    bool Menu_Arithmetic_ImageNOT( Image & );

    bool Menu_Color_Histogram( Image & );
    bool Menu_Color_Red( Image & );
    bool Menu_Color_Green( Image & );
    bool Menu_Color_Blue( Image & );
    bool Menu_Color_Luminance( Image & );
    bool Menu_Color_Inphase( Image & );
    bool Menu_Color_Quadrature( Image & );
    bool Menu_Color_Hue( Image & );
    bool Menu_Color_Saturation( Image & );
    bool Menu_Color_Intensity( Image & );
    bool Menu_Color_FalseColor( Image & );

    bool Menu_Noise_AddImpulseNoise( Image & );
    bool Menu_Noise_AddWhiteNoise( Image & );
    bool Menu_Noise_AddBlackNoise( Image & );
    bool Menu_Noise_AddGaussianNoise( Image & );
    bool Menu_Noise_NoiseClean( Image & );

};

