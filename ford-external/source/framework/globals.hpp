#ifndef globals_hpp
#define globals_hpp

#include <framework/roblox/datamodel.hpp>
#include <framework/math.hpp>
#include <memory>

struct game_t
{
    std::uint64_t last_datamodel;
    datamodel_t datamodel;
    instance_t players;

    matrix4 view_matrix;
    vector2 dimensions;

};

struct entity_t;
struct visual_engine_t;

struct visuals_t
{
    bool boxes = true;
    bool health_bar = true;

    bool calculate_bounds( visual_engine_t& engine, entity_t& entity, float& left, float& top, float& right, float& bottom );
    void render( );
};

inline std::shared_ptr< game_t > game = std::make_shared< game_t >( );
inline std::shared_ptr< visuals_t > visuals = std::make_shared< visuals_t >( );

#endif // globals_hpp