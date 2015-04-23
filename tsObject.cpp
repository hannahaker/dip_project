

struct tsObject
{
    // METHODS

    // CONSTRUCTORS
    tsObject( tXMin, tXMax, tYMin, tYMax,
              sXMin = 1, sXMax = 1, sYMin = 1, sYMax = 1) ;

    int transXMin ;
    int transXMax ;
    int transYMin ;
    int transYMax ;

    int scaleXMin ;
    int scaleXMax ;
    int scaleYMin ;
    int scaleYMax ;

    int rotThetaMin = 1;
    int rotThetaMax = 1;
};

tsObject( tXMin, tXMax, tYMin, tYMax,
          sXMin = 1, sXMax = 1, sYMin = 1, sYMax = 1)
{

    transXMin = tXMin ;
    transXMax = tXMax ;
    transYMin = tYMin ;
    transYMax = tYMax ;

    scaleXMin = sXMin ;
    scaleXMax = sXMax ;
    scaleYMin = sYMin ;
    scaleYMax = sYMax ;

}



