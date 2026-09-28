#include <framework/entities/entities.hpp>
#include <overlay/overlay.hpp>
#include <framework/globals.hpp>

int main( )
{
    auto pid = get_pid_by_name( "RobloxPlayerBeta.exe" );
    if ( !pid )
        return 1;

    g_memory.pid = pid;
    g_memory.handle = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pid );
    g_memory.base = get_image_base_from_pid( pid );

    if ( !g_memory.handle || !g_memory.base )
        return 1;

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
