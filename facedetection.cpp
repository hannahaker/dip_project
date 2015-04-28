
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
    faceDetection obj;
    ImageApp app( argc, argv );
    app.AddActions( &obj );
    return app.Start();
}


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
bool faceDetection::Menu_Hausdorff_RunHausdorff( Image& image1 )
{
    Image image2;
    int thresh = 10;
    double forwardP = 1.0;
    double reverseP = 0.95;


    //int thresh = 10;
    //    if ( !Dialog( "pixel error threshold" ).Add( thresh, "Threshold", 1, 30 ).Show() )
    //        return false;
    //if ( !getParams( image2 ) ) return false;
    if ( !Dialog( "Hausdorff Parameters" )
            .Add( image2, "Model Image")
            .Add( thresh, "Pixel Error Threshold", 1, 30 )
            .Add( forwardP, "Percent of List Used in Forward Hausdorff", 0, 1.0 )
            .Add( reverseP, "Percent of List Used in Reverse Hausdorff", 0, 1.0 )
            .Show() )
            return false;



    // setup image_model objects to store image, voronoi surface of image,
    // and other properties of the image
    Image_Model target(image1);
    Image_Model model(image2);

    // intantiate list containers to hold objects containing matching
    // transformations
    vector<tsObject> matches;

    // intantiate list containers to hold validated transformations
    // validation occurs through the partial box-reverse HD
    vector<tsObject> goodMatches;

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
    // This allows a smaller set of points to work with for the Hausdorff
    target.thin();
        //displayImage(target.image, "Thinnned image");

    // get the target points from the image
    if( target.init_points() == 0 )
        return false;

    // calculate the voronoi surface of both target and model
    target.init_voronoi_mask();
    model.init_voronoi_mask();
    //target.init_voronoi();
    //target.display_voronoi();

    // store matches found using partial forward HD
    matches = decomp(target, model, thresh, 1.0, 1);

    // store matches validated through the partial box-reverse HD
    goodMatches = validMatches(matches, target, model, thresh, 0.95);


    // Draw boxes around all the matches
    draw_box(image1, goodMatches, model.rows, model.cols);

    return true;
}

/************************************************************************
   Function: Image_Model thin
   Author: Zachary Pierson
   Description: Successive thinnings until there is no more change. This
        allows a smaller set of points to work with for the Hausdorff
 ************************************************************************/
bool thin(Image &image)
{
    Image prev(image);

    thinBinaryZS( image );

    while( !equal(image, prev) )
    {
        prev = image;
        thinBinaryZS( image );
    }

    return true;
}




bool faceDetection::Menu_Preproccessing_GaussianSmoothing( Image &image )
{
    double sigma = 2.0;
    if ( !getParams( sigma ) ) return false;
    smoothGaussian( image, 2.0 );
    return true;
}

bool faceDetection::Menu_Preprocessing_Sobel( Image &image )
{
    sobel( image );
    return true;
}

bool faceDetection::Menu_Preproccessing_BinaryThreshold( Image &image )
{
    int thresh = 128;
    if ( !Dialog( "grayscale threshold" ).Add( thresh, "Threshold", 1, 255 ).Show() )
        return false;
    binaryThreshold( image, thresh );
    return true;
}

bool faceDetection::Menu_Preproccessing_SuccessiveThinning( Image &image )
{
    //thin( image );
    return true;
}
