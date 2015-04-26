#include "hausdorff.h"

/************************************************************************
   Function: Image_Model Constructor
   Author: Zachary Pierson
   Description: Constructs Image_Model object from Image
 ************************************************************************/
Image_Model::Image_Model( Image& img )
{
    image = img;

    cols = image.Width();
    rows = image.Height();

    //allocate memory for voronoi surface, to be filled later with init_voronoi_mask
    voronoi = (double**) malloc(cols * sizeof(double*));
    if(voronoi == NULL)
        exit(1);
    for( unsigned int i = 0; i < cols; i++ )
    {
        voronoi[i] = (double*) malloc(rows * sizeof(double));
        if(voronoi[i] == NULL)
            exit(1);
    }


    init_points();
    trans.x = 0;
    trans.y = 0;
    trans.scale = 1.0;
}


/************************************************************************
   Function: Image_Model init_points
   Author: Zachary Pierson
   Description: Assuming sobel edge has been applied, store all (edge)
        points for the Huasdorff algorithm
 ************************************************************************/
unsigned int Image_Model::init_points()
{
    unsigned int r, c;
    point pixl;

    points.clear();

    for(r = 0; r < image.Height(); r++)
    {
        pixl.y = r;
        for(c = 0; c < image.Width(); c++)
        {
            pixl.x = c;
            if(image[r][c] > 200)
            {
                points.push_back(pixl);
            }
        }
    }
    return points.size();
}


/************************************************************************
   Function: Image_Model init_voronoi_mask
   Author: Zachary Pierson
   Description: Populate voronoi surface with distances the the nearest points.
        Using the mask reduces computation time by about half, However: the mask
        is only an approximation to the euclidian distance. There is about a 2%
        error that is introduced using this 3x3 mask. This is explained by Gunilla
        Borgefors in: "Distance Transformations in Digital Images"
 ************************************************************************/
void Image_Model::init_voronoi_mask()
{
    point pt;
    point p;
    queue<point> temp;
    double val;
    double mask[3][3] = {{1.3507, 1.0, 1.35707},
                        {1.0, 0, 1.0},
                        {1.3507, 1.0, 1.35707}};

    //initialize voronoi with all -1
    for(unsigned int c = 0; c < cols; c++)
        for(unsigned int r = 0; r < rows; r++)
            voronoi[c][r] = -1;

    //init voronoi with points
    for(unsigned int i = 0; i < points.size(); i++)
        voronoi[points[i].x][points[i].y] = 0;

    //clock_t t = clock();

    //initialize the queue
    for(unsigned int i = 0; i < points.size(); i++)
        temp.push(points[i]);

    //Do wavefront algorithm to find distances
    while(!temp.empty()) 
    {
        pt = temp.front();
           
        //loop through the neighborhood
        for(int x = pt.x - 1; x <= pt.x+1; x++)
        {
            //skip point if out of the x range
            if(x < 0 || x >= (int)cols)
                continue;

            for(int y = pt.y - 1; y <= pt.y+1; y++)
            {
                //skip point if out of the y range
                if(y < 0 || y >= (int)rows)
                    continue;
                
                //compute and apply mask: voronoi[pt.x][pt.y] + mask
                val = voronoi[pt.x][pt.y] + mask[(x-pt.x)+1][(y-pt.y)+1];
                
                //if voronoi not set, set it and push point on the queue
                if(voronoi[x][y] < 0)
                {
                    p.x = x;
                    p.y = y;
                    voronoi[x][y] = val;   //applay mask!!
                    temp.push(p);
                }
                //voronoi set, but computed smaller dist, reset it
                else if( voronoi[x][y] > val )
                    voronoi[x][y] = val;
            }
        }
        temp.pop();
    }
   //printf("time to calculate voronoi surface (mask): %f\n", (double)(clock() - t)/CLOCKS_PER_SEC );

}


/************************************************************************
   Function: Image_Model init_voronoi
   Author: Zachary Pierson
   Description: Populate voronoi surface with distances to the nearest points
   DO NOT USE: there is a bug and the distances are not correct!
   TO DO: Fix bug
 ************************************************************************/
