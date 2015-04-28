#include "header.h"

/**************************************************************************//**
 * @author Steven Huerta
 *
 * @par Description:
 * tsObject - CONTRUCTOR for the tsObject
 *
 * @param[in] tXmin - minimum translation along x
 * @param[in] tXmax - maximum translation along x
 * @param[in] tYmin - minimum translation along y
 * @param[in] tYmax - maximum translation along y
 * @param[in] sXmin - minimum scaling along x
 * @param[in] sXmax - maximum scaling along x
 * @param[in] sYmin - minimum scaling along y
 * @param[in] sYmax - maximum scaling along y
 *
 *****************************************************************************/
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

/**************************************************************************//**
 * @author Zachary Pierson
 *
 * @par Description:
 * Overloaded = Operator creates another tsObject with the same value
 *
 * @param[in,out] rhs - object to be copied
 *
 * @returns tsObject - copied object
 *
 *****************************************************************************/
tsObject tsObject::operator=( const tsObject &rhs)
{
    tsObject newTsObj(rhs.transXMin, rhs.transXMax, rhs.transYMin, rhs.transYMax,
                      rhs.scaleXMin, rhs.scaleXMax, rhs.scaleYMin, rhs.scaleYMax);
    return newTsObj;
}
