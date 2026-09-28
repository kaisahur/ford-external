#ifndef structs_hpp
#define structs_hpp

#include <cstdint>
#include <cstddef>
#include <framework/math.hpp>
#include <framework/offsets.hpp>

struct vector_t
{
    std::uint64_t first;
    std::uint64_t last;
    std::uint64_t end;
};

struct shared_pointer_t
{
    std::uint64_t object;
    std::uint64_t ref;
};

struct sso_string_t
{
    union
    {
        char buffer[ 16 ];
        std::uint64_t ptr;
    } string;

    std::size_t length;
    std::size_t capacity;
};

struct primitive_t
{
    matrix3 rotation;
    vector3 position;
    vector3 linear_velocity;
    vector3 angular_velocity;
    std::uint8_t pad_1[ Offsets::Primitive::Flags - ( Offsets::Primitive::AssemblyAngularVelocity + sizeof( vector3 ) ) ];
    std::uint8_t flags;
    std::uint8_t pad_2[ Offsets::Primitive::Size - ( Offsets::Primitive::Flags + 1 ) ];
    vector3 size;
};

#endif // structs_hpp
