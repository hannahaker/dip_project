#include "header.h"


/************************************************************************
   Function: Image_Model thin
   Author: Zachary Pierson
   Description: Successive thinnings until there is no more change. This
        allows a smaller set of points to work with for the Hausdorff
 ************************************************************************/
bool thin(Image& image)
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

/************************************************************************
   Function: Equal
   Author: Zachary Pierson
   Description: Checks to see if two images are the same
 ************************************************************************/
bool equal(Image& image1, Image& image2)
{
    if(image1.Width() != image2.Width() || image1.Height() != image2.Height())
        return false;

    for(unsigned int r = 0; r < image1.Height(); r++)
    {
        for(unsigned int c = 0; c < image1.Width(); c++)
        {
            if(image1[r][c] != image2[r][c])
                return false;
        }
    }

    return true;
}

/**************************************************************************//**
 * @author Zachary Pierson
 *
 * @par Description:
 * euclidean_dist - calculates the Euclidean Distance between two points
 *
 * @param[in] point A - x,y coordinate of a point in the image
 * @param[in] point B - x,y coordinate of a point in the image
 *
 *****************************************************************************/
double euclidean_dist(point A, point B)
{
    return sqrt( pow(A.x - B.x, 2) + pow(A.y - B.y, 2));
}

/**************************************************************************//**
 * @author Zachary Pierson
 *
 * @par Description:
 * Forward Hausdorff - Returns a list of the directed hausorff distances.
 * An ordered list with best matching distances first. This is ORDERS of
 * magnitueds FASTER than the original hausdorff distance calculation.
 * Uses voronoi surface to faster calculate Hausdorff values.
 *
 * @param[in] modelPoints - the list of points in the model
 * @param[in,out] target - the Image_model object of the target image
 *
 * @returns vector<double> list of sorted distances
 *
 *****************************************************************************/
vector<double> forward_hausdorff(vector<point> modelPoints, Image_Model& target)
{
    /*
    //clock_t t = clock();
    vector<double> distance;

    //make sure there are points to process
    if(pts.size() == 0)
        return distance;

    for(unsigned int i = 0; i < pts.size(); i++)
        distance.push_back(surface[pts[i].x][pts[i].y]);

    sort(distance.begin(), distance.end());
    cout << "Hausdorff Distance (Voronoi): " << distance.back() << endl;

   //printf("time to calculate hausdorff (voronoi): %f\n", (double)(clock() - t)/CLOCKS_PER_SEC );

    return distance;
    */

    //clock_t t = clock();
    vector<double> distance;

    //make sure there are points to process
    if(modelPoints.size() == 0)
        return distance;

    for(unsigned int i = 0; i < modelPoints.size(); i++)
    {
        distance.push_back(target.voronoi[modelPoints[i].x][modelPoints[i].y]);
    }

    sort(distance.begin(), distance.end());
    //cout << "Hausdorff Distance (Voronoi): " << distance.back() << endl;

   //printf("time to calculate hausdorff (voronoi): %f\n", (double)(clock() - t)/CLOCKS_PER_SEC );

    return distance;
}

/**************************************************************************//**
 * @author Zachary Pierson, Hannah Aker, Steven Huerta
 *
 * @par Description:
 * reverse_hausdorff - Box Reverse Hausdorff. Returns a list of the
 * directed hausorff distances.  An ordered list with best matching distances
 * first. This is ORDERS of magnitueds FASTER than the original hausdorff
 * distance calculation.
 *
 * @param[in] ts - tsObject containing transformation
 * @param[in] target - the Image_model object of the target image
 * @param[in,out] model - the Image_model object of the model image
 *
 * @returns vector<double> list of sorted distances
 *
 *****************************************************************************/
