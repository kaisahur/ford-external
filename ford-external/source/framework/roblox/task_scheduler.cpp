#include <framework/memory/memory.hpp>
#include <framework/roblox/task_scheduler.hpp>
#include <framework/globals.hpp>

std::string job_t::get_name( )
{
    return read_string( this->address + Offsets::TaskScheduler::JobName );
}

std::vector< job_t > task_scheduler_t::get_jobs( )
{
    std::vector< job_t > jobs;

    const auto container = get_member<std::uint64_t>( Offsets::TaskScheduler::JobStart );
    const auto vector = g_memory.read< vector_t >( container );

    if ( !vector.first || !vector.last )
        return jobs;

    auto count = ( vector.last - vector.first ) / sizeof( shared_pointer_t );
    if ( count <= 0 || count > 4000 )
        return jobs;

    jobs.reserve( count );

    std::vector< shared_pointer_t > buf( count );
    if ( g_memory.read_buffer( vector.first, buf.data( ), buf.size( ) * sizeof( shared_pointer_t ) ) )
        return jobs;

    for ( std::uint64_t i = 0; i < count; i++ )
    {
        auto& cur_slot = buf[ i ];
        if ( !cur_slot.object )
            break;

        jobs.push_back( job_t( cur_slot.object ) );
    }

    return jobs;
}

std::uint64_t task_scheduler_t::find_first_job( std::string_view name )
{
    for ( job_t entry : get_jobs( ) )
    {
        if ( entry.get_name( ) == name )
            return entry.address;
    }

    return 0;
}