#include <framework/memory/memory.hpp>

std::int32_t get_pid_by_name( const char* process_name )
{
    HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );

    if ( snapshot == INVALID_HANDLE_VALUE )
        return 0;

    PROCESSENTRY32 process_entry{};
    process_entry.dwSize = sizeof( process_entry );

    if ( Process32First( snapshot, &process_entry ) )
    {
        do
        {
            if ( _stricmp( process_entry.szExeFile, process_name ) == 0 )
            {
                std::int32_t pid = process_entry.th32ProcessID;
                CloseHandle( snapshot );
                return pid;
            }
        } while ( Process32Next( snapshot, &process_entry ) );
    }

    CloseHandle( snapshot );
    return 0;
}

std::uint64_t get_image_base_from_pid( std::int32_t pid )
{
    HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid );

    if ( snapshot == INVALID_HANDLE_VALUE )
        return 0;

    MODULEENTRY32 module_entry{};
    module_entry.dwSize = sizeof( module_entry );

    if ( Module32First( snapshot, &module_entry ) )
    {
        std::uint64_t base_address = reinterpret_cast< std::uint64_t >( module_entry.modBaseAddr );

        CloseHandle( snapshot );
        return base_address;
    }

    CloseHandle( snapshot );
    return 0;
}

bool memory_t::read_buffer( std::uint64_t address, void* buf, size_t size, PSIZE_T bytes_read )
{
    return nt_read_virtual_mem( handle, ( PVOID )address, ( void* )buf, size, bytes_read );
}

bool memory_t::write_buffer( std::uint64_t address, void* buf, size_t size, PSIZE_T bytes_read )
{
    return nt_write_virtual_mem( handle, ( PVOID )address, ( void* )buf, size, bytes_read );
}