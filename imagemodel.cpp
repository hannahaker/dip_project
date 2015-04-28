#include "header.h"

/**************************************************************************//**
 * @author Zachary Pierson
 *
 * @par Description:
 * Image_Model Constructor - Constructs Image_Model object from Image
 *
 * @param[in,out] image1 - the target image to search
 *
 *****************************************************************************/
Image_Model::Image_Model( Image& img )
{
    // create a copy of the image
    image = img;

    // get size of the image
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

/**************************************************************************//**
 * @author Zachary Pierson
 *
 * @par Description:
 * Image_Model init_points - Assuming sobel edge has been applied, store all
 * (edge) points for the Huasdorff algorithm
 *
 * @return unsigned int - number of points in the image
 *
 *****************************************************************************/
unsigned int Image_Model::init_points()
{
    unsigned int r, c;
    // initiating the point object
    point pixl;

    // clear the vector
    points.clear();

    // store all the points remaining in the image
    for(r = 0; r < image.Height(); r++)
    {
        pixl.y = r;
        for(c = 0; c < image.Width(); c++)
        {
            pixl.x = c;
            if(image[r][c] > 200)
            {
                // push them into the vector containing all points in image
                points.push_back(pixl);
            }
        }
    }

    // return the vector size
    return points.size();
}

/**************************************************************************//**
 * @author Zachary Pierson
 *
 * @par Description:
 * init_voronoi_mask - Populate voronoi surface with distances the the nearest points.
 * Using the mask reduces computation time by about half, However: the mask
 * is only an approximation to the euclidian distance. There is about a 2%
 * error that is introduced using this 3x3 mask. This is explained by Gunilla
 * Borgefors in: "Distance Transformations in Digital Images"
 *
 *****************************************************************************/
void Image_Model::init_voronoi_mask()
{
    point pt;
    point p;
    queue<point> temp;
    double val;

    // store the distances from the center point as a 2D array
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
   //printf("time to calculate voronoi surface (mask): %f\n",
   //(double)(clock() - t)/CLOCKS_PER_SEC );

}

/**************************************************************************//**
 * @author Zachary Pierson
 *
 * @par Description:
 * Image_Model init_voronoi - Populate voronoi surface with distances to
 * the nearest points
 *  DO NOT USE: there is a bug and the distances are not correct!
 *  TO DO: Fix bug
 *
 *****************************************************************************/
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
   //printf("time to calculate voronoi surface (euclidean): %f\n",
   //(double)(clock() - t)/CLOCKS_PER_SEC );
}

/**************************************************************************//**
 * @author Zachary Pierson
 *
 * @par Description:
 * Image_Model display_voronoi - Used to check validity of voronoi surface
 *
 *****************************************************************************/
void Image_Model::display_voronoi()
{
    // Print the voronoi surface
    for(unsigned int i = 0; i < cols; i++)
    {
        for(unsigned int j = 0; j < rows; j++)
        {
            if(voronoi[i][j] > 20)
                printf("%f\n", voronoi[i][j]);
        }
    }
}

/**************************************************************************//**
 * @author Zachary Pierson
 *
 * @par Description:
 * Image_Model Constructor - Constructs Image_Model object from Image
 *
 * @param[in,out] image1 - the target image to search
 *
 *****************************************************************************/
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

/**************************************************************************//**
 * @author Zachary Pierson
 *
 * @par Description:
 * Image_Model match - Rescale image close to the size of the given image
 *
 *****************************************************************************/
bool Image_Model::match(Image& img)
{
    // resclase
    rescale( image, img.Height(), img.Width() );

    return true;
}

/**************************************************************************//**
 * @author Zachary Pierson
 *
 * @par Description:
 * Image_Model Destructor - Frees memory allocated for voronoi surface
 *
 *****************************************************************************/
Image_Model::~Image_Model()
{
    // release memory
    for( unsigned int i = 0; i < cols; i++ )
        free( voronoi[i] );
    free( voronoi );
}