vector<double> reverse_hausdorff(tsObject ts, Image_Model& target,
                                 Image_Model& model)
{
    //clock_t t = clock();
    vector<double> distance;

    //make sure there are points to process
    if(target.points.size() == 0)
        return distance;

    // create a list of points describing the shape
    // of the transformed model, which will form the "box" from which
    // we will pull our points
    int x1 = ts.transXCenter ;
    int x2 = x1 + model.cols ;
    int y1 = ts.transYCenter ;
    int y2 = y1 + model.rows ;

    // for all the points in the target image
    for(  unsigned int i = 0; i < target.points.size(); i++)
    {        
        // for the ones that are inside our transformed "box"
        if( (target.points[i].x >= x1)&&(target.points[i].x<x2)&&
            (target.points[i].y>=y1)&&(target.points[i].y<y2) )
        {
            // Let's find the points distance from the points on the image
            // to the model
            int xVor = target.points[i].x - ts.transXCenter ;
            int yVor = target.points[i].y - ts.transYCenter ;
            distance.push_back( model.voronoi[xVor][yVor] );
        }
    }

    // sort the list
    sort(distance.begin(), distance.end());
    //cout << "Reverse Hausdorff Distance (Voronoi): " << distance.back() << endl;

   //printf("time to calculate hausdorff (voronoi): %f\n", (double)(clock() - t)/CLOCKS_PER_SEC );

    return distance;
}

/**************************************************************************//**
 * @author Zachary Pierson, Hannah Aker, Steven Huerta
 *
 * @par Description:
 * directed_hausdorff - Box Reverse Hausdorff. Returns a list of the
 * directed hausorff distances.  An ordered list with best matching distances
 * first. This is ORDERS of magnitueds FASTER than the original hausdorff
 * distance calculation.
 *
 * @param[in,out] B - list of points in the model image
 * @param[in,out] A - list of points in the target image
 *
 * @returns vector<double> list of sorted distances
 *
 *****************************************************************************/
vector<double> directed_hausdorff(vector<point> &B, vector<point>& A)
{
    //clock_t t = clock();
    double max = -1;
    double min = 9999;
    double dist;
    vector<double> distance;

    if(A.size() == 0)
        return distance;

    //find maximum of the smallest distances from B to A
    for(unsigned int i = 0; i < B.size(); i++)
    {
        for(unsigned int j = 0; j < A.size(); j++)
        {
            dist = euclidean_dist(B[i], A[j]);
            if(dist < min)
            {
                min = dist;
            }
        }

        //keep track of maximum
        if(min > max)
        {
            max = min;
        }

        //save the the smallest distance from B[i] to A
        distance.push_back(min);
        min = 9999;
    }

    // sort the distances
    sort(distance.begin(), distance.end());
    cout << "Hausdorff Distance: " << distance.back() << endl;

   //printf("time to calculate hausdorff: %f\n", (double)(clock() - t)/CLOCKS_PER_SEC );

    return distance;
}

/**************************************************************************//**
 * @author Hannah Aker, Zachary Pierson, Steven Huerta
 *
 * @par Description:
 * decomp - Finds interesting transformation spaces by continually dividing
 * up the transformation space, evaluating for possible matches, and
 * dividing again until no further division is possible
 *
 * @param[in,out] target - Image_Model of the target
 * @param[in,out] model - Image_Model of the model
 * @param[in,out] pixelErrorThresh - pixel threshold for maximum distance
 * @param[in,out] percentList - percent of list of distances to evaluate
 * @param[in,out] alpha - contols skew factor - NOT IMPLEMENTED
 *
 * @returns vector<double> list of transformations as tsObjects
 *
 *****************************************************************************/
vector<tsObject> decomp(Image_Model& target, Image_Model& model,
                        float pixelErrorThresh = 1, float percentList = 1.0,
                        int alpha = 1)
{
    cout << "Decomp!" << endl;
    // initialize a list of container for tsObjects
    vector<tsObject> realMatches;

    // initialize our first transformation space paramaters
    tsObject tsObj(0, target.cols - model.cols, 0, target.rows - model.rows,1,1,1,1);

    // get our distance of possible transforms in the transform space
    double gamma = calcGamma(tsObj, model.cols-1, model.rows-1);

    queue<tsObject> matches;
    matches.push(tsObj);

    //while (!matches.size()!=0)
    while (!matches.empty())
    {
        gamma = calcGamma(matches.front(), model.cols-1, model.rows-1);
        //cout << "Gamma is " << gamma << endl;
        int thresh = pixelErrorThresh + gamma;
        if ( isInteresting( matches.front(), target, model, percentList, thresh ) )
        {
            if(gamma < 1.5)
            {
                realMatches.push_back(matches.front());
                //printf("realMatches\n");
            }
            else
            {

                vector<tsObject> addMatches = divide(matches.front());
                while( addMatches.size()!= 0)
                {
                 matches.push(addMatches.back());
                 addMatches.pop_back();
                }
            }

            //vector<tsObject> divided = divide(matches.front());
            //matches.insert( matches.end(), divided.begin(), divided.end() ) ;

        }
        //matches.erase( matches.begin() ) ;
        matches.pop();
    }
    cout << "Finished Decomp!!" << endl;
    return realMatches;
}


