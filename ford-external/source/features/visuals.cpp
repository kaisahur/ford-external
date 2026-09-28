#include "visuals.hpp"

#include <imgui.h>

#include <cmath>

#include <framework/entities/entities.hpp>
#include <framework/roblox/visual_engine.hpp>

bool visuals_t::calculate_bounds( visual_engine_t& engine, entity_t& entity, float& left, float& top, float& right, float& bottom )
{
    bool any = false;
    left = 0.f;
    top = 0.f;
    right = 0.f;
    bottom = 0.f;

    for ( auto& bone : entity.bones )
    {
        const std::string& name = bone.key;
        if ( name != "Head" && name != "Torso" && name != "UpperTorso" && name != "LowerTorso" && name != "Left Arm" && name != "Right Arm" &&
             name != "Left Leg" && name != "Right Leg" && name != "LeftUpperArm" && name != "LeftLowerArm" && name != "LeftHand" &&
             name != "RightUpperArm" && name != "RightLowerArm" && name != "RightHand" && name != "LeftUpperLeg" && name != "LeftLowerLeg" &&
             name != "LeftFoot" && name != "RightUpperLeg" && name != "RightLowerLeg" && name != "RightFoot" )
            continue;

        if ( !bone.primitive )
            continue;

        auto prim = g_memory.read< primitive_t >( bone.primitive + Offsets::Primitive::Rotation );
        if ( !( prim.size.x > 0.05f && prim.size.x < 25.f ) || !( prim.size.y > 0.05f && prim.size.y < 25.f ) ||
             !( prim.size.z > 0.05f && prim.size.z < 25.f ) )
            continue;

        vector3 half = prim.size * 0.5f;
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

                    vector3 world{
                        prim.position.x + prim.rotation.m[ 0 ][ 0 ] * lx + prim.rotation.m[ 0 ][ 1 ] * ly + prim.rotation.m[ 0 ][ 2 ] * lz,
                        prim.position.y + prim.rotation.m[ 1 ][ 0 ] * lx + prim.rotation.m[ 1 ][ 1 ] * ly + prim.rotation.m[ 1 ][ 2 ] * lz,
                        prim.position.z + prim.rotation.m[ 2 ][ 0 ] * lx + prim.rotation.m[ 2 ][ 1 ] * ly + prim.rotation.m[ 2 ][ 2 ] * lz };

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

        for ( int i = 0; i < 8; i++ )
        {
            const float sx = corners[ i ].x;
            const float sy = corners[ i ].y;

            if ( !any )
            {
                left = right = sx;
                top = bottom = sy;
                any = true;
                continue;
            }

            if ( sx < left )
                left = sx;
            if ( sy < top )
                top = sy;
            if ( sx > right )
                right = sx;
            if ( sy > bottom )
                bottom = sy;
        }
    }

    return any && right > left && bottom > top;
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
    auto players = cache::get_snapshot( );

    for ( auto& entity : players )
    {
        float left = 0.f;
        float top = 0.f;
        float right = 0.f;
        float bottom = 0.f;

        if ( !calculate_bounds( engine, entity, left, top, right, bottom ) )
            continue;

        const float x1 = std::floor( left );
        const float y1 = std::floor( top );
        const float x2 = std::floor( right );
        const float y2 = std::floor( bottom );

        const auto flags = draw->Flags;
        draw->Flags &= ~( ImDrawListFlags_AntiAliasedLines | ImDrawListFlags_AntiAliasedFill );

        if ( visuals->boxes )
        {
            draw->AddRect( ImVec2( x1 - 1.f, y1 - 1.f ), ImVec2( x2 + 1.f, y2 + 1.f ), IM_COL32( 0, 0, 0, 255 ) );
            draw->AddRect( ImVec2( x1, y1 ), ImVec2( x2, y2 ), IM_COL32( 255, 255, 255, 255 ) );
            draw->AddRect( ImVec2( x1 + 1.f, y1 + 1.f ), ImVec2( x2 - 1.f, y2 - 1.f ), IM_COL32( 0, 0, 0, 255 ) );
        }

        if ( visuals->health_bar && entity.humanoid )
        {
            const float health = g_memory.read< float >( entity.humanoid + Offsets::Humanoid::Health );
            const float max_health = g_memory.read< float >( entity.humanoid + Offsets::Humanoid::MaxHealth );

            if ( max_health > 0.f )
            {
                float ratio = health / max_health;
                if ( ratio < 0.f )
                    ratio = 0.f;
                if ( ratio > 1.f )
                    ratio = 1.f;

                const float filled = std::floor( ( y2 - y1 ) * ratio );

                draw->AddRect( ImVec2( x1 - 6.f, y1 - 1.f ), ImVec2( x1 - 3.f, y2 + 1.f ), IM_COL32( 0, 0, 0, 255 ) );
                if ( filled >= 1.f )
                    draw->AddRectFilled( ImVec2( x1 - 5.f, y2 - filled ), ImVec2( x1 - 4.f, y2 ), IM_COL32( 255, 255, 255, 255 ) );
            }
        }

        draw->Flags = flags;
    }
}
