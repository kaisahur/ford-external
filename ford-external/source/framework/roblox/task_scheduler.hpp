#ifndef task_scheduler_hpp
#define task_scheduler_hpp

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include <utilities/utilities.hpp>

struct job_t : object_base_t
{
    using object_base_t::object_base_t;

    std::string get_name( );
};

struct task_scheduler_t : object_base_t
{
    using object_base_t::object_base_t;

    bool is_loaded( )
    {
        return this->get_jobs( ).size( ) > 0;
    }

    std::vector< job_t > get_jobs( );
    std::uint64_t find_first_job( std::string_view name );
};

#endif  // task_scheduler_hpp