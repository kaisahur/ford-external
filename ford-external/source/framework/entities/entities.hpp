#ifndef entities_hpp
#define entities_hpp

#include <framework/globals.hpp>
#include <shared_mutex>
#include <string>
#include <vector>

struct bone_t
{
    std::string key;
    std::uint64_t primitive;
};

struct entity_t
{
    std::string name;
    std::uint64_t humanoid;
    std::vector< bone_t > bones;
};

namespace cache
{
    inline std::vector< entity_t > entities;
    inline std::shared_mutex mtx;

    void update( );
    void tick( );

    std::vector< entity_t > get_snapshot( );
}  // namespace cache

#endif  // entities_hpp