/**************************************************************************//**
 * @author Zachary Pierson, Hannah Aker, Steven Huerta
 *
 * @par Description:
 * calcGamma - Calculate Gamma for Decomp function. Gamma is the additional
 * length to be added to the threshold so that we can evaluate for possible
 * matches of the model.
 *
 * @param[in] ts - tsObject holding transform
 * @param[in] xMax - holds width of model
 * @param[in] yMax - holds height of model
 *
 * @returns double - gamma
 *
 *****************************************************************************/
double calcGamma( tsObject ts, int xMax, int yMax )
{

    //if(ts.transXMax == ts.transXMin)
      //  printf("hahaha!\n");

    // calculate the x and y to the furthest point in the
    // transformation space
    double x = ts.transXMax - ts.transXMin + (
             ( ts.scaleXMax - ts.scaleXMin )*xMax/2.0);

    double y = ts.transYMax - ts.transYMin + (
             ( ts.scaleYMax - ts.scaleYMin )*yMax/2.0);

    double gamma = (x*x) + (y*y) ;

    // finish calculating the euclidean distance of that distance from the
    // origin
    gamma = sqrt( gamma ) ;

    return gamma;
}

/**************************************************************************//**
 * @author Hannah Aker, Zachary Pierson, Steven Huerta
 *
 * @par Description:
 * isInteresting - determines if a transformation space is intersting, runs
 * hausdorff and examines values returned.
 *
 * @param[in,out] ts - object containing the transformation space
 * @param[in,out] target - Image_Model of the target
 * @param[in,out] model - Image_Model of the model
 * @param[in] percentList - percent of list of distances to evaluate
 * @param[in] threshold - distance threshold for foward partial HD
 *
 * @returns true    - space contains possible matches
 * @returns false   - no possible matches in space
 *
 *****************************************************************************/
bool isInteresting( tsObject & ts, Image_Model & target, Image_Model & model,
                    float percentList, int threshold )
{

    // transform model with quadruple
    vector<point> modelPointsTrans = transform(ts, model);
    vector<double> haus = forward_hausdorff(modelPointsTrans, target);

    // compare partial forward HD ( t(M), I ) at k * #points in sorted
    int hausIndex = percentList * (haus.size()-1);
    double hausDist = haus.at(hausIndex);

    // if the distance is more than the treshold, there is no possible
    // match in this space
    if( hausDist < threshold )
        return true ;
    else
        return false ;
}

/**************************************************************************//**
 * @author Hannah Aker, Zachary Pierson, Steven Huerta
 *
 * @par Description:
 * divide - divides transformation space into subspaces, and pushes them
 * to a list to be passed back.
 *
 * @param[in] ts - object containing the transformation space
 *
 * @returns vector<tsObject>    - list containing transformation spaces
 *
 *****************************************************************************/
