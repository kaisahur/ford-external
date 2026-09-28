#ifndef datamodel_hpp
#define datamodel_hpp

#include <framework/roblox/instance.hpp>

struct datamodel_t : instance_t 
{
    using instance_t::instance_t;

    std::uint64_t get_place_id( );
    std::uint64_t get_creator_id( );
    std::uint64_t get_job_id( );
    std::uint64_t get_game_id( );

    bool is_in_game( );
    bool is_game_loaded( );
    static datamodel_t get( );

    void tick( );
};

#endif // datamodel_hpp