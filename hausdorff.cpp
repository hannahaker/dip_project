#include "hausdorff.h"

Image_Model::Image_Model( Image& img )
{
    image = img;
    init_points();
}

//assuming sobel edge has been applied
//store all (edge) points for the Huasdorff algorithm
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

//rescale image close to the size of the given image
bool Image_Model::match(Image& img)
{
    double widthFactor;
    double heightFactor;
    int r = image.Height();
    int c = image.Width();

    widthFactor = (double)image.Width()/(double)img.Width();
    heightFactor = (double)image.Height()/(double)img.Height();

    r = r/heightFactor;
    c = c/widthFactor;
    
    if ( r < 1 || r > 16384 || c < 1 || c > 16384 ) return false;
    rescale( image, r, c );

    return true;
}

//Successive thinnings until there is no more cahnge.
//This allows a smaller set of points to work with for the Hausdorff
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

double euclidean_dist(point A, point B)
{
    return sqrt( pow(A.x - B.x, 2) + pow(A.y - B.y, 2));
}

//returns a list of the directed hausorff distances.  An ordered list with 
//best matching distances first
vector<double> directed_hausdorff(vector<point>& model, vector<point>& target)
{
    double max = -1;
    double min = 9999;
    double dist;
    vector<double> distance;

    if(target.size() == 0)
        return distance;

    //find maximum of the smallest distances from the model to the target
    for(unsigned int i = 0; i < model.size(); i++)
    {
        for(unsigned int j = 0; j < target.size(); j++)
        {
            dist = euclidean_dist(model[i], target[j]);
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

        //save the the smallest distance from model[i] to target
        distance.push_back(min);
        min = 9999;
    }

    sort(distance.begin(), distance.end());
    cout << "Hausdorff Distance: " << distance.back() << endl;

    return distance;
}

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
