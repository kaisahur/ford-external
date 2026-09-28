#include "instance.hpp"

std::string instance_t::get_name( )
{
    const auto name_container = get_member< std::uint64_t >( Offsets::Instance::NameContainer );
    if ( !name_container )
        return "unknown";
    return read_string( name_container + 0x8 );
}

std::string instance_t::get_class_name( )
{
    const auto class_desc = get_member< std::uint64_t >( Offsets::Instance::ClassDescriptor );
    if ( !class_desc )
        return "unknown";

    const auto class_name = g_memory.read< std::uint64_t >( class_desc + 0x8 );
    if ( !class_name )
        return "unknown";

    return read_string( class_name );
}

std::vector< instance_t > instance_t::get_children( )
{
    std::vector< instance_t > children;

    const auto container = get_member< std::uint64_t >( Offsets::Instance::ChildrenStart );
    const auto vector = g_memory.read< vector_t >( container );

    if ( !vector.first || !vector.last )
        return children;

    auto count = ( vector.last - vector.first ) / sizeof( shared_pointer_t );
    if ( count <= 0 || count > 4000 )
        return children;

    children.reserve( count );

    std::vector< shared_pointer_t > buf( count );
    if ( g_memory.read_buffer( vector.first, buf.data( ), buf.size( ) * sizeof( shared_pointer_t ) ) )
        return children;

    for ( std::uint64_t i = 0; i < count; i++ )
    {
        auto& cur_slot = buf[ i ];
        if ( !cur_slot.object )
            break;

        children.push_back( instance_t( cur_slot.object ) );
    }

    return children;
}

instance_t instance_t::find_first_child( std::string_view name )
{
    for ( auto& child : get_children( ) )
    {
        if ( child.get_name( ) == name )
        {
            return child;
        }
    }
    return instance_t( );
}

instance_t instance_t::find_first_child_of_class( std::string_view name )
{
    for ( auto& child : get_children( ) )
    {
        if ( child.get_class_name( ) == name )
        {
            return child;
        }
    }
    return instance_t( );
}
