#ifndef instance_hpp
#define instance_hpp

#include <vector>
#include <utilities/utilities.hpp>

struct instance_t : object_base_t
{
	using object_base_t::object_base_t;

	std::string get_name( );
    std::string get_class_name( );

    std::vector< instance_t > get_children( );

    instance_t find_first_child( std::string_view name );
    instance_t find_first_child_of_class( std::string_view name );
};

#endif // instance_hpp