void Image_Model::init_voronoi()
{
    //clock_t t = clock();
    point pt;
    vector<queue<point> > temp;
    double dist;
    bool done = false;
    unsigned int size;

    //initialize voronoi with all infinity
    for(unsigned int c = 0; c < cols; c++)
        for(unsigned int r = 0; r < rows; r++)
            voronoi[c][r] = DBL_MAX;

    //initialize vector of queues
    temp.resize(points.size());
    for(unsigned int i = 0; i < points.size(); i++)
        temp[i].push(points[i]);


    //Do wavefront algorithm to find distances
   while(!done) 
   {
       done = true;

       //For each point
       for( unsigned int i = 0; i < points.size(); i++)
       {
           size = temp[i].size();

           //do one iteration
           for( unsigned int j = 0; j < size; j++)
           {
               pt = temp[i].front();
               dist = euclidean_dist( pt, points[i] );
                
               if( dist < voronoi[pt.x][pt.y] )
               {
                   voronoi[pt.x][pt.y] = dist;
                   push_neighbors( temp[i], pt );
                   done = false;
               }
                
               temp[i].pop();
           }
       }
   }
   //printf("time to calculate voronoi surface (euclidean): %f\n", (double)(clock() - t)/CLOCKS_PER_SEC );
}


/************************************************************************
   Function: Image_Model display_voronoi
   Author: Zachary Pierson
   Description: Used to check validity of voronoi surface
 ************************************************************************/
void Image_Model::display_voronoi()
{
    for(unsigned int i = 0; i < cols; i++)
    {
        for(unsigned int j = 0; j < rows; j++)
        {
            if(voronoi[i][j] > 20)
                printf("%f\n", voronoi[i][j]);
        }
    }
}

/************************************************************************
   Function: Image_Model push_neighbors
   Author: Zachary Pierson
   Description: This is a helper function for the original init_voronoi
 ************************************************************************/
int Image_Model::push_neighbors(queue<point> &q, point p)
{
    point temp = p;
    int count = 0;

    //left point
    temp.x = p.x-1;
    if( temp.x >= 0  && voronoi[temp.x][temp.y] > 99999 )
    {
        q.push(temp);
        count++;
    }

    //right point
    temp.x = p.x+1;
    if( temp.x < (int)cols && voronoi[temp.x][temp.y] > 99999 )
    {
        q.push(temp);
        count++;
    }

    //set x back to center
    temp.x = p.x;

    //upper point
    temp.y = p.y-1;
    if( temp.y >= 0 && voronoi[temp.x][temp.y] > 99999 )
    {
        q.push(temp);
        count++;
    }

    //lower point
    temp.y = p.y+1;
    if( temp.y < (int)rows && voronoi[temp.x][temp.y] > 99999 )
    {
        q.push(temp);
        count++;
    }

    return count;
}


/************************************************************************
   Function: Image_Model match
   Author: Zachary Pierson
   Description: Rescale image close to the size of the given image
 ************************************************************************/
bool Image_Model::match(Image& img)
{
    rescale( image, img.Height(), img.Width() );

    return true;
}


/************************************************************************
   Function: Image_Model thin
   Author: Zachary Pierson
   Description: Successive thinnings until there is no more change. This
        allows a smaller set of points to work with for the Hausdorff
 ************************************************************************/
bool Image_Model::thin()
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
   Function: Image_Model Destructor
   Author: Zachary Pierson
   Description: Frees memory allocated for voronoi surface
 ************************************************************************/
Image_Model::~Image_Model()
{
    for( unsigned int i = 0; i < cols; i++ )
        free( voronoi[i] );
    free( voronoi );
}

/************************************************************************
   Function: euclidean_dist
   Author: Zachary Pierson
   Description: calculates the Euclidean Distance between two points
 ************************************************************************/
double euclidean_dist(point A, point B)
{
    return sqrt( pow(A.x - B.x, 2) + pow(A.y - B.y, 2));
}


