#include "entities.hpp"

void cache::update( )
{
    auto players = game->players.get_children( );

    std::vector< entity_t > temp;
    temp.reserve( players.size( ) );

    for ( auto& player : players )
    {
        auto model_instance = instance_t( g_memory.read< std::uint64_t >( player.address + Offsets::Player::ModelInstance ) );
        if ( !model_instance.address )
            continue;

        entity_t entity{};

        for ( auto& part : model_instance.get_children( ) )
        {
            auto class_name = part.get_class_name( );
            if ( class_name != "Part" && class_name != "MeshPart" )
            {
                continue;
            }

            bone_t bone;

            bone.key = part.get_name( );
            bone.primitive = g_memory.read< std::uint64_t >( part.address + Offsets::BasePart::Primitive );

            entity.bones.push_back( bone );
        }

        entity.humanoid = model_instance.find_first_child_of_class( "Humanoid" ).address;
        entity.name = player.get_name( );
        entity.self = player.address == game->local_player.address;
        temp.push_back( entity );
    }

    {
        std::unique_lock lock( mtx );
        entities = std::move( temp );
    }

    std::this_thread::sleep_for( std::chrono::milliseconds( 1500 ) );
    std::this_thread::yield( );
}

void cache::tick( )
{
    std::thread(
        []( )
        {
            while ( true )
            {
                update( );
            }
        } )
        .detach( );
}

std::vector< entity_t > cache::get_snapshot( )
{
    std::vector< entity_t > snapshot;
    {
        std::shared_lock lock( mtx );
        snapshot = entities;
    }
    return snapshot;
}
