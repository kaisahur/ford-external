#ifndef utilities_hpp
#define utilities_hpp

#include <string>
#include <framework/memory/memory.hpp>
#include <utilities/structs.hpp>
#include <framework/offsets.hpp>

inline memory_t g_memory;

struct object_base_t
{
    std::uint64_t address;

    object_base_t( ) : address( 0 )
    {
    }
    explicit object_base_t( std::uint64_t addr ) : address( addr )
    {
    }

    template< typename T >
    T get_member( std::uint64_t offset )
    {
        return g_memory.read< T >( this->address + offset );
    }
};

inline std::string read_string( std::uint64_t address )
{
    auto sso = g_memory.read< sso_string_t >( address );
    if ( sso.length <= 0 || sso.length > 256 )
        return "unknown";

    if ( sso.length < sizeof( sso.string.buffer ) )
    {
        return std::string( sso.string.buffer, sso.length );
    }

    std::string ret;
    ret.resize( sso.length );

    if ( g_memory.read_buffer( sso.string.ptr, ret.data( ), ret.size( ) ) )
        return "unknown";

    return ret.empty( ) ? "unknown" : ret;
}

#endif // utilities_hpp