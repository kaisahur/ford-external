#ifndef visuals_hpp
#define visuals_hpp

#include <framework/globals.hpp>
#include <imgui_internal.h>

struct visuals_t
{
    bool self = false;
    bool names = true;
    bool boxes = true;
    bool health_bar = true;

    bool calculate_bounds( visual_engine_t& engine, entity_t& entity, ImRect& rect );
    void render( );
};

inline std::shared_ptr< visuals_t > visuals = std::make_shared< visuals_t >( );

#endif // visuals_hpp