/************************************************************************
   Function: Forward Hausdorff
   Author: Zachary Pierson
   Description: Returns a list of the directed hausorff distances.  An ordered
        list with best matching distances first. This is ORDERS of magnitueds
        FASTER than the original hausdorff distance calculation. Uses voronoi
        surface to faster calculate Hausdorff values.
 ************************************************************************/
vector<double> forward_hausdorff(vector<point>& pts, double** surface)
{
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
}


/************************************************************************
   Function: Box Reverse Hausdorff
   Author: Zachary Pierson
   Description: Box Reverse Hausdorff. Returns a list of the directed hausorff
        distances.  An ordered list with best matching distances first. This is
        ORDERS of magnitueds FASTER than the original hausdorff distance calculation.
   TO DO: need to implement
 ************************************************************************/
vector<double> reverse_hausdorff(Image_Model& target, Image_Model& model)
{
    //clock_t t = clock();
    vector<double> distance;

    //make sure there are points to process
    if(target.points.size() == 0)
        return distance;

    for(unsigned int i = 0; i < target.points.size(); i++)
    {
        //Continue if index outside of model space
        if(target.points[i].x < 0 || target.points[i].x >= (int)model.rows || target.points[i].y < 0 || target.points[i].y >= (int)model.cols)
            continue;

        distance.push_back(model.voronoi[target.points[i].x][target.points[i].y]);
    }

    sort(distance.begin(), distance.end());
    cout << "Hausdorff Distance (Voronoi): " << distance.back() << endl;

   //printf("time to calculate hausdorff (voronoi): %f\n", (double)(clock() - t)/CLOCKS_PER_SEC );

    return distance;
}


/************************************************************************
   Function: Directed Hausdorff
   Author: Zachary Pierson
   Description: Hausdorff WITHOUT voronoi surface. This is ORDERS of
        magnitudes SLOWER than using the voronoi surface as a look up
        table for distances.
 ************************************************************************/
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

    sort(distance.begin(), distance.end());
    cout << "Hausdorff Distance: " << distance.back() << endl;

   //printf("time to calculate hausdorff: %f\n", (double)(clock() - t)/CLOCKS_PER_SEC );

    return distance;
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


/************************************************************************
   Function: Decomp
   Author: Hannah Aker
   Description: Finds interesting transformation spaces
 ************************************************************************/

vector<tsObject> decomp(Image_Model& target, Image_Model& model, int t, float k = 1.0, int alpha = 1)
{
    int imageRows = target.Height() ;
    int imageCols = target.Width() ;

    int modelRows = model.Height() ;
    int modelCols = model.Width() ;

    // initialize our first transformation space paramaters
    tsObject *tsObj = new tsObject(0, imageCols - modelCols, 0, imageRows - modelCols1,1,1,1);

    // get our distance of possible transforms in the transform space
    int gamma = calcGamma(matches.front());

    vector<tsObject> matches;
    matches.push_back(*tsObj);
    //while( cellsize(matches.front())!=0)
    while (!matches.size()!=0)
    {
        // get our distance of possible transforms in the transform space
        gamma = calcGamma(matches.front());
        thresh = t + gamma;
        if ( isInteresting( matches.front(), target, model, k, thresh ) )
        {
            vector<tsObject> divided = divide(matches.front());
            tsList.insert( tsList.end(), divided.begin(), divided.end() ) ;

        }
        matches.erase( matches.begin() ) ;

    }
    return matches;
}
/*
    int cell ;
    int imageRows = I.Height() ;
    int imageCols = I.Width() ;

    int modelRows = M.Height() ;
    int modelCols = I.Width() ;

    // initialize our first transformation space paramaters
    tsObject transfSpace( 0, imageCols - modelCols, 0, imageRows - modelCols ) ;

    // get our distance of possible transforms in the transform space
    int gamma = getGamma( transfSpace ) ;

    // temp vector to test our matches ;
    vector<tsObject> matches ;

    vector<tsObject> tsList ;

    tsList.push_back( tranfSpace ) ;

    while( gamma !=0 )
    {
        threshold = t + gamma ;

        if( isInteresting( tsList.front(), I, M, threshold, k ) )
        {
            vector<tsObject> temp ;
            temp = divideSpace( ts.List.front() ) ;
            tsList.insert( tsList.end(), temp.begin(), temp.end() ) ;
        }

        tsList.erase( tsList.begin() ) ;

        gamma = getGamma( tsList.begin() ) ;
    }

 */


