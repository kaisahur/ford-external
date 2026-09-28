#include <framework/entities/entities.hpp>
#include <overlay/overlay.hpp>
#include <framework/globals.hpp>

int main( )
{
    if ( !g_overlay.init( ) )
        return 1;

    game->datamodel.tick( );
    cache::tick( );

    while ( g_overlay.begin_frame( ) )
    {
        g_overlay.end_frame( );
    }

    g_overlay.shutdown( );

    return 0;
}
