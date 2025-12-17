#include "sample.h"
#include <mgl2/mgl.h>
//
int main( int, char** )
{
    mglGraph gr;
    //
    sample( &gr );
    gr.WriteSVG( "demo2.svg" );
    //
    return 0;
}