/************************************************************************
   Function: calcGamma
   Author: Steven Heurta
   Description: Calculate Gamma for Decomp function
 ************************************************************************/
int calcGamma( tsObject tranfSpace)
{
    int coords[2] = { ( tranfSpace.transXMax - tranfSpace.transXMin )/2 +
                      ( tranfSpace.scaleXMax - tranfSpace.scaleXMin ) ,
                      ( tranfSpace.transYMax - tranfSpace.transYMin )/2 +
                      ( tranfSpace.scaleYMax - tranfSpace.scaleYMin ) } ;

    int gamma = pow( coords[0] , 2 ) + pow( coords[1] , 2 ) ;

    gamma = pow( gamma, 0.5 ) ;

    return gamma ;
}


/************************************************************************
   Function: isInteresting
   Author: Steven Heurta
   Description: determines if a transformation space is intersting, runs
       hausdorff and examines values returned.
 ************************************************************************/
bool isInteresting( tsObject transSpace, Image_Model I, Image_Model M, float k, int threshold )
{
    int distance = 0 ; // placeholder

    // transform model with quadruple
    // (transSpace.transXCenter, transSpace.transYCenter,
    //  transSpace.scaleXCenter, transSpace.scaleYCenter

    // compare partial forward HD ( t(M), I ) at k * #points in sorted
    // forward distance list

    if( distance < threshold )
        return true ;
    else
        return false ;
}

/************************************************************************
   Function: isInteresting
   Author: Steven Heurta
   Description: divides the transformation into 4 smaller transformation
       spaces
 ************************************************************************/
vector<tsObject> divide( tsObject trfSpace )
{
    vector<tsObject> subTransformSpace ;

    tsObject cell_1( trfSpace.transXMin , trfSpace.transXCenter,
                     trfSpace.transYMin , trfSpace.transYCenter ) ;
    tsObject cell_2( trfSpace.transXCenter , trfSpace.transXMax,
                     trfSpace.transYMin , trfSpace.transYCenter ) ;
    tsObject cell_3( trfSpace.transXMin , trfSpace.transXCenter,
                     trfSpace.transYCenter , trfSpace.transYMax ) ;
    tsObject cell_4( trfSpace.transXCenter , trfSpace.transXMax,
                     trfSpace.transYCenter , trfSpace.transYMax ) ;

    subTransformSpace.push_back( cell_1 ) ;
    subTransformSpace.push_back( cell_2 ) ;
    subTransformSpace.push_back( cell_3 ) ;
    subTransformSpace.push_back( cell_4 ) ;

    return subTransformSpace ;
}

//queue<tsObject> decomp(Image_Model& image, Image_Model& model, int alpha)
//{
//    tsObject tsObj = new tsObject(0,0,0,0,1,1,1,1);
//    int gamma = 0;
//    //gamma = calcGamma(tsObj);
//    queue<tsObject> matches = new vector<tsObject>;
//    matches.push_back(tsObj);
//    //while( cellsize(matches.front())!=0)
//    while (!matches.isEmpty())
//    {
//        //gamma = calcGamma(matches.front());
//        //if ( isInteresting( matches.front(), image, model )
//        //{
//        //matches.add( divide(matches.front()));
//        //}
//        matches.pop_front();
//
//    }
//    return matches;
//}
//
//int calcGamma( tsObject tsObj)
//{
//
//    int gamma = 0;
//    return gamma;
//}


//vector<point> transform(tsObject ts, Image_Model & )

