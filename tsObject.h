#ifndef TSOBJECT_H
#define TSOBJECT_H



struct tsObject
{
    // METHODS

    // CONSTRUCTORS
    tsObject( tXMin, tXMax, tYMin, tYMax,
              sXMin = 1, sXMax = 1, sYMin = 1, sYMax = 1) ;

    int transXMin ;
    int transXMax ;
    int transXCenter ;

    int transYMin ;
    int transYMax ;
    int transYCenter ;

    int scaleXMin ;
    int scaleXMax ;
    int scaleXCenter ;

    int scaleYMin ;
    int scaleYMax ;
    int scaleYCenter ;

    int rotThetaMin = 1;
    int rotThetaMax = 1;
    int rotThetaCenter ;


};

tsObject( tXMin, tXMax, tYMin, tYMax,
          sXMin = 1, sXMax = 1, sYMin = 1, sYMax = 1)
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





#endif // TSOBJECT_H