vector<tsObject> divide( tsObject ts )
{
    // instantiate list for storing subspaces
    vector<tsObject> subTransformSpace ;

    // create subspaces from transformation space
    // SCALING NOT IMPLEMENTED, SO ONLY 4 SUBSPACES NEEDED & NOT 16
    tsObject cell_1_1( ts.transXMin , ts.transXCenter,
                     ts.transYMin , ts.transYCenter,
                     ts.scaleXMin , ts.scaleXMax,
                     ts.scaleYMin , ts.scaleYMax) ;
//    tsObject cell_1_2( ts.transXMin , ts.transXCenter,
//                     ts.transYMin , ts.transYCenter,
//                     ts.scaleXCenter , ts.scaleXMax,
//                     ts.scaleYMin , ts.scaleYCenter) ;
//    tsObject cell_1_3( ts.transXMin , ts.transXCenter,
//                     ts.transYMin , ts.transYCenter,
//                     ts.scaleXMin , ts.scaleXCenter,
//                     ts.scaleYCenter , ts.scaleYMax) ;
//    tsObject cell_1_4( ts.transXMin , ts.transXCenter,
//                     ts.transYMin , ts.transYCenter,
//                     ts.scaleXCenter , ts.scaleXMax,
//                     ts.scaleYCenter , ts.scaleYMax) ;

    tsObject cell_2_1( ts.transXCenter , ts.transXMax,
                       ts.transYMin , ts.transYCenter,
                     ts.scaleXMin , ts.scaleXMax,
                     ts.scaleYMin , ts.scaleYMax) ;
//    tsObject cell_2_2( ts.transXCenter , ts.transXMax,
//                       ts.transYMin , ts.transYCenter,
//                     ts.scaleXCenter , ts.scaleXMax,
//                     ts.scaleYMin , ts.scaleYCenter) ;
//    tsObject cell_2_3( ts.transXCenter , ts.transXMax,
//                       ts.transYMin , ts.transYCenter,
//                     ts.scaleXMin , ts.scaleXCenter,
//                     ts.scaleYCenter , ts.scaleYMax) ;
//    tsObject cell_2_4( ts.transXCenter , ts.transXMax,
//                       ts.transYMin , ts.transYCenter,
//                     ts.scaleXCenter , ts.scaleXMax,
//                     ts.scaleYCenter , ts.scaleYMax) ;

    tsObject cell_3_1( ts.transXMin , ts.transXCenter,
                       ts.transYCenter , ts.transYMax,
                     ts.scaleXMin , ts.scaleXMax,
                     ts.scaleYMin , ts.scaleYMax) ;
//    tsObject cell_3_2( ts.transXMin , ts.transXCenter,
//                       ts.transYCenter , ts.transYMax,
//                     ts.scaleXCenter , ts.scaleXMax,
//                     ts.scaleYMin , ts.scaleYCenter) ;
//    tsObject cell_3_3( ts.transXMin , ts.transXCenter,
//                       ts.transYCenter , ts.transYMax,
//                     ts.scaleXMin , ts.scaleXCenter,
//                     ts.scaleYCenter , ts.scaleYMax) ;
//    tsObject cell_3_4( ts.transXMin , ts.transXCenter,
//                       ts.transYCenter , ts.transYMax,
//                     ts.scaleXCenter , ts.scaleXMax,
//                     ts.scaleYCenter , ts.scaleYMax) ;

    tsObject cell_4_1( ts.transXCenter , ts.transXMax,
                       ts.transYCenter , ts.transYMax,
                     ts.scaleXMin , ts.scaleXMax,
                     ts.scaleYMin , ts.scaleYMax) ;
//    tsObject cell_4_2( ts.transXCenter , ts.transXMax,
//                       ts.transYCenter , ts.transYMax,
//                     ts.scaleXCenter , ts.scaleXMax,
//                     ts.scaleYMin , ts.scaleYCenter) ;
//    tsObject cell_4_3( ts.transXCenter , ts.transXMax,
//                       ts.transYCenter , ts.transYMax,
//                     ts.scaleXMin , ts.scaleXCenter,
//                     ts.scaleYCenter , ts.scaleYMax) ;
//    tsObject cell_4_4( ts.transXCenter , ts.transXMax,
//                       ts.transYCenter , ts.transYMax,
//                     ts.scaleXCenter , ts.scaleXMax,
//                     ts.scaleYCenter , ts.scaleYMax) ;

    // push subspaces into list
    subTransformSpace.push_back( cell_1_1 ) ;
//    subTransformSpace.push_back( cell_1_2 ) ;
//    subTransformSpace.push_back( cell_1_3 ) ;
//    subTransformSpace.push_back( cell_1_4 ) ;

    subTransformSpace.push_back( cell_2_1 ) ;
//    subTransformSpace.push_back( cell_2_2 ) ;
//    subTransformSpace.push_back( cell_2_3 ) ;
//    subTransformSpace.push_back( cell_2_4 ) ;

    subTransformSpace.push_back( cell_3_1 ) ;
//    subTransformSpace.push_back( cell_3_2 ) ;
//    subTransformSpace.push_back( cell_3_3 ) ;
//    subTransformSpace.push_back( cell_3_4 ) ;

    subTransformSpace.push_back( cell_4_1 ) ;
//    subTransformSpace.push_back( cell_4_2 ) ;
//    subTransformSpace.push_back( cell_4_3 ) ;
//    subTransformSpace.push_back( cell_4_4 ) ;

    return subTransformSpace ;
}

