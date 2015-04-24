// THIS FILE IS TO BE ADDED TO OTHER FILES LATER
// IT IS ONLY TO HOLD FUNCTIONS AS A STAGING AREA
#include <tsObject.h>
#include <qtip.h>

int getGamma( tsObject tranfSpace )
{
    int coords[2] = ( ( tranfSpace.transXMax - tranfSpace.transXMin )/2 +
                      ( tranfSpace.scaleXMax - tranfSpace.scaleXMin ) ,
                      ( tranfSpace.transYMax - tranfSpace.transYMin )/2 +
                      ( tranfSpace.scaleYMax - tranfSpace.scaleYMin ) ) ;

    int gamma = pow( coords[0] , 2 ) + pow( coords[1] , 2 ) ;

    gamma = pow( gamma, 0.5 ) ;

    return gamma ;
}

bool isIntersting( tsObject transSpace, Image I, Image M, float k, int threshold )
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

vector<tsObject> divideSpace( tsObject trfSpace )
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

vector<tsObject> decomp( Image I, Image M, int t, float k = 1.0, int alpha = 1 )
{
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

    // JUST FOR TESTING PURPOSES
    for( int i = 0 ; i < (int)tsList.size() ; i++ )
    {
        if( getGamma( tsList[i] ) == 0 )
        {
            matches.push_back( tsList[i] ) ;
        }
    }

    if( matches.size() == tsList.size() )
        return true ;
    else return false ;

    // END TESTING

    return tsList ;
}
