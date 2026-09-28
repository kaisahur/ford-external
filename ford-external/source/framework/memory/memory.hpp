#ifndef memory_hpp
#define memory_hpp

#include <Windows.h>

#include <cstddef>
#include <cstdint>

#include <TlHelp32.h>
#include <handleapi.h>

extern "C"
{
    long nt_read_virtual_mem( HANDLE, PVOID, PVOID, SIZE_T, PSIZE_T );
    long nt_write_virtual_mem( HANDLE, PVOID, PVOID, SIZE_T, PSIZE_T );
    long nt_alloc_virtual_mem( HANDLE, PVOID, PSIZE_T, ULONG, ULONG );
}

std::int32_t get_pid_by_name( const char* process_name );
std::uint64_t get_image_base_from_pid( std::int32_t pid );

struct memory_t
{
    HANDLE handle;
    std::uint64_t base;
    std::int32_t pid;

    memory_t( ) = default;
    memory_t( std::int32_t pid ) : pid( pid )
    {
        handle = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pid );
        base = get_image_base_from_pid( pid );
    }

    ~memory_t( )
    {
        if ( handle != INVALID_HANDLE_VALUE )
        {
            CloseHandle( handle );
        }
    }

    template< typename T >
    T read( std::uint64_t address )
    {
        T ret{};
        size_t bytes_read = 0;

        nt_read_virtual_mem( handle, ( PVOID )address, ( void* )&ret, sizeof( T ), &bytes_read );
        return ret;
    }

    template< typename T >
    bool write( std::uint64_t address, T& ret )
    {
        size_t bytes_read = 0;
        return nt_write_virtual_mem( handle, ( PVOID )address, ( void* )ret, sizeof( T ), &bytes_read );
    }

    bool read_buffer( std::uint64_t address, void* buf, size_t size, PSIZE_T bytes_read = 0 );
    bool write_buffer( std::uint64_t address, void* buf, size_t size, PSIZE_T bytes_read = 0 );
};

#endif  // memory_hpp