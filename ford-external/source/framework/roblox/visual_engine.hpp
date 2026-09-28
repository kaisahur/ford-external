#ifndef visual_engine_hpp
#define visual_engine_hpp

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include <utilities/utilities.hpp>
#include <framework/math.hpp>

struct visual_engine_t : object_base_t
{
    using object_base_t::object_base_t;

    static visual_engine_t get( );
    bool world_to_screen( vector3 pos, vector2& ret );
};

#endif  // visual_engine_hpp