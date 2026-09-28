#include "visual_engine.hpp"

#include <framework/globals.hpp>

visual_engine_t visual_engine_t::get( )
{
    return visual_engine_t( g_memory.read< std::uint64_t >( g_memory.base + Offsets::VisualEngine::Pointer ) );
}

bool visual_engine_t::world_to_screen( vector3 pos, vector2& ret )
{
    float w = ( pos.x * game->view_matrix.m[ 3 ][ 0 ] ) + ( pos.y * game->view_matrix.m[ 3 ][ 1 ] ) + ( pos.z * game->view_matrix.m[ 3 ][ 2 ] ) +
              game->view_matrix.m[ 3 ][ 3 ];

    if ( w < 0.1f )
        return false;

    float x = ( pos.x * game->view_matrix.m[ 0 ][ 0 ] ) + ( pos.y * game->view_matrix.m[ 0 ][ 1 ] ) + ( pos.z * game->view_matrix.m[ 0 ][ 2 ] ) +
              game->view_matrix.m[ 0 ][ 3 ];
    float y = ( pos.x * game->view_matrix.m[ 1 ][ 0 ] ) + ( pos.y * game->view_matrix.m[ 1 ][ 1 ] ) + ( pos.z * game->view_matrix.m[ 1 ][ 2 ] ) +
              game->view_matrix.m[ 1 ][ 3 ];

    ret.x = ( game->dimensions.x / 2.f ) + ( x / w ) * ( game->dimensions.x / 2.f );
    ret.y = ( game->dimensions.y / 2.f ) - ( y / w ) * ( game->dimensions.y / 2.f );

    return true;
}
