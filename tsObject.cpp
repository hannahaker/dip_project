#include "tsObject.h"


tsOjbect::tsObject( int tXMin, int tXMax, int tYMin, int tYMax,
                    int sXMin, int sXMax, int sYMin, int sYMax )
{
    // Set the values for possible x,y translations
    transXMin = tXMin ;
    transXMax = tXMax ;
    transYMin = tYMin ;
    transYMax = tYMax ;

    // Set the values for possible x,y scaling
    scaleXMin = sXMin ;
    scaleXMax = sXMax ;
    scaleYMin = sYMin ;
    scaleYMax = sYMax ;

    // Calc the ceiling of the center of each dimension of the transformation
    // space
    transXCenter = ceil( ( transformSpace.transXMax +
                           transformSpace.transXMin ) / 2 );
    transYCenter = ceil( ( transformSpace.transYMax +
                           transformSpace.transYMin ) / 2 );
    scaleXCenter = ceil( ( transformSpace.scaleXMax +
                           transformSpace.scaleXMin ) / 2 );
    scaleYCenter = ceil( ( transformSpace.scaleYMax +
                           transformSpace.scaleYMin ) / 2 );

}

