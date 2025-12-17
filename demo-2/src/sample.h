#include <mgl2/mgl.h>
//
void sample( mglGraph* gr )
{
    gr->SubPlot( 1, 1, 0, "<_" );
    // if ( big != 3 )
        gr->Title( "Complex ODE sample" );
    double  t = 50;
    mglData ini;
    ini.SetList( 3, 1., 1e-3, 0. );
    mglDataC r( mglODEc( "-b*f;a*conj(f);a*conj(b)-0.1*f", "abf", ini, 0.1, t ) );
    gr->SetRanges( 0, t, 0, r.Maximal() );
    gr->Plot( r.SubData( 0 ), "b", "legend 'a'" );
    gr->Plot( r.SubData( 1 ), "g", "legend 'b'" );
    gr->Plot( r.SubData( 2 ), "r", "legend 'f'" );
    gr->Axis();
    gr->Box();
    gr->Legend();
}