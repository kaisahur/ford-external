#include "visuals.hpp"

#include <imgui.h>

#include <algorithm>
#include <cmath>
#include <framework/entities/entities.hpp>
#include <framework/roblox/visual_engine.hpp>

#undef max
#undef min

bool visuals_t::calculate_bounds( visual_engine_t& engine, entity_t& entity, ImRect& bb )
{
    bool any = false;

    for ( const auto& bone : entity.bones )
    {
        if ( !bone.primitive )
            continue;

        auto prim = g_memory.read< primitive_t >( bone.primitive + Offsets::Primitive::Rotation );

        vector3 half = prim.size / 2.f;
        vector2 corners[ 8 ];
        bool valid = true;
        int n = 0;

        for ( int x = -1; x <= 1 && valid; x += 2 )
        {
            for ( int y = -1; y <= 1 && valid; y += 2 )
            {
                for ( int z = -1; z <= 1 && valid; z += 2 )
                {
                    const float lx = half.x * x;
                    const float ly = half.y * y;
                    const float lz = half.z * z;

                    vector3 world{ prim.position.x + prim.rotation.m[ 0 ][ 0 ] * lx + prim.rotation.m[ 0 ][ 1 ] * ly + prim.rotation.m[ 0 ][ 2 ] * lz,
                                   prim.position.y + prim.rotation.m[ 1 ][ 0 ] * lx + prim.rotation.m[ 1 ][ 1 ] * ly + prim.rotation.m[ 1 ][ 2 ] * lz,
                                   prim.position.z + prim.rotation.m[ 2 ][ 0 ] * lx + prim.rotation.m[ 2 ][ 1 ] * ly +
                                       prim.rotation.m[ 2 ][ 2 ] * lz };

                    if ( !engine.world_to_screen( world, corners[ n ] ) )
                    {
                        valid = false;
                        break;
                    }

                    n++;
                }
            }
        }

        if ( !valid )
            continue;

        any = true;

        for ( int i = 0; i < 8; i++ )
        {
            auto& corner = corners[ i ];

            bb.Min.x = std::min( bb.Min.x, corner.x );
            bb.Min.y = std::min( bb.Min.y, corner.y );

            bb.Max.x = std::max( bb.Max.x, corner.x );
            bb.Max.y = std::max( bb.Max.y, corner.y );
        }
    }

    return any && bb.Max.x > bb.Min.x && bb.Max.y > bb.Min.y;
}

void visuals_t::render( )
{
    if ( !visuals->boxes && !visuals->health_bar )
        return;

    auto engine = visual_engine_t::get( );
    if ( !engine.address )
        return;

    game->dimensions = engine.get_member< vector2 >( Offsets::VisualEngine::Dimensions );
    game->view_matrix = engine.get_member< matrix4 >( Offsets::VisualEngine::ViewMatrix );

    if ( game->dimensions.x < 1.f || game->dimensions.y < 1.f )
        return;

    auto draw = ImGui::GetBackgroundDrawList( );

    for ( auto& entity : cache::get_snapshot( ) )
    {
        if ( !visuals->self && entity.self )
            continue;

        ImRect bb{ FLT_MAX, FLT_MAX, -FLT_MAX, -FLT_MAX };

        if ( !calculate_bounds( engine, entity, bb ) )
            continue;

        bb.Min.x = std::floorf( bb.Min.x );
        bb.Min.y = std::floorf( bb.Min.y );

        bb.Max.x = std::floorf( bb.Max.x );
        bb.Max.y = std::floorf( bb.Max.y );

        if ( visuals->boxes )
        {
            draw->AddRect( bb.Min, bb.Max, IM_COL32( 0, 0, 0, 255 ), 0.f, 0, 3.f );
            draw->AddRect( bb.Min, bb.Max, IM_COL32( 255, 255, 255, 255 ), 0.f, 0, 1.f );
        }

        if ( visuals->health_bar && entity.humanoid )
        {
            const float health = g_memory.read< float >( entity.humanoid + Offsets::Humanoid::Health );
            const float max_health = g_memory.read< float >( entity.humanoid + Offsets::Humanoid::MaxHealth );

            if ( max_health > 0.f )
            {
                float ratio = std::clamp( health / max_health, 0.f, 1.f );
                const float filled = std::floor( ( bb.Max.y - bb.Min.y ) * ratio );

                draw->AddRect( ImVec2( bb.Min.x - 5.f, bb.Min.y - 1.f ), ImVec2( bb.Min.x - 2.f, bb.Max.y + 1.f ), IM_COL32( 0, 0, 0, 255 ) );
                if ( filled >= 1.f )
                {
                    draw->AddRectFilled(
                        ImVec2( bb.Min.x - 4.f, bb.Max.y - filled ), ImVec2( bb.Min.x - 3.f, bb.Max.y ), IM_COL32( 255, 255, 255, 255 ) );
                }
            }
        }
    }
}
