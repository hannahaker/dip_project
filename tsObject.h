#ifndef TSOBJECT_H
#define TSOBJECT_H

class tsObject
{
public:
    // METHODS

    // CONSTRUCTORS
    tsObject( int tXMin, int tXMax, int tYMin, int tYMax,
              int sXMin = 1, int sXMax = 1, int sYMin = 1, int sYMax = 1) ;

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

    int rotThetaMin ;
    int rotThetaMax ;
    int rotThetaCenter ;


};

#endif //TSOBJECT_H