/**************************************************************************//**
 * @author Hannah Aker, Zachary Pierson, Steven Huerta
 *
 * @par Description:
 * transform - transforms list of points of the model
 *
 * @param[in] ts - object containing the transformation space
 * @param[in,out] transImage - object containing the transformation space
 *
 * @returns vector<point> - vector of transformed points
 *
 *****************************************************************************/
vector<point> transform(tsObject ts, Image_Model & transImage )
{
    point temp;
    // list to hold our transformed points
    vector<point> points;

    // for each point in the model, apply transform to point
    for( int i = 0; i < (int)transImage.points.size(); i++)
    {
        temp.x = transImage.points[i].x *ts.scaleXCenter + ts.transXCenter;
        temp.y = transImage.points[i].y *ts.scaleYCenter + ts.transYCenter;
        points.push_back(temp);
    }
    return points;
}

/**************************************************************************//**
 * @author Hannah Aker
 *
 * @par Description:
 * draw_box - draws boxes around transformation spaces
 *
 * @param[in,out] image - original image of target
 * @param[in,out] ts - object containing transformation space
 * @param[in] rows - model height
 * @param[in] cols - model width
 *
 *****************************************************************************/
void draw_box(Image& image, vector<tsObject> &ts, int rows, int cols)
{
    // four points for the corners of our outline box
    point p1, p2, p3, p4;
    for(unsigned int i = 0; i < ts.size(); i++)
    {
        // set the corners to the transformed model
        p1.x = ts[i].transXCenter;
        p1.y = ts[i].transYCenter;
        p2.x = ts[i].transXCenter + (cols * ts[i].scaleXCenter);
        p2.y = ts[i].transYCenter;
        p3.x = ts[i].transXCenter;
        p3.y = ts[i].transYCenter + (rows * ts[i].scaleYCenter);
        p4.x = ts[i].transXCenter + (cols * ts[i].scaleXCenter);
        p4.y = ts[i].transYCenter + (rows * ts[i].scaleYCenter);

        // draw the lines
        image.DrawLine(p1.x, p1.y, p2.x, p2.y, Pixel(0,255,0));
        image.DrawLine(p1.x, p1.y, p3.x, p3.y, Pixel(0,255,0));
        image.DrawLine(p2.x, p2.y, p4.x, p4.y, Pixel(0,255,0));
        image.DrawLine(p4.x, p4.y, p3.x, p3.y, Pixel(0,255,0));
    }
}

/**************************************************************************//**
 * @author Hannah Aker, Zachary Pierson, Steven Huerta
 *
 * @par Description:
 * validMatches - weeds out matches based on reverse Hausdorff
 *
 * @param[in,out] matches - list of transformation spaces
 * @param[in,out] target - Image_model object of the target image
 * @param[in,out] model - Image_model object of the model image
 * @param[in] rows - model height
 * @param[in] cols - model width
 *
 * @returns vector<tsObject> - list containing transformation spaces
 *
 *****************************************************************************/
vector<tsObject> validMatches ( vector<tsObject> & matches,
                                Image_Model & target, Image_Model & model,
                                double threshold, int percentList = 1.0 )
{
    // setup a list for our confirmed matches
    vector<tsObject> goodMatches;

    // set up a vector of points from the image
    vector<point> orgTargetPoints = target.points;

    // for each of the matches found
    for( unsigned int i = 0; i < matches.size(); i++ )
    {
        // get the list of partial reverse-box hausdorff distances
        vector<double> r_haus = reverse_hausdorff(matches[i], target, model);

        // calculate the index in the list to reference from the reverse
        // distances
        int hausIndex = percentList * (r_haus.size()-1);

        // get the distance at the index
        double hausDist = r_haus.at(hausIndex);

        // if the distance is less than the threshold push it into
        // the confirmed list of good matches
        if( hausDist < threshold )
        {
            goodMatches.push_back(matches[i]);
        }
        //target.points = orgTargetPoints;
    }
cout << "Finished Valid Matches!!" << endl;
   return goodMatches;


}
