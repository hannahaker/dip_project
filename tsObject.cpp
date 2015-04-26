#include "tsObject.h"

tsObject::tsObject( int tXMin, int tXMax, int tYMin, int tYMax,
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
    transXCenter = ceil( ( transXMax + transXMin ) / 2 );
    transYCenter = ceil( ( transYMax + transYMin ) / 2 );
    scaleXCenter = ceil( ( scaleXMax + scaleXMin ) / 2 );
    scaleYCenter = ceil( ( scaleYMax + scaleYMin ) / 2 );

}

tsObject tsObject::operator=( const tsObject &rhs)
{
    tsObject newTsObj(rhs.transXMin, rhs.transXMax, rhs.transYMin, rhs.transYMax,
                      rhs.scaleXMin, rhs.scaleXMax, rhs.scaleYMin, rhs.scaleYMax);
    return newTsObj;
